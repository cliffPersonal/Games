//
//  GoombaMove.h
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#ifndef GoombaMove_h
#define GoombaMove_h

#include <stdio.h>
#include "MoveComponent.h"

class GoombaMove : public MoveComponent{
public:
    GoombaMove(class Actor* owner);
    
private:
    float mYSpeed = 0.0f;
    
    void Update(float deltaTime) override;
};

#endif /* GoombaMove_h */
