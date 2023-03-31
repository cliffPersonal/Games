//
//  Collider.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/27/21.
//

#include "Collider.h"
#include "Game.h"
#include "CollisionComponent.h"

Collider::Collider(Game* game)
:Actor(game)
{
    cc = new CollisionComponent(this);
}

void Collider::SetWH(float w, float h)
{
    cc->SetSize(w, h);
}
