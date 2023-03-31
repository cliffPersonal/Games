//
//  Log.cpp
//  Lab03
//
//  Created by Clifford Lin on 9/9/21.
//

#include "Log.h"
#include "SpriteComponent.h"
#include "WrappingMove.h"
#include "Game.h"
#include "CollisionComponent.h"

Log::Log(Game* game, char inputLetter)
:Actor(game),
letter(inputLetter)
{
    sc = new SpriteComponent(this);
    cc = new CollisionComponent(this);
    if(letter == 'X'){
        sc->SetTexture(game->GetTexture("Assets/LogX.png"));
        cc->SetSize(192.0f, 48.0f);
    }
    else if(letter == 'Y'){
        sc->SetTexture(game->GetTexture("Assets/LogY.png"));
        cc->SetSize(256.0f, 48.0f);
    }
    else if(letter == 'Z'){
        sc->SetTexture(game->GetTexture("Assets/LogZ.png"));
        cc->SetSize(384.0f, 48.0f);
    }
    wm = new WrappingMove(this);
    wm->SetForwardSpeed(75.0f);
    mGame -> AddLog(this);
}

Log::~Log(){
    mGame -> RemoveLog(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

WrappingMove* Log::GetWrappingMove(){
    return wm;
}

