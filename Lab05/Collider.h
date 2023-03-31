//
//  Collider.h
//  Lab05
//
//  Created by Clifford Lin on 9/27/21.
//

#ifndef Collider_h
#define Collider_h

#include <stdio.h>
#include "Actor.h"

class Collider: public Actor
{
public:
    Collider(class Game* game);
    void SetWH(float w, float h);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    
private:
    class CollisionComponent* cc;
};

#endif /* Collider_h */
