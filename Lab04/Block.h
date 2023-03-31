//
//  Block.h
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#ifndef Block_h
#define Block_h

#include <stdio.h>
#include "Actor.h"

class Block : public Actor{
public:
    Block(class Game* game, char inputLetter);
    virtual ~Block();
    class CollisionComponent* GetCollisionComponent(){return cc;};
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
    char letter;
};

#endif /* Block_h */
