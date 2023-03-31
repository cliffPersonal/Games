//
//  SideBlock.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#include "SideBlock.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "Player.h"

SideBlock::SideBlock(Game* game)
:Actor(game)
{
    SetScale(500.0f);
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mc->SetTextureIndex(1);
}

void SideBlock::OnUpdate(float deltaTime){
    if(GetPosition().x + 2000.0f < GetGame()->GetPlayer()->GetPosition().x){
        SetState(ActorState::Destroy);
    }
}
