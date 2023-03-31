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
#include "SideBlock.h"
#include "MeshComponent.h"
#include "Random.h"
#include "Bullet.h"
#include "Block.h"
#include "CollisionComponent.h"

PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
{
    mVelocity.Set(400.0f, 0.0f, 0.0f);
    SetForwardSpeed(400.0f);
}

void PlayerMove::ProcessInput(const Uint8 *keyState){
    mVelocity.Set(400.0f, 0.0f, 0.0f);
    bool thisFrame = false;
    if(keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]){
        mVelocity.Set(400.0f, 0.0f, 300.0f);
    }
    if(!keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]){
        mVelocity.Set(400.0f, 0.0f, -300.0f);
    }
    if(keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]){
        mVelocity.Set(400.0f, 0.0f, 0.0f);
    }
    if(keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D]){
        mVelocity.Set(400.0f, -300.0f, 0.0f);
    }
    if(!keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]){
        mVelocity.Set(400.0f, 300.0f, 0.0f);
    }
    if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]){
        mVelocity.Set(400.0f, 0.0f, 0.0f);
    }
    if(keyState[SDL_SCANCODE_SPACE]){
        thisFrame = true;
        if(thisFrame && !mSpacePressed){
            Bullet* b = new Bullet(mOwner->GetGame());
            b->SetPosition(mOwner->GetPosition());
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Shoot.wav"), 0);
        }
    }
    mSpacePressed = thisFrame;
}

void PlayerMove::Update(float deltaTime){
    Vector3 pos = mOwner->GetPosition();
    pos += deltaTime * mVelocity * mutiplier;
    
    if(pos.y <= -195.0f){
        pos.y = -195.0f;
    }
    if(pos.y >= 195.0f){
        pos.y = 195.0f;
    }
    if(pos.z <= -225.0f){
        pos.z = -225.0f;
    }
    if(pos.z >= 225.0f){
        pos.z = 225.0f;
    }
    mOwner->SetPosition(pos);
    
    multiplierTimer += deltaTime;
    if(multiplierTimer >= 10.0f){
        multiplierTimer = 0.0f;
        mutiplier += 0.15f;
        SetForwardSpeed(mutiplier * 400.0f);
    }
    
    for(Block* b: mOwner->GetGame()->GetBlocks()){
        CollisionComponent* bcc = b->GetComponent<CollisionComponent>();
        if(mOwner->GetComponent<CollisionComponent>()->Intersect(bcc)){
            mOwner->SetState(ActorState::Paused);
            Mix_HaltChannel(mOwner->GetGame()->GetSoundValue());
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
        }
    }
    
    float lastX = mOwner->GetGame()->GetLastX();
    if(lastX - pos.x < 2500.0f){
        lastX += 500.0f;
        SideBlock* sb1 = new SideBlock(mOwner->GetGame());
        sb1->SetPosition(Vector3(lastX, -500.0f, 0.0f));
        sb1->GetComponent<MeshComponent>()->SetTextureIndex(mOwner->GetGame()->GetTextureIndex());
        SideBlock* sb2 = new SideBlock(mOwner->GetGame());
        sb2->SetPosition(Vector3(lastX, 500.0f, 0.0f));
        sb2->GetComponent<MeshComponent>()->SetTextureIndex(mOwner->GetGame()->GetTextureIndex());
        mOwner->GetGame()->IncrementTextureIndex();
        mOwner->GetGame()->SetLastX(lastX);
    }
    
    if(pos.x >= checkPoint){
        checkPoint += 1000;
        if(randomTime){
            int randNumber = Random::GetIntRange(1, 20);
            mOwner->GetGame()->LoadBlocks("Assets/Blocks/" + std::to_string(randNumber) + ".txt");
        }
        else{
            mOwner->GetGame()->LoadBlocks("Assets/Blocks/" + std::to_string(counter) + ".txt");
            counter++;
            if(counter > 20){
                randomTime = true;
            }
        }
    }
    
    Vector3 cameraPos = mOwner->GetPosition() - (mOwner->GetForward() * HDIST) + (Vector3::UnitZ * VDIST);
    Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * TARGETDIST);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}
