//
//  Key.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "Key.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Collectible.h"
#include "Player.h"
#include "Actor.h"

Key::Key(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this, 100);
    sc->SetTexture(this->GetGame()->GetTexture("Assets/Key.png"));
    cc = new CollisionComponent(this);
    cc->SetSize(16.0f, 32.0f);
    Collectible* coll = new Collectible(this);
    coll->SetOnCollect([this] {
        this->GetGame()->GetPlayer()->PickedUpKey();
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Key.wav"), 0);
    });}
