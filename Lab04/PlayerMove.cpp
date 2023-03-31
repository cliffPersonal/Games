//
//  PlayerMove.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "Block.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Goomba.h"
#include "AnimatedSprite.h"
#include "SDL2/SDL_mixer.h"

PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
{
    mYSpeed = 0.0f;
    mSpacePressed = false;
    mInAir = false;
    dead = false;
    win = false;
}

void PlayerMove::ProcessInput(const Uint8 *keyState){
    bool thisFrame = false;
    
    if(keyState[SDL_SCANCODE_LEFT]){
        this->SetForwardSpeed(-300.0f);
    }
    else if(keyState[SDL_SCANCODE_RIGHT]){
        this->SetForwardSpeed(300.0f);
    }
    else{
        this->SetForwardSpeed(0.0f);
    }
    if(keyState[SDL_SCANCODE_SPACE]){
        thisFrame = true;
        if(thisFrame && !mSpacePressed && !mInAir){
            mInAir = true;
            mYSpeed = -700.0f;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
        }
    }
    mSpacePressed = thisFrame;
}

void PlayerMove::MarioAnimations(){
    AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
    if(!mInAir){
        if(this->GetForwardSpeed() < 0.0f){
            asc->SetAnimation("runLeft");
        }
        else if(this->GetForwardSpeed() > 0.0f){
            asc->SetAnimation("runRight");
        }
        else{
            asc->SetAnimation("idle");
        }
    }
    else{
        if(this->GetForwardSpeed() < 0.0f){
            asc->SetAnimation("jumpLeft");
        }
        else if(this->GetForwardSpeed() > 0.0f){
            asc->SetAnimation("jumpRight");
        }
        else{
            if(asc->GetAnimName() == "runRight" || asc->GetAnimName() == "jumpRight" || asc->GetAnimName() == "idle"){
                asc->SetAnimation("jumpRight");
            }
            else{
                asc->SetAnimation("jumpLeft");
            }
        }
    }
    
    if(dead){
        mOwner->GetComponent<AnimatedSprite>()->SetAnimation("dead");
        mOwner->GetGame()->GetPlayer()->SetState(ActorState::Paused);
        Mix_HaltChannel(mOwner->GetGame()->GetSoundValue());
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
    }
    
    if(win){
        Mix_HaltChannel(mOwner->GetGame()->GetSoundValue());
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
        mOwner->GetGame()->GetPlayer()->SetState(ActorState::Paused);
    }
}

void PlayerMove::Update(float deltaTime){
    Vector2 pos;
    pos.x = mOwner->GetPosition().x;
    pos.y = mOwner->GetPosition().y;
    
    pos.x += this->GetForwardSpeed() * deltaTime;
    pos.y += mYSpeed * deltaTime;
    
    if(pos.y > 460.0f){
        dead = true;
    }
    mOwner->SetPosition(pos);
    
    CollisionComponent* playercc = mOwner->GetGame()->GetPlayer()->GetCollisionComponent();
    
    bool collided = false;
    for(Block* block:mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = block->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = playercc->GetMinOverlap(blockcc, offset);
        if(collision == CollSide::Top && mYSpeed > 0.0f){
            mYSpeed = 0.0f;
            mInAir = false;
        }
        else if(collision == CollSide::Bottom && mYSpeed < 0.0f){
            mYSpeed = 0.0f;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
        }
        if(collision != CollSide::None){
            collided = true;
        }
        mOwner->SetPosition(mOwner->GetPosition() + offset);
    }
    
    if(!collided){
        mInAir = true;
    }
    
    for(Goomba* goomba:mOwner->GetGame()->GetGoombas()){
        CollisionComponent* thisGoombacc = goomba->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = playercc->GetMinOverlap(thisGoombacc, offset);
        if(collision == CollSide::Top || (mInAir && (collision == CollSide::Left || collision == CollSide::Right))){
            goomba->SetStomped();
            mYSpeed = -350.0f;
            mInAir = true;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
        }
        else if(collision != CollSide::None){
            dead = true;
        }
    }
    
    mYSpeed += 2000.0f * deltaTime;
    
    float cameraX = mOwner->GetGame()->GetCameraPosition().x;
    if(mOwner->GetPosition().x < cameraX){
        mOwner->SetPosition(Vector2(cameraX, mOwner->GetPosition().y));
    }
    
    mOwner->GetGame()->SetCameraX(mOwner->GetPosition().x - 300.0f);
    
    if(mOwner->GetPosition().x >= 6368.0f){
        win = true;
    }
    this->MarioAnimations();
}
