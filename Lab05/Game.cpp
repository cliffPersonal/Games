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
#include "Player.h"
#include "Spawner.h"
#include "TiledBGComponent.h"
#include "CSVHelper.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"

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
        "Lab5",                            // Title
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
    soundValue = Mix_PlayChannel(-1, this->GetSound("Assets/Sounds/Music.ogg"), -1);
    currRoom = "Room1";
    for(int i = 1; i < 8; i++){
        std::fstream fin;
        std::string fname = "Assets/Dungeon/Room" + std::to_string(i) + ".csv";
        std::string cr = "Room" + std::to_string(i);
        fin.open(fname);
        
        std::string line;
        while(std::getline(fin, line)){
            std::vector<std::string> temp = CSVHelper::Split(line);
            if(temp[0] == "Type"){
                continue;
            }
            else if(temp[0] == "Player"){
                player = new Player(this);
                player->SetPosition(Vector2(std::stof(temp[1]), std::stof(temp[2])));
            }
            else if(temp[0] == "Collider"){
                Collider* c = new Collider(this);
                float w = std::stof(temp[3]);
                float h = std::stof(temp[4]);
                float x = std::stof(temp[1]) + (w / 2.0f);
                float y = std::stof(temp[2]) + (h / 2.0f);
                c->SetPosition(Vector2(x, y));
                c->SetWH(w, h);
                colliders.push_back(c);
            }
            else if(temp[0] == "Door"){
                Door* d = new Door(this);
                float w = std::stof(temp[3]);
                float h = std::stof(temp[4]);
                float x = std::stof(temp[1]) + (w / 2.0f);
                float y = std::stof(temp[2]) + (h / 2.0f);
                d->SetPosition(Vector2(x, y));
                d->SetUp(temp[6], temp[7], temp[5]);
                doorHashmap[cr].push_back(d);
            }
            else if(temp[0] == "SecretBlock"){
                SecretBlock* sb = new SecretBlock(this);
                float w = std::stof(temp[3]);
                float h = std::stof(temp[4]);
                float x = std::stof(temp[1]) + (w / 2.0f);
                float y = std::stof(temp[2]) + (h / 2.0f);
                sb->SetPosition(Vector2(x, y));
                sb->SetOXY(x, y);
                sb->SetDirection(temp[6]);
                secretBlocks.push_back(sb);
            }
            else{
                Spawner* spawn = new Spawner(this);
                spawn->SetType(temp[0]);
                float w = std::stof(temp[3]);
                float h = std::stof(temp[4]);
                float x = std::stof(temp[1]) + (w / 2.0f);
                float y = std::stof(temp[2]) + (h / 2.0f);
                spawn->SetPosition(Vector2(x, y));
                spawnerHashmap[cr].push_back(spawn);
            }
        }
        
    }
    
    Actor* a = new Actor(this);
    TiledBGComponent* tc = new TiledBGComponent(a);
    tc->LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);
    tc->SetTexture(this->GetTexture("Assets/Dungeon/DungeonTiles.png"));
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

void Game::AddEnemy(class Actor *actor){
    actorHashmap[currRoom].push_back(actor);
}

void Game::RemoveEnemy(class Actor *actor){
    std::unordered_map<std::string, std::vector<Actor*>>::iterator ptr;
    for (ptr = actorHashmap.begin(); ptr != actorHashmap.end(); ++ptr){
        std::vector<Actor*>& av = ptr->second;
        auto a = std::find(av.begin(), av.end(), actor);
        if(a != av.end()){
            av.erase(a);
            return;
        }
    }
}

CollisionComponent* Game::GetGoal(){
    return goal;
}

void Game::SetCameraPos(Vector2 value)
{
    cameraPosition.x = value.x;
    cameraPosition.y = value.y;
}

void Game::SetCurrRoom(std::string s)
{
    for(Actor* a:actorHashmap[currRoom]){
        a->SetState(ActorState::Paused);
    }
    for(Actor* a:actorHashmap[s]){
        a->SetState(ActorState::Active);
    }
    currRoom = s;
    if(!roomsEntered[s]){
        roomsEntered[s] = true;
        for(Spawner* sp: spawnerHashmap[s]){
            sp->Trigger();
        }
    }
};
