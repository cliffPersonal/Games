#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
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
    class Enemy* GetEnemy() {return enemy; }
    class HeightMap* GetHeightMap() { return hMap; }
    float GetLastX() { return lastX; }
    void SetLastX(float l) { lastX = l; }
    int GetTextureIndex() { return textureIndex; }
    void IncrementTextureIndex() { textureIndex++; if(textureIndex > 5){ textureIndex = 1;}}
    void LoadBlocks(std::string filename);
    int GetSoundValue() { return soundValue; }
    void SetSoundValue(int sv){ soundValue = sv; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class Renderer* mRenderer = nullptr;
    class Player* player;
    class Enemy* enemy;
    class HeightMap* hMap;
    float lastX;
    float lastBlockX = 1000.0f;
    int textureIndex = 1;
    int soundValue;
    float startTimer = 8.5f;
    bool started = false;
    
    const float WIDTH = 1024.0f;
    const float HEIGHT = 768.0f;
    const float FIRSTEDGE = 3000.0f;
    const float SIDEBLOCKADD = 500.0f;
    const float YZCHANGE = 25.0f;
    const float LEFTYZ = 237.5f;
    const float XCHANGE = 1000.0f;
    const float FOV = 1.22f;
    const float NEARPLANEDIST = 10.0f;
    const float FARPLANEDIST = 10000.0f;
    const float EYEX = -300.0f;
    const float EYEZ = 100.0f;
    const float TARGETX = 20.0f;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
};
