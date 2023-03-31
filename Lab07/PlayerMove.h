//
//  PlayerMove.h
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#ifndef PlayerMove_h
#define PlayerMove_h

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent{
public:
    PlayerMove(class Actor* owner);
    
private:
    Vector3 mVelocity;
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    const float HDIST = 300.0f;
    const float VDIST = 100.0f;
    const float TARGETDIST = 20.0f;
    float checkPoint = 0.0f;
    int counter = 3;
    bool randomTime = false;
    bool mSpacePressed = false;
    float mutiplier = 1.0f;
    float multiplierTimer = 0.0f;
};

#endif /* PlayerMove_h */
