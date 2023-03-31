//
//  Vehicle.h
//  Lab03
//
//  Created by Clifford Lin on 9/9/21.
//

#ifndef Vehicle_h
#define Vehicle_h

#include <stdio.h>
#include "Actor.h"

class Vehicle : public Actor{
public:
    Vehicle(class Game* game, char inputLetter);
    virtual ~Vehicle();
    class WrappingMove* GetWrappingMove();
    class CollisionComponent* GetCollisionComp();
    
private:
    class SpriteComponent* sc;
    char letter;
    class WrappingMove* wm;
    class CollisionComponent* cc;
    
    void OnUpdate(float deltaTime) override;
};

#endif /* Vehicle_h */
