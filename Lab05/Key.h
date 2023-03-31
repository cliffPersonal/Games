//
//  Key.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef Key_h
#define Key_h

#include <stdio.h>
#include "Actor.h"

class Key : public Actor{
public:
    Key(class Game* game);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
};

#endif /* Key_h */
