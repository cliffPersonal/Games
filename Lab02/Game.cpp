//
//  Game.cpp
//  Game-mac
//


#include "Game.h"
#include <SDL2/SDL_image.h>
#include "Ship.h"
#include "Random.h"
#include "Asteroid.h"

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
        "Lab2",                            // Title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH,                      // width
        WINDOW_HEIGHT,                     // height
        0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    
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

void Game::AddAsteroid(Asteroid *ast){
    asteroids.push_back(ast);
}

void Game::RemoveAsteroid(Asteroid *ast){
    asteroids.erase(std::find(asteroids.begin(), asteroids.end(), ast));
}

void Game::LoadData(){
    Ship* ship = new Ship(this);
    ship -> SetPosition(Vector2((float(WINDOW_WIDTH))/2, ((float)WINDOW_HEIGHT)/2));
    
    for(int i = 0; i < 10; i++){
        new Asteroid(this);
    }
    
    Actor* stars = new Actor(this);
    stars -> SetPosition(Vector2(512.0f, 384.0f));
    SpriteComponent* sc = new SpriteComponent(stars, 50);
    sc->SetTexture(GetTexture("Assets/Stars.png"));
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
