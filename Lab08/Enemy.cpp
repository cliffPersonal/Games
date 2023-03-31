//
//  Enemy.cpp
//  Lab08
//
//  Created by Clifford Lin on 10/27/21.
//

#include "Enemy.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "EnemyMove.h"

Enemy::Enemy(Game* game)
:Actor(game)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    mc->SetTextureIndex(6);
    em = new EnemyMove(this);
    SetScale(0.75f);
}
