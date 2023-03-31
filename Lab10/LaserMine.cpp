//
//  LaserMine.cpp
//  Lab10
//
//  Created by Clifford Lin on 11/8/21.
//

#include "LaserMine.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "LaserComponent.h"

LaserMine::LaserMine(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
    new LaserComponent(this);
}

