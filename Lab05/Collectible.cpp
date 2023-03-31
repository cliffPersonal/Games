//
//  Collectible.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "Collectible.h"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"

Collectible::Collectible(class Actor* owner)
:Component(owner)
{
    
}

void Collectible::SetOnCollect(std::function<void()> onCollect)
{
    mOnCollect = onCollect;
}

void Collectible::Update(float deltaTime)
{
    CollisionComponent* occ = mOwner->GetComponent<CollisionComponent>();
    CollisionComponent* pcc = mOwner->GetGame()->GetPlayer()->GetCollisionComponent();
    
    if(occ->Intersect(pcc)){
        mOwner->SetState(ActorState::Destroy);
        if (mOnCollect)
        {
           mOnCollect();
        }
    }
}
