//
//  PlayerMove.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "CameraComponent.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "Block.h"

PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
{
    mGravity.x = 0.0f;
    mGravity.y = 0.0f;
    mGravity.z = GRAVITYZ;
    mJumpForce.x = 0.0f;
    mJumpForce.y = 0.0f;
    mJumpForce.z = JUMPZ;
    ChangeState(MoveState::Falling);
}

void PlayerMove::ProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]){
        
    }
    else if(keyState[SDL_SCANCODE_W]){
        AddForce(FORCEMULT * mOwner->GetForward());
    }
    else if(keyState[SDL_SCANCODE_S]){
        AddForce(-FORCEMULT * mOwner->GetForward());
    }
    else{
        
    }
    
    if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]){
        
    }
    else if(keyState[SDL_SCANCODE_D]){
        AddForce(FORCEMULT * mOwner->GetRight());
    }
    else if(keyState[SDL_SCANCODE_A]){
        AddForce(-FORCEMULT * mOwner->GetRight());
    }
    else{
        
    }
    
    bool thisFrame = false;
    if(keyState[SDL_SCANCODE_SPACE]){
        thisFrame = true;
        if(thisFrame && !mSpacePressed && mCurrentState == MoveState::OnGround){
            AddForce(mJumpForce);
            ChangeState(MoveState::Jump);
        }
    }
    mSpacePressed = thisFrame;
    
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    float floatX = ((float)x / MOUSEDENOM) * Math::Pi * MOUSEMULT;
    SetAngularSpeed(floatX);
    float floatY = ((float)y / MOUSEDENOM) * Math::Pi * MOUSEMULT;
    mOwner->GetGame()->GetPlayer()->GetComponent<CameraComponent>()->SetPitchSpeed(floatY);
}

void PlayerMove::Update(float deltaTime){
    if(mCurrentState == MoveState::OnGround){
        UpdateOnGround(deltaTime);
    }
    else if(mCurrentState == MoveState::Jump){
        UpdateJump(deltaTime);
    }
    else if(mCurrentState == MoveState::Falling){
        UpdateFalling(deltaTime);
    }
}

void PlayerMove::UpdateOnGround(float deltaTime){
    PhysicsUpdate(deltaTime);
    CollisionComponent* playercc = mOwner->GetComponent<CollisionComponent>();
    bool fall = true;
    for(Actor* b : mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = b->GetComponent<CollisionComponent>();
        CollSide coll = FixCollision(playercc, blockcc);
        if(coll == CollSide::Top){
            fall = false;
        }
    }
    
    if(fall){
        ChangeState(MoveState::Falling);
    }
}

void PlayerMove::UpdateJump(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    
    CollisionComponent* playercc = mOwner->GetComponent<CollisionComponent>();
    for(Actor* b : mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = b->GetComponent<CollisionComponent>();
        CollSide coll = FixCollision(playercc, blockcc);
        if(coll == CollSide::Bottom){
            mVelocity.z = 0.0f;
        }
        if(mVelocity.z <= 0.0f){
            ChangeState(MoveState::Falling);
        }
    }
}

void PlayerMove::UpdateFalling(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    
    CollisionComponent* playercc = mOwner->GetComponent<CollisionComponent>();
    for(Actor* b : mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = b->GetComponent<CollisionComponent>();
        CollSide coll = FixCollision(playercc, blockcc);
        if(coll == CollSide::Top){
            mVelocity.z = 0.0f;
            ChangeState(MoveState::OnGround);
        }
    }
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block){
    Vector3 offset;
    CollSide coll = self->GetMinOverlap(block, offset);
    
    if(coll != CollSide::None){
        Vector3 pos = mOwner->GetPosition();
        pos += offset;
        mOwner->SetPosition(pos);
    }
    
    return coll;
}

void PlayerMove::PhysicsUpdate(float deltaTime){
    mAcceleration = mPendingForces * (1.0f / mMass);
    mVelocity += mAcceleration * deltaTime;
    FixXYVelocity();
    Vector3 pos = mOwner->GetPosition();
    pos += mVelocity * deltaTime;
    mOwner->SetPosition(pos);
    float ang = mOwner->GetRotation();
    ang += GetAngularSpeed() * deltaTime;
    mOwner->SetRotation(ang);
    mPendingForces = Vector3::Zero;
}

void PlayerMove::AddForce(const Vector3& force){
    mPendingForces += force;
}

void PlayerMove::FixXYVelocity(){
    Vector2 xyVelocity;
    xyVelocity.x = mVelocity.x;
    xyVelocity.y = mVelocity.y;
    
    if(xyVelocity.Length() > XYMAXLENGTH){
        xyVelocity.Normalize();
        xyVelocity *= XYMAXLENGTH;
    }
    
    if(mCurrentState == MoveState::OnGround){
        if(Math::NearZero(mAcceleration.x)){
            xyVelocity.x *= XYMULT;
        }
        if(Math::NearZero(mAcceleration.y)){
            xyVelocity.y *= XYMULT;
        }
        if((mAcceleration.x > 0.0f && xyVelocity.x < 0.0f) || (mAcceleration.x < 0.0f && xyVelocity.x > 0.0f)){
            xyVelocity.x *= XYMULT;
        }
        if((mAcceleration.y > 0.0f && xyVelocity.y < 0.0f) || (mAcceleration.y < 0.0f && xyVelocity.y > 0.0f)){
            xyVelocity.y *= XYMULT;
        }
    }
    mVelocity.x = xyVelocity.x;
    mVelocity.y = xyVelocity.y;
}
