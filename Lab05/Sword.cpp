//
//  Sword.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "Sword.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

Sword::Sword(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Sword/Up.png"));
    cc = new CollisionComponent(this);
    cc->SetSize(25.0f, 25.0f);
}


