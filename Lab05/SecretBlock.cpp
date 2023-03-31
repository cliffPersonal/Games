//
//  SecretBlock.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/27/21.
//

#include "SecretBlock.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

SecretBlock::SecretBlock(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this, 100);
    sc->SetTexture(this->GetGame()->GetTexture("Assets/SecretBlock.png"));
    cc = new CollisionComponent(this);
    cc->SetSize(32.0f, 32.0f);
}

void SecretBlock::SetDirection(std::string s){
    if (s == "Up") {
        dir = Direction::Up;
    }
    else if(s == "Down"){
        dir = Direction::Down;
    }
    else if(s == "Left"){
        dir = Direction::Left;
    }
    else if(s == "Right"){
        dir = Direction::Right;
    }
}
