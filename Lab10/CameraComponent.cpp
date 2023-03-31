//
//  CameraComponent.cpp
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"

CameraComponent::CameraComponent(class Actor* owner)
:Component(owner)
{
    
}

void CameraComponent::Update(float deltaTime)
{
    mPitchAngle += mPitchSpeed * deltaTime;
    if(mPitchAngle > Math::PiOver2 / 2){
        mPitchAngle = Math::PiOver2 / 2;
    }
    else if(mPitchAngle < -Math::PiOver2 / 2){
        mPitchAngle = -Math::PiOver2 / 2;
    }
    Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
    Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 product = pitch * yaw;
    Vector3 transformed = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), product);
    
    Vector3 targetPos = mOwner->GetPosition() + (transformed * TARGETDIST);
    //
    if(mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>()->GetCurrentState() == PlayerMove::MoveState::WallRun){
        CollSide s = mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>()->GetWallRunColl();
        if(s == CollSide::SideMinY){
            sideNormal.x = 0.0f;
            sideNormal.y = -1.0f;
        }
        else if(s == CollSide::SideMaxY){
            sideNormal.x = 0.0f;
            sideNormal.y = 1.0f;
        }
        else if(s == CollSide::SideMinX){
            sideNormal.x = -1.0f;
            sideNormal.y = 0.0f;
        }
        else if(s == CollSide::SideMaxX){
            sideNormal.x = 1.0f;
            sideNormal.y = 0.0f;
        }
        
        zVector.x += sideNormal.x * deltaTime;
        zVector.y += sideNormal.y * deltaTime;
        if(zVector.x >= 0.5f){
            zVector.x = 0.5f;
        }
        if(zVector.x <= -0.5f){
            zVector.x = -0.5f;
        }
        if(zVector.y >= 0.5f){
            zVector.y = 0.5f;
        }
        if(zVector.y <= -0.5f){
            zVector.y = -0.5f;
        }
    }
    else if((mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>()->GetCurrentState() == PlayerMove::MoveState::Falling || mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>()->GetCurrentState() == PlayerMove::MoveState::OnGround) && (zVector.x != 0.0f || zVector.y != 0.0f)){
        zVector.x -= sideNormal.x * deltaTime;
        zVector.y -= sideNormal.y * deltaTime;
        if(zVector.x < 0.01f && zVector.x > -0.01f){
            zVector.x = 0.0f;
        }
        if(zVector.y < 0.01f && zVector.y > -0.01f){
            zVector.y = 0.0f;
        }

    }
    //
    Matrix4 viewMatrix = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, zVector);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

Vector3 CameraComponent::IdealPosition(){
    Vector3 idealPos = mOwner->GetPosition() - (mOwner->GetForward() * HDIST);
    idealPos.z = VDIST;
    return idealPos;
}
