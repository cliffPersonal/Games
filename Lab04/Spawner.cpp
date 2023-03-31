//
//  Spawner.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#include "Spawner.h"
#include "Goomba.h"
#include "Game.h"
#include "Player.h"

Spawner::Spawner(Game* game)
:Actor(game)
{
    
}

void Spawner::OnUpdate(float deltaTime){
    if(this->GetPosition().x - mGame->GetPlayer()->GetPosition().x < 600.0f){
        Goomba *g = new Goomba(mGame);
        g->SetPosition(this->GetPosition());
        this->SetState(ActorState::Destroy);
    }
}
