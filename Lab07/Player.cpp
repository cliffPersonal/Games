//
//  Player.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/19/21.
//

#include "Player.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "PlayerMove.h"

Player::Player(Game* game)
:Actor(game)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(100.0f, 40.0f, 60.0f);
    pm = new PlayerMove(this);
}
