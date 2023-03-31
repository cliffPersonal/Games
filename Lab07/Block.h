//
//  Block.h
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#ifndef Block_h
#define Block_h

#include <stdio.h>
#include "Actor.h"

class Block : public Actor{
public:
    Block(class Game* game, char inputLetter);
    virtual ~Block();
    bool GetExploding() { return exploding; }
private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
    char letter;
    bool exploding;
    
    void OnUpdate(float deltaTime) override;
};
#endif /* Block_h */
