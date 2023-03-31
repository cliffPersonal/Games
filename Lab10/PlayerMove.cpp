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
    mGravity.z = -980.0f;
    mJumpForce.z = 35000.0f;
    mWallClimbForce.z = 1800.0f;
    mWallRunForce.z = 1200.0f;
    mWallRunTimer = 0.0f;
    ChangeState(MoveState::Falling);
}

void PlayerMove::ProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]){
        
    }
    else if(keyState[SDL_SCANCODE_W]){
        AddForce(700.0f * mOwner->GetForward());
    }
    else if(keyState[SDL_SCANCODE_S]){
        AddForce(-700.0f * mOwner->GetForward());
    }
    else{
        
    }
    
    if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]){
        
    }
    else if(keyState[SDL_SCANCODE_D]){
        AddForce(700.0f * mOwner->GetRight());
    }
    else if(keyState[SDL_SCANCODE_A]){
        AddForce(-700.0f * mOwner->GetRight());
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
    float floatX = ((float)x / 500.0f) * Math::Pi * 10.0f;
    SetAngularSpeed(floatX);
    float floatY = ((float)y / 500.0f) * Math::Pi * 10.0f;
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
    else if(mCurrentState == MoveState::WallClimb){
        UpdateWallClimb(deltaTime);
    }
    else if(mCurrentState == MoveState::WallRun){
        UpdateWallRun(deltaTime);
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
        else if(coll == CollSide::SideMinY || coll == CollSide::SideMaxY || coll == CollSide::SideMinX || coll == CollSide::SideMaxX){
            bool canClimb = blockcc->CanWallClimb(coll);
            if(canClimb){
                ChangeState(MoveState::WallClimb);
                mWallClimbTimer = 0.0f;
                return;
            }
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
        
        if(coll == CollSide::SideMinY || coll == CollSide::SideMaxY || coll == CollSide::SideMinX || coll == CollSide::SideMaxX){
            bool canClimb = blockcc->CanWallClimb(coll);
            if(canClimb){
                ChangeState(MoveState::WallClimb);
                mWallClimbTimer = 0.0f;
                return;
            }
            else if(blockcc->CanWallRun(coll)){
                mWallRunColl = coll;
                ChangeState(MoveState::WallRun);
                mWallRunTimer = 0.0f;
                return;
            }
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

void PlayerMove::UpdateWallClimb(float deltaTime){
    mWallClimbTimer += deltaTime;
    AddForce(mGravity);
    if(mWallClimbTimer < 0.4f){
        AddForce(mWallClimbForce);
    }
    PhysicsUpdate(deltaTime);
    
    bool collideSide = false;
    CollisionComponent* playercc = mOwner->GetComponent<CollisionComponent>();
    for(Actor* b : mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = b->GetComponent<CollisionComponent>();
        CollSide coll = FixCollision(playercc, blockcc);
        if(coll == CollSide::SideMinY || coll == CollSide::SideMaxY || coll == CollSide::SideMinX || coll == CollSide::SideMaxX){
            collideSide = true;
        }
    }
    
    if(!collideSide || mVelocity.z <= 0.0f){
        mVelocity.z = 0.0f;
        ChangeState(MoveState::Falling);
    }
}

void PlayerMove::UpdateWallRun(float deltaTime){
    mWallRunTimer += deltaTime;
    AddForce(mGravity);
    if(mWallRunTimer < 0.4f){
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltaTime);
    
    CollisionComponent* playercc = mOwner->GetComponent<CollisionComponent>();
    for(Actor* b : mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = b->GetComponent<CollisionComponent>();
        FixCollision(playercc, blockcc);
    }
    
    if(mVelocity.z <= 0.0f){
        mVelocity.z = 0.0f;
        ChangeState(MoveState::Falling);
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
    
    Vector2 norm = self->GetNormal(coll);
    Vector3 normForce;
    normForce.x = norm.x;
    normForce.y = norm.y;
    normForce *= 700;
    AddForce(normForce);
    
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
    
    if(xyVelocity.Length() > 400.0f){
        xyVelocity.Normalize();
        xyVelocity *= 400.0f;
    }
    
    if(mCurrentState == MoveState::OnGround || mCurrentState == MoveState::WallClimb){
        if(Math::NearZero(mAcceleration.x)){
            xyVelocity.x *= 0.9f;
        }
        if(Math::NearZero(mAcceleration.y)){
            xyVelocity.y *= 0.9f;
        }
        if((mAcceleration.x > 0.0f && xyVelocity.x < 0.0f) || (mAcceleration.x < 0.0f && xyVelocity.x > 0.0f)){
            xyVelocity.x *= 0.9f;
        }
        if((mAcceleration.y > 0.0f && xyVelocity.y < 0.0f) || (mAcceleration.y < 0.0f && xyVelocity.y > 0.0f)){
            xyVelocity.y *= 0.9f;
        }
    }
    mVelocity.x = xyVelocity.x;
    mVelocity.y = xyVelocity.y;
}
