//
//  WrappingMove.cpp
//  Lab03
//
//  Created by Clifford Lin on 9/14/21.
//

#include "WrappingMove.h"
#include "Actor.h"

WrappingMove::WrappingMove(Actor* owner)
:MoveComponent(owner)
{
    
}

void WrappingMove::Update(float deltaTime){
    Vector2 currPos = mOwner->GetPosition();
    Vector2 newPos = currPos + (direction * this->GetForwardSpeed() * deltaTime);
    if(newPos.x < 0){
        newPos.x = 896;
    }
    else if(newPos.x > 896){
        newPos.x = 0;
    }
    mOwner->SetPosition(newPos);
}

void WrappingMove::SetDirection(Vector2 vec){
    direction = vec;
}

Vector2 WrappingMove::GetDirection(){
    return direction;
}
