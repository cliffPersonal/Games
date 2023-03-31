//
//  PlayerMove.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Random.h"
#include "Block.h"
#include "CollisionComponent.h"
#include "HeightMap.h"
#include "PlayerUI.h"
#include "Enemy.h"

PlayerMove::PlayerMove(Actor* owner)
:VehicleMove(owner)
{
    mOwner->SetPosition(mOwner->GetGame()->GetHeightMap()->CellToWorld(39, 58));
}

void PlayerMove::ProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_W]){
        SetPedalPressed(true);
    }
    else{
        SetPedalPressed(false);
    }
    if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]){
        SetTurn(Turn::None);
    }
    else if(keyState[SDL_SCANCODE_A]){
        SetTurn(Turn::Left);
    }
    else if(keyState[SDL_SCANCODE_D]){
        SetTurn(Turn::Right);
    }
    else{
        SetTurn(Turn::None);
    }
}

void PlayerMove::Update(float deltaTime){
    VehicleMove::Update(deltaTime);
}

void PlayerMove::OnLapChange(int newLap){
    if(newLap == 5){
        Mix_FadeOutChannel(mOwner->GetGame()->GetSoundValue(), 250);
        int playerLap = mOwner->GetGame()->GetPlayer()->GetComponent<VehicleMove>()->GetCurrentLap();
        int enemyLap = mOwner->GetGame()->GetEnemy()->GetComponent<VehicleMove>()->GetCurrentLap();
        if(playerLap > enemyLap){
            mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::Won);
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Won.wav"), 0);
        }
        else{
            mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::Lost);
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Lost.wav"), 0);
        }
        mOwner->GetGame()->GetPlayer()->SetState(ActorState::Paused);
        mOwner->GetGame()->GetEnemy()->SetState(ActorState::Paused);
    }
    else if(newLap == 4){
        Mix_FadeOutChannel(mOwner->GetGame()->GetSoundValue(), 250);
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/FinalLap.wav"), 0);
        int sv = Mix_FadeInChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/MusicFast.ogg"), -1, 4000);
        mOwner->GetGame()->SetSoundValue(sv);
        mOwner->GetComponent<PlayerUI>()->OnLapChange(newLap);
    }
    else{
        mOwner->GetComponent<PlayerUI>()->OnLapChange(newLap);
    }
}
