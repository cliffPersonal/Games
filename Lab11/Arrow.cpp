//
//  Arrow.cpp
//  Lab11
//
//  Created by Clifford Lin on 11/16/21.
//

#include "Arrow.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

Arrow::Arrow(Game* game, Actor* parent)
:Actor(game, parent)
{
    SetScale(0.15f);
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
}

void Arrow::OnUpdate(float deltaTime){
    if(GetGame()->GetFront() == nullptr){
        mQuat = Quaternion::Identity;
    }
    else{
        Vector3 playerToNext = GetGame()->GetFront()->GetPosition() - GetGame()->GetPlayer()->GetPosition();
        playerToNext.Normalize();
        
        float dot = Vector3::Dot(Vector3(1.0f, 0.0f, 0.0f), playerToNext);
        if(dot == 1.0f){
            mQuat = Quaternion::Identity;
        }
        else if(dot == -1.0f){
            mQuat = Quaternion(Vector3::UnitZ, Math::Pi);
        }
        else{
            float theta = Math::Acos(dot);
            Vector3 cross = Vector3::Cross(Vector3(1.0f, 0.0f, 0.0f), playerToNext);
            float crossLength = cross.Length();
            Vector3 axis = cross * (1.0f / crossLength);
            
            mQuat = Quaternion(axis, theta);
        }
    }
    
    Vector3 arrowPos = GetGame()->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f));
    SetPosition(arrowPos);
}
