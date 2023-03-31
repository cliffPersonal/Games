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

class PlayerMove : public MoveComponent{
public:
    PlayerMove(class Actor* owner);
    
private:
    float mYSpeed;
    bool mSpacePressed;
    bool mInAir;
    bool dead;
    bool win;
    
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void MarioAnimations();
};

#endif /* PlayerMove_h */
