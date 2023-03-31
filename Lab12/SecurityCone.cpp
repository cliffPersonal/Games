//
//  SecurityCone.cpp
//  Lab11
//
//  Created by Clifford Lin on 11/17/21.
//

#include "SecurityCone.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMove.h"
#include "SecurityCamera.h"

SecurityCone::SecurityCone(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this, true);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
    
    SetPosition(Vector3(30.0f, 0.0f, 0.0f));
    SetScale(3.0f);
}

void SecurityCone::OnUpdate(float deltaTime){
    Vector3 pos = GetWorldPos();
    Vector3 forward = GetWorldForward();
    Vector3 playerPos = GetGame()->GetPlayer()->GetWorldPos();
    
    Vector3 coneToPlayer = playerPos - pos;
    if(coneToPlayer.Length() <= 300.0f){
        coneToPlayer.Normalize();
        float dot = Vector3::Dot(coneToPlayer, forward);
        float theta = Math::Acos(dot);
        if(theta < Math::PiOver2 / 3){
            mc->SetTextureIndex(1);
            inCone = true;
        }
        else{
            mc->SetTextureIndex(0);
            inCone = false;
        }
    }
    else{
        mc->SetTextureIndex(0);
        inCone = false;
    }
    
    SecurityCamera* sec = static_cast<SecurityCamera*>(mParent);
    sec->SetPause(inCone);
    
    if(inCone){
        coneTimer += deltaTime;
        if(!musicPlaying){
            mSFX = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);
            musicPlaying = true;
        }
    }
    else{
        musicPlaying = false;
        if(mSFX != -1){
            Mix_HaltChannel(mSFX);
            mSFX = -1;
        }
        coneTimer = 0.0f;
    }
    
    if(coneTimer >= 2.0f){
        coneTimer = 0.0f;
        death = true;
    }
    
    if(death){
        mc->SetTextureIndex(2);
        deathTimer += deltaTime;
    }
    
    if(deathTimer >= 0.5f){
        deathTimer = 0.0f;
        death = false;
        GetGame()->GetPlayer()->GetComponent<PlayerMove>()->Respawn();
    }
}
