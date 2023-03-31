//
//  GoombaMove.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#include "GoombaMove.h"
#include "Goomba.h"
#include "Actor.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Game.h"

GoombaMove::GoombaMove(Actor* owner)
:MoveComponent(owner)
{
    this->SetForwardSpeed(-100.0f);
    mYSpeed = 0.0f;
}

void GoombaMove::Update(float deltaTime){
    Vector2 goombaPos;
    goombaPos.x = mOwner->GetPosition().x;
    goombaPos.y = mOwner->GetPosition().y;
    
    goombaPos.x += this->GetForwardSpeed() * deltaTime;
    goombaPos.y += mYSpeed * deltaTime;
    
    if(goombaPos.y >448.0f){
        mOwner->SetState(ActorState::Destroy);
    }
    mOwner->SetPosition(goombaPos);
    
    CollisionComponent* goombacc = mOwner->GetComponent<CollisionComponent>();
    for(Block* block:mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = block->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = goombacc->GetMinOverlap(blockcc, offset);
        if(collision == CollSide::Top && mYSpeed > 0.0f){
            mYSpeed = 0.0f;
        }
        mOwner->SetPosition(mOwner->GetPosition() + offset);
    }
    
    for(Block* block:mOwner->GetGame()->GetBlocks()){
        CollisionComponent* blockcc = block->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = goombacc->GetMinOverlap(blockcc, offset);
        if(this->GetForwardSpeed() < 0.0f && collision == CollSide::Right){
            this->SetForwardSpeed(100.0f);
        }
        if(this->GetForwardSpeed() > 0.0f && collision == CollSide::Left){
            this->SetForwardSpeed(-100.0f);
        }
        mOwner->SetPosition(mOwner->GetPosition() + offset);
    }
    
    for(Goomba* goomba:mOwner->GetGame()->GetGoombas()){
        CollisionComponent* thisGoombacc = goomba->GetCollisionComponent();
        Vector2 offset;
        
        if(goomba == mOwner){
            continue;
        }
        
        CollSide collision = goombacc->GetMinOverlap(thisGoombacc, offset);

        if(this->GetForwardSpeed() < 0.0f && collision == CollSide::Right){
            this->SetForwardSpeed(100.0f);
        }
        if(this->GetForwardSpeed() > 0.0f && collision == CollSide::Left){
            this->SetForwardSpeed(-100.0f);
        }
        mOwner->SetPosition(mOwner->GetPosition() + offset);
    }
    mYSpeed += 2000.0f * deltaTime;
}
