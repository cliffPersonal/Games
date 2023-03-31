//
//  Checkpoint.cpp
//  Lab11
//
//  Created by Clifford Lin on 11/11/21.
//

#include "Checkpoint.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.h"

Checkpoint::Checkpoint(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(25.0f, 25.0f, 25.0f);
}

void Checkpoint::OnUpdate(float deltaTime){
    Checkpoint* tempCheck = static_cast<Checkpoint*>(GetGame()->GetFront());
    if(this != tempCheck){
        SetInactive();
    }
    else{
        SetActive();
    }
    
    if(active){
        mc->SetTextureIndex(0);
    }
    else{
        mc->SetTextureIndex(1);
    }
}
