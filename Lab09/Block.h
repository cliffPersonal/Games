//
//  Block.h
//  Lab09
//
//  Created by Clifford Lin on 11/1/21.
//

#ifndef Block_h
#define Block_h

#include <stdio.h>
#include "Actor.h"

class Block : public Actor{
public:
    Block(class Game* game);
    virtual ~Block();
private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
};

#endif /* Block_h */
