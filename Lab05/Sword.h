//
//  Sword.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef Sword_h
#define Sword_h

#include <stdio.h>
#include "Actor.h"

class Sword: public Actor
{
public:
    Sword(class Game* game);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
};

#endif /* Sword_h */
