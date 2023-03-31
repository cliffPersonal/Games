//
//  Block.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#include "Block.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

Block::Block(Game* game, char inputLetter)
:Actor(game),
letter(inputLetter)
{
    SetScale(25.0f);
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(1.0f, 1.0f, 1.0f);
    if(letter == 'A'){
        exploding = false;
        mc->SetTextureIndex(2);
    }
    else if(letter == 'B'){
        exploding = true;
        mc->SetTextureIndex(4);
    }
    game -> AddBlock(this);
}

Block::~Block(){
    mGame -> RemoveBlock(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

void Block::OnUpdate(float deltaTime){
    if(GetPosition().x + 2000.0f < GetGame()->GetPlayer()->GetPosition().x){
        SetState(ActorState::Destroy);
    }
}

