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
    Matrix4 viewMatrix = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

Vector3 CameraComponent::IdealPosition(){
    Vector3 idealPos = mOwner->GetPosition() - (mOwner->GetForward() * HDIST);
    idealPos.z = VDIST;
    return idealPos;
}
