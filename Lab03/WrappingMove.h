//
//  WrappingMove.h
//  Lab03
//
//  Created by Clifford Lin on 9/14/21.
//

#ifndef WrappingMove_h
#define WrappingMove_h

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"

class WrappingMove : public MoveComponent{
public:
    WrappingMove(class Actor* owner);
    void Update(float deltaTime) override;
    void SetDirection(Vector2 vec);
    Vector2 GetDirection();
    
private:
    Vector2 direction;
};
#endif /* WrappingMove_h */
