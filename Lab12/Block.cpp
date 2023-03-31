//
//  Block.cpp
//  Lab09
//
//  Created by Clifford Lin on 11/1/21.
//

#include "Block.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

Block::Block(Game* game, Actor* parent)
:Actor(game, parent)
{
    SetScale(64.0f);
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(1.0f, 1.0f, 1.0f);
    mGame -> AddBlock(this);
}

Block::~Block(){
    mGame -> RemoveBlock(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

void Block::OnUpdate(float deltaTime){
    if(isRotate){
        float rot = GetRotation();
        rot += deltaTime * (Math::PiOver2 / 2);
        SetRotation(rot);
    }
}
