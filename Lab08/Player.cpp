//
//  Player.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/19/21.
//

#include "Player.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "PlayerUI.h"

Player::Player(Game* game)
:Actor(game)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    pm = new PlayerMove(this);
    SetScale(0.75f);
    cc = new CameraComponent(this);
    cc->SnapToIdeal();
    pui = new PlayerUI(this);
}
