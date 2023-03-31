//
//  Block.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#include "Block.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Block::Block(Game* game, char inputLetter)
:Actor(game),
letter(inputLetter)
{
    sc = new SpriteComponent(this);
    cc = new CollisionComponent(this);
    if(letter == 'A'){
        sc->SetTexture(game->GetTexture("Assets/BlockA.png"));
    }
    else if(letter == 'B'){
        sc->SetTexture(game->GetTexture("Assets/BlockB.png"));
    }
    else if(letter == 'C'){
        sc->SetTexture(game->GetTexture("Assets/BlockC.png"));
    }
    else if(letter == 'D'){
        sc->SetTexture(game->GetTexture("Assets/BlockD.png"));
    }
    else if(letter == 'E'){
        sc->SetTexture(game->GetTexture("Assets/BlockE.png"));
    }
    else if(letter == 'F'){
        sc->SetTexture(game->GetTexture("Assets/BlockF.png"));
    }
    else if(letter == 'G'){
        sc->SetTexture(game->GetTexture("Assets/BlockG.png"));
    }
    else if(letter == 'H'){
        sc->SetTexture(game->GetTexture("Assets/BlockH.png"));
    }
    else if(letter == 'I'){
        sc->SetTexture(game->GetTexture("Assets/BlockI.png"));
    }
    cc->SetSize(32.0f, 32.0f);
    mGame -> AddBlock(this);
}

Block::~Block(){
    mGame -> RemoveBlock(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}
