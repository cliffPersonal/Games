//
//  EnemyComponent.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "EnemyComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Door.h"

EnemyComponent::EnemyComponent(class Actor* owner)
:Component(owner)
{
    owner->GetGame()->AddEnemy(owner);
}

EnemyComponent::EnemyComponent(class Actor* owner, int health)
:Component(owner)
{
    owner->GetGame()->AddEnemy(owner);
    hitPoint = health;
    boss = true;
}

EnemyComponent::~EnemyComponent()
{
    mOwner->GetGame()->RemoveEnemy(mOwner);
}

void EnemyComponent::SetOnDeath(std::function<void()> onDeath)
{
    mOnDeath = onDeath;
}

void EnemyComponent::SetOnTakeDamage(std::function<void()> onDamage){
    mOnTakeDamage = onDamage;
}

void EnemyComponent::TakeDamage(int amount){
    if(!takenDamage){
        hitPoint -= amount;
        if(hitPoint <= 0){
            mOwner->SetState(ActorState::Destroy);
            if(boss){
                Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/BossDie.wav"), 0);
                for(Door* d: mOwner->GetGame()->GetDoors(mOwner->GetGame()->GetCurrRoom())){
                    d->SetState(State::Open);
                }
            }
            else{
                Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/EnemyDie.wav"), 0);
            }
            if (mOnDeath)
            {
                mOnDeath();
            }
        }
        else{
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/BossHit.wav"), 0);
            if (mOnTakeDamage)
            {
               mOnTakeDamage();
            }
        }
        takenDamage = true;
    }
}

void EnemyComponent::Update(float deltaTime){
    if(takenDamage){
        invulnerableTimer -= deltaTime;
        if(invulnerableTimer <= 0.0f){
            invulnerableTimer = 1.0f;
            takenDamage = false;
        }
    }
}
