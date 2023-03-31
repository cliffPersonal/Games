//
//  Skeleton.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef Skeleton_h
#define Skeleton_h

#include <stdio.h>
#include "Actor.h"

class Skeleton: public Actor
{
public:
    Skeleton(class Game* game);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    
private:
    class AnimatedSprite* asc;
    class CollisionComponent* cc;
    class EnemyComponent* ec;
    void OnUpdate(float deltaTime) override;
    float fireTimer;
};

#endif /* Skeleton_h */
