//
//  Game.cpp
//  Game-mac
//


#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.h"
#include "SideBlock.h"
#include "MeshComponent.h"
#include "Block.h"

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
    mRenderer = new Renderer(this);
    bool rendered = mRenderer->Initialize(WIDTH, HEIGHT);
    if(!rendered){
        return false;
    }

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
    mRenderer->Draw();
}

void Game::LoadData()
{
    soundValue = Mix_PlayChannel(-1, this->GetSound("Assets/Sounds/ShipLoop.wav"), -1);
    player = new Player(this);
    Matrix4 projectionMatrix = Matrix4::CreatePerspectiveFOV(FOV, WIDTH, HEIGHT, NEARPLANEDIST, FARPLANEDIST);
    mRenderer->SetProjectionMatrix(projectionMatrix);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(Vector3(EYEX, 0.0f, EYEZ), Vector3(TARGETX, 0.0f, 0.0f), Vector3::UnitZ);
    mRenderer->SetViewMatrix(viewMatrix);
    for(float i = 0.0f; i < FIRSTEDGE; i += SIDEBLOCKADD){
        SideBlock* sb1 = new SideBlock(this);
        sb1->SetPosition(Vector3(i, -SIDEBLOCKADD, 0.0f));
        sb1->GetComponent<MeshComponent>()->SetTextureIndex(textureIndex);
        SideBlock* sb2 = new SideBlock(this);
        sb2->SetPosition(Vector3(i, SIDEBLOCKADD, 0.0f));
        sb2->GetComponent<MeshComponent>()->SetTextureIndex(textureIndex);
        IncrementTextureIndex();
        lastX = i;
    }
    LoadBlocks("Assets/Blocks/1.txt");
    LoadBlocks("Assets/Blocks/2.txt");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
    mRenderer->Shutdown();
    delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

void Game::AddBlock(class Block *b){
    blocks.push_back(b);
}

void Game::RemoveBlock(Block *b){
    blocks.erase(std::find(blocks.begin(), blocks.end(), b));
}

void Game::LoadBlocks(std::string filename){
    std::string line;
    std::ifstream myfile;
    myfile.open(filename);
    if(myfile.is_open()){
        float topLeftY = -LEFTYZ;
        float topLeftZ = LEFTYZ;
        while(std::getline(myfile, line)){
            for(int i = 0; i < (int)line.length(); i++){
                if(line[i] == 'A' || line[i] == 'B'){
                    Block* b = new Block(this, line[i]);
                    b->SetPosition(Vector3(lastBlockX, topLeftY, topLeftZ));
                }
                topLeftY += YZCHANGE;
            }
            topLeftY = -LEFTYZ;
            topLeftZ -= YZCHANGE;
        }
        myfile.close();
    }
    else{
        SDL_Log("Failed to Open");
    }
    lastBlockX += XCHANGE;
}
