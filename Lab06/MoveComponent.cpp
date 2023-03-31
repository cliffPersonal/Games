#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
    float rot = mOwner->GetRotation();
    rot += mAngularSpeed * deltaTime;
    mOwner->SetRotation(rot);
    
    Vector2 pos = mOwner->GetPosition();
    Vector2 forw = mOwner->GetForward();
    pos += forw * mForwardSpeed * deltaTime;
    mOwner->SetPosition(pos);
}
