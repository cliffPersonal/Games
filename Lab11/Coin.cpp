//
//  Coin.cpp
//  Lab11
//
//  Created by Clifford Lin on 11/16/21.
//

#include "Coin.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

Coin::Coin(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(100.0f, 100.0f, 100.0f);
}

void Coin::OnUpdate(float deltaTime){
    angle += deltaTime * Math::Pi;
    if(angle > Math::TwoPi){
        angle = 0.0f;
    }
    mQuat = Quaternion(Vector3::UnitZ, angle);
    
    if(cc->Intersect(GetGame()->GetPlayer()->GetComponent<CollisionComponent>())){
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Coin.wav"), 0);
        SetState(ActorState::Destroy);
    }
}


