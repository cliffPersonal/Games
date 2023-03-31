//
//  Fireball.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef Fireball_h
#define Fireball_h

#include <stdio.h>
#include "Actor.h"

class Fireball: public Actor
{
public:
    Fireball(class Game* game);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
    class MoveComponent* mc;
    void OnUpdate(float deltaTime) override;
    float deathTimer = 0.0f;
};

#endif /* Fireball_h */
