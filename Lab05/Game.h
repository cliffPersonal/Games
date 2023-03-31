#pragma once

#include "Math.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game{
public:
    Game();
    bool Initialize();
    void Shutdown();
    void RunLoop();
    void AddActor(class Actor *actor);
    void RemoveActor(class Actor *actor);
    void AddSprite(class SpriteComponent *sc);
    void RemoveSprite(class SpriteComponent *sc);
    void AddEnemy(class Actor *actor);
    void RemoveEnemy(class Actor *actor);
    void SetCameraPos(Vector2 value);
    void SetCurrRoom(std::string s);
    int GetSoundValue(){return soundValue;};
    class CollisionComponent* GetGoal();
    class Player* GetPlayer(){return player;};
    std::vector<class Collider*>& GetColliders(){return colliders;};
    std::vector<class Door*>& GetDoors(std::string s){return doorHashmap[s];};
    std::vector<class SecretBlock*>& GetSecretBlocks(){return secretBlocks;};
    std::vector<class Actor*>& GetEnemies(std::string s){return actorHashmap[s];};
    std::string GetCurrRoom(){return currRoom;};
    Vector2 GetCameraPosition(){return cameraPosition;};
    SDL_Texture* GetTexture(const char *filename);
    Mix_Chunk* GetSound(const std::string& filename);
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isGameOver;
    int soundValue;
    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 448;
    float previousTime;
    std::vector<class Actor*> actors;
    std::unordered_map<std::string, SDL_Texture*> hashmap;
    std::unordered_map<std::string, Mix_Chunk*> soundHashmap;
    std::unordered_map<std::string, std::vector<class Door*>> doorHashmap;
    std::unordered_map<std::string, std::vector<class Spawner*>> spawnerHashmap;
    std::unordered_map<std::string, std::vector<class Actor*>> actorHashmap;
    std::unordered_map<std::string, bool> roomsEntered = {{"Room1", true}, {"Room2", false}, {"Room3", false}, {"Room4", false}, {"Room5", false}, {"Room6", false}, {"Room7", false},};
    std::vector<class SpriteComponent*> sprites;
    std::vector<class Collider*> colliders;
    std::vector<class SecretBlock*> secretBlocks;
    std::string currRoom;
    class CollisionComponent* goal;
    class Player* player;
    Vector2 cameraPosition;
    
    void LoadData();
    void UnloadData();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
};
