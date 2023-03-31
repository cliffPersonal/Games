//
//  SideBlock.h
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#ifndef SideBlock_h
#define SideBlock_h

#include <stdio.h>
#include "Actor.h"

class SideBlock : public Actor{
public:
    SideBlock(class Game* game);
    
private:
    class MeshComponent* mc;
    
    void OnUpdate(float deltaTime) override;
};

#endif /* SideBlock_h */
