//
//  DeadFrog.hpp
//  Lab03
//
//  Created by Clifford Lin on 9/14/21.
//

#ifndef DeadFrog_h
#define DeadFrog_h

#include <stdio.h>
#include "Actor.h"

class DeadFrog : public Actor{
public:
    DeadFrog(class Game* game);
    
private:
    class SpriteComponent* sc;
    float deathTimer;
    
    void OnUpdate(float deltaTime) override;
};

#endif /* DeadFrog_h */
