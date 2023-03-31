#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }
    class Player* GetPlayer() { return player; }
    int GetSoundValue() { return soundValue; }
    void SetSoundValue(int sv){ soundValue = sv; }
    
    void AddBlock(class Actor *b);
    void RemoveBlock(class Actor *b);
    std::vector<class Actor*>& GetBlocks() { return mBlocks; };
    void SetPlayer(class Player *p){ player = p; };
    void AddCamera(class SecurityCamera* sc);
    void RemoveCamera(class SecurityCamera* sc);
    std::vector<class SecurityCamera*>& GetCameras(){ return mCameras;};
    
    void AddCheckpoint(class Actor* c);
    void RemoveCheckpoint();
    
    Actor* GetFront();
    
    void SetNextLevel(std::string s){ mNextLevel = s; };
    void LoadNextLevel();
    int IncrementCoin(){ coinCount++; return coinCount;};
    int GetCoin(){ return coinCount;};
    float GetTimer(){ return time;};
    void IncrementTimer(float timer){ time += timer; };
    class Texture* GetTexture(){ return mCheckpointText; };
    void SetTexture(Texture* tx){ mCheckpointText = tx; };
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;

    std::vector<class Actor*> mBlocks;
	class Renderer* mRenderer = nullptr;
    class Player* player;
    int soundValue;
    std::string mNextLevel;
    
    std::queue<class Actor*> mCheckpoints;
    
    const float WIDTH = 1024.0f;
    const float HEIGHT = 768.0f;
    const float FOV = 1.22f;
    const float NEARPLANEDIST = 10.0f;
    const float FARPLANEDIST = 10000.0f;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
    
    float time = 0.0f;
    int coinCount = 0;
    class Texture* mCheckpointText = NULL;
    
    std::vector<class SecurityCamera*> mCameras;
};
