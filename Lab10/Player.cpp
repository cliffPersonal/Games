//
//  Player.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/19/21.
//

#include "Player.h"
#include "Game.h"
#include "Actor.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"

Player::Player(Game* game, Actor* parent)
:Actor(game, parent)
{
    pm = new PlayerMove(this);
    cc = new CameraComponent(this);
    collc = new CollisionComponent(this);
    collc->SetSize(50.0f, 175.0f, 50.0f);
}
