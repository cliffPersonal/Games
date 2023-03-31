//
//  DeadFrog.cpp
//  Lab03
//
//  Created by Clifford Lin on 9/14/21.
//

#include "DeadFrog.h"
#include "SpriteComponent.h"
#include "Game.h"

DeadFrog::DeadFrog(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Dead.png"));
    deathTimer = 0.0f;
}

void DeadFrog::OnUpdate(float deltaTime){
    deathTimer += deltaTime;
    if(deathTimer >= 0.5f){
        this->SetState(ActorState::Destroy);
    }
}
