//
//  Game.cpp
//  Game-mac
//

#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include "Random.h"
#include "Frog.h"
#include "Vehicle.h"
#include "Log.h"
#include "WrappingMove.h"
#include "SpriteComponent.h"
#include "Frog.h"
#include "CollisionComponent.h"
#include "Actor.h"

Game::Game(){
    isGameOver = false;
}

bool Game::Initialize(){
    Random::Init();
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow(
        "Lab3",                            // Title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH/2,                      // width
        WINDOW_HEIGHT/2,                     // height
        0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    int flags=IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if((initted&flags) != flags) {
        printf("IMG_Init: Failed to init required png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
    LoadData();
    previousTime = (float)SDL_GetTicks();
    return true;
}

void Game::ProcessInput(){
    // x is clicked
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT) {
            isGameOver = true;
        }
    }
    
    //Get Keyboard Input
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    std::vector<Actor*> actorsCopy = actors;
    for(int i = 0; i < (int)actorsCopy.size(); i++){
        actorsCopy[i] -> ProcessInput(state);
    }
    if (state[SDL_SCANCODE_ESCAPE]) {
        isGameOver = true;
    }
}

void Game::UpdateGame(){
    //Frame Limiting, Delta Time, Max Delta Time
    float currentTime = (float)SDL_GetTicks();
    while(currentTime - previousTime < 16){
        currentTime = (float)SDL_GetTicks();
    }
    float delta = (currentTime - previousTime) / 1000;
    if(delta > 0.033){
        delta = (float)0.033;
    }
    previousTime = currentTime;
    
    //Update Copy Vector
    std::vector<Actor*> actorsCopy = actors;
    for(int i = 0; i < (int)actorsCopy.size(); i++){
        actorsCopy[i] -> Update(delta);
    }
    
    //Move Destroy State Actors to Temp Vector
    std::vector<Actor*> tempActors;
    for(int i = 0; i < (int)actors.size(); i++){
        if(actors[i] -> GetState() == ActorState::Destroy){
            tempActors.push_back(actors[i]);
        }
    }
    
    //Loop Over Temp Vector & Delete All Actors
    for(Actor* it : tempActors){
        delete it;
    }
}

void Game::GenerateOutput(){
    //Background Color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    for(SpriteComponent* s:sprites){
        if(s -> IsVisible()){
            s -> Draw(renderer);
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::RunLoop(){
    while(!isGameOver){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown(){
    IMG_Quit();
    UnloadData();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::AddActor(Actor *actor){
    actors.push_back(actor);
}

void Game::RemoveActor(Actor *actor){
    actors.erase(std::find(actors.begin(), actors.end(), actor));
}

void Game::LoadData(){
    Actor* background = new Actor(this);
    background -> SetPosition(Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
    SpriteComponent* sc = new SpriteComponent(background, 50);
    sc->SetTexture(GetTexture("Assets/Background.png"));
    
    std::string line;
    std::ifstream myfile;
    myfile.open("Assets/Level.txt");
    if(myfile.is_open()){
        float topLeftX = 64.0f;
        float topLeftY = 160.0f;
        int counter = 0;
        while(std::getline(myfile, line)){
            for(int i = 0; i < (int)line.length(); i++){
                if(line[i] == 'X' || line[i] == 'Y' || line[i] == 'Z'){
                    Log* log = new Log(this, line[i]);
                    log->SetPosition(Vector2(topLeftX, topLeftY));
                    WrappingMove* wm = log->GetWrappingMove();
                    if(counter % 2 == 0){
                        wm->SetDirection(Vector2(1.0f, 0.0f));
                    }
                    else{
                        wm->SetDirection(Vector2(-1.0f, 0.0f));
                    }
                }
                else if(line[i] == 'A' || line[i] == 'B' || line[i] == 'C' || line[i] == 'D' || line[i] == 'T'){
                    Vehicle* veh = new Vehicle(this, line[i]);
                    veh->SetPosition(Vector2(topLeftX, topLeftY));
                    WrappingMove* wm = veh->GetWrappingMove();
                    if(counter % 2 == 0){
                        wm->SetDirection(Vector2(1.0f, 0.0f));
                    }
                    else{
                        wm->SetDirection(Vector2(-1.0f, 0.0f));
                    }
                }
                else if(line[i] == 'F'){
                    frog = new Frog(this);
                    frog->SetPosition(Vector2(topLeftX, topLeftY));
                }
                else if(line[i] == 'G'){
                    Actor* act = new Actor(this);
                    goal = new CollisionComponent(act);
                    goal->SetSize(64.0f, 64.0f);
                    act->SetPosition(Vector2(448, 160));
                }
                topLeftX += 64.0f;
            }
            topLeftX = 64.0f;
            topLeftY += 64.0f;
            counter++;
        }
        myfile.close();
    }
    else{
        SDL_Log("Failed to Open Level.txt");
    }
}

void Game::UnloadData(){
    while(!actors.empty()){
        delete actors.back();
    }
    
    std::unordered_map<std::string, SDL_Texture*>::iterator it = hashmap.begin();
    while(it != hashmap.end()){
        SDL_DestroyTexture(it -> second);
        it++;
    }
    hashmap.clear();
}

SDL_Texture* Game::GetTexture(const char *filename){
    if(hashmap.find(filename) != hashmap.end()){
        return hashmap[filename];
    }
    else{
        SDL_Surface *image;
        image = IMG_Load(filename);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
        
        if(image == nullptr) {
            printf("IMG_Load: %s\n", IMG_GetError());
        }
        else{
            hashmap[filename] = texture;
        }
        return texture;
    }
}

void Game::AddSprite(SpriteComponent *sc){
    sprites.push_back(sc);
    std::sort(sprites.begin(), sprites.end(),
        [](SpriteComponent* a, SpriteComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });

}

void Game::RemoveSprite(SpriteComponent *sc){
    sprites.erase(std::find(sprites.begin(), sprites.end(), sc));
}

void Game::AddVehicle(Vehicle *veh){
    cars.push_back(veh);
}

void Game::RemoveVehicle(Vehicle *veh){
    cars.erase(std::find(cars.begin(), cars.end(), veh));
}

void Game::AddLog(Log *log){
    logs.push_back(log);
}

void Game::RemoveLog(Log *log){
    logs.erase(std::find(logs.begin(), logs.end(), log));
}

std::vector<Vehicle*>& Game::GetVehicles(){
    return cars;
}

std::vector<Log*>& Game::GetLogs(){
    return logs;
}

CollisionComponent* Game::GetGoal(){
    return goal;
}

Frog* Game::GetFrog(){
    return frog;
}
