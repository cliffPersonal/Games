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
    DampenConst = 2.0f * sqrt(SPRINGCONST);
}

void CameraComponent::Update(float deltaTime)
{
    Vector3 displacement = CameraPos - IdealPosition();
    Vector3 SpringAcc = (-SPRINGCONST * displacement) - (DampenConst * CameraVelocity);
    CameraVelocity += SpringAcc * deltaTime;
    CameraPos += CameraVelocity * deltaTime;
    
    Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * TARGETDIST);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(CameraPos, targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

Vector3 CameraComponent::IdealPosition(){
    Vector3 idealPos = mOwner->GetPosition() - (mOwner->GetForward() * HDIST);
    idealPos.z = VDIST;
    return idealPos;
}

void CameraComponent::SnapToIdeal(){
    Vector3 idealPos = mOwner->GetPosition() - (mOwner->GetForward() * HDIST);
    idealPos.z = VDIST;
    CameraPos = idealPos;
    Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * TARGETDIST);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(idealPos, targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}
