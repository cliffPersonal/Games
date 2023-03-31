//
//  Fireball.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "Fireball.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Player.h"

Fireball::Fireball(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/FireballRed.png"));
    cc = new CollisionComponent(this);
    cc->SetSize(16.0f, 16.0f);
    mc = new MoveComponent(this);
    mc->SetForwardSpeed(200.0f);
}

void Fireball::OnUpdate(float deltaTime){
    deathTimer += deltaTime;
    if(deathTimer >= 2.0f){
        this->SetState(ActorState::Destroy);
    }
    if(cc->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>())){
        mGame->GetPlayer()->TakeDamage(1);
        this->SetState(ActorState::Destroy);
    }
}
