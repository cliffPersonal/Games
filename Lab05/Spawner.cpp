//
//  Spawner.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#include "Spawner.h"
#include "Game.h"
#include "Player.h"
#include "Key.h"
#include "Skeleton.h"
#include "Boss.h"

Spawner::Spawner(Game* game)
:Actor(game)
{
    
}

void Spawner::OnUpdate(float deltaTime){
    
}

void Spawner::Trigger(){
    if(type == "Key"){
        Key* k = new Key(mGame);
        k->SetPosition(this->GetPosition());
    }
    else if(type == "Skeleton"){
        Skeleton* s = new Skeleton(mGame);
        s->SetPosition(this->GetPosition());
    }
    else if(type == "Boss"){
        Boss* b = new Boss(mGame);
        b->SetPosition(this->GetPosition());
    }
}
