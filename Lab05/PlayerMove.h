//
//  PlayerMove.h
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#ifndef PlayerMove_h
#define PlayerMove_h

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"
#include "Door.h"

class PlayerMove : public MoveComponent{
public:
    PlayerMove(class Actor* owner);
    
private:
    float mYSpeed;
    Vector2 directionVector;
    Direction direction;
    class Sword* sword;
    float timer = 0.0f;
    bool attacking = false;
    bool mSpacePressed = false;
    
    void UpdateSword();
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
};

#endif /* PlayerMove_h */
