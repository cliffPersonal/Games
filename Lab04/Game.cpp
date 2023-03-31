//
//  Game.cpp
//  Game-mac
//

#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include "Random.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Block.h"
#include "Player.h"
#include "Goomba.h"
#include "Spawner.h"

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
        WINDOW_WIDTH,                      // width
        WINDOW_HEIGHT,                     // height
        0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    //SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    int flags=IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if((initted&flags) != flags) {
        printf("IMG_Init: Failed to init required png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
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
    Mix_CloseAudio();
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
    background -> SetPosition(Vector2(3392, 224));
    SpriteComponent* sc = new SpriteComponent(background, 50);
    sc->SetTexture(GetTexture("Assets/Background.png"));
    
    soundValue = Mix_PlayChannel(-1, this->GetSound("Assets/Sounds/Music.ogg"), -1);
    
    std::string line;
    std::ifstream myfile;
    myfile.open("Assets/Level1.txt");
    if(myfile.is_open()){
        float topLeftX = 16.0f;
        float topLeftY = 16.0f;
        int counter = 0;
        while(std::getline(myfile, line)){
            for(int i = 0; i < (int)line.length(); i++){
                if(line[i] == 'A' || line[i] == 'B' || line[i] == 'C' || line[i] == 'D' || line[i] == 'E' || line[i] == 'F' || line[i] == 'G' || line[i] == 'H' || line[i] == 'I'){
                    Block* b = new Block(this, line[i]);
                    b->SetPosition(Vector2(topLeftX, topLeftY));
                }
                else if(line[i] == 'P'){
                    player = new Player(this, line[i]);
                    player->SetPosition(Vector2(topLeftX, topLeftY));
                }
                else if(line[i] == 'Y'){
                    Spawner* s = new Spawner(this);
                    s->SetPosition(Vector2(topLeftX, topLeftY));
                }
                topLeftX += 32.0f;
            }
            topLeftX = 16.0f;
            topLeftY += 32.0f;
            counter++;
        }
        myfile.close();
    }
    else{
        SDL_Log("Failed to Open Level1.txt");
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
    
    std::unordered_map<std::string, Mix_Chunk*>::iterator it2 = soundHashmap.begin();
    while(it2 != soundHashmap.end()){
        Mix_FreeChunk(it2 -> second);
        it2 -> second = NULL;
        it2++;
    }
    soundHashmap.clear();
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

Mix_Chunk* Game::GetSound(const std::string& filename){
    if(soundHashmap.find(filename) != soundHashmap.end()){
        return soundHashmap[filename];
    }
    else{
        Mix_Chunk *mc;
        mc = Mix_LoadWAV(filename.c_str());
        if(!mc) {
            printf("Mix_LoadWAV: %s\n", Mix_GetError());
        }
        else{
            soundHashmap[filename] = mc;
        }
        
        return mc;
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

void Game::AddBlock(class Block *b){
    blocks.push_back(b);
}

void Game::RemoveBlock(Block *b){
    blocks.erase(std::find(blocks.begin(), blocks.end(), b));
}

void Game::AddGoomba(class Goomba *g){
    goombas.push_back(g);
}

void Game::RemoveGoomba(Goomba *g){
    goombas.erase(std::find(goombas.begin(), goombas.end(), g));
}

CollisionComponent* Game::GetGoal(){
    return goal;
}

void Game::SetCameraX(float value)
{
    if(cameraPosition.x <= value){
        cameraPosition.x = value;
    }
    if(cameraPosition.x < 0.0f){
        cameraPosition.x = 0.0f;
    }
}

