//
//  Goomba.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#include "Goomba.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"
#include "Game.h"

Goomba::Goomba(Game* game)
:Actor(game)
{
    asc = new AnimatedSprite(this, 150);
    cc = new CollisionComponent(this);
    
    std::vector<SDL_Texture*> walkAnim{
           GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
           GetGame()->GetTexture("Assets/Goomba/Walk1.png")
    };
    asc->AddAnimation("walk", walkAnim);
    
    std::vector<SDL_Texture*> dead{
        GetGame()->GetTexture("Assets/Goomba/Dead.png")
    };
    asc->AddAnimation("dead", dead);
    
    cc->SetSize(32.0f, 32.0f);
    asc->SetAnimation("walk");
    gm = new GoombaMove(this);
    
    stomped = false;
    deathTimer = 0.0f;
    game->AddGoomba(this);
}

Goomba::~Goomba(){
    mGame -> RemoveGoomba(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

void Goomba::OnUpdate(float deltaTime){
    if(stomped){
        asc->SetAnimation("dead");
        gm->SetForwardSpeed(0.0f);
        deathTimer += deltaTime;
        if(deathTimer >= 0.25f){
            this->SetState(ActorState::Destroy);
        }
    }
}
