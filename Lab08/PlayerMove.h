//
//  PlayerMove.h
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#ifndef PlayerMove_h
#define PlayerMove_h

#include <stdio.h>
#include "VehicleMove.h"
#include "Math.h"

class PlayerMove : public VehicleMove{
public:
    PlayerMove(class Actor* owner);
    void OnLapChange(int newLap) override;
private:
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
};

#endif /* PlayerMove_h */
