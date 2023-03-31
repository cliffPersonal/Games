//
//  LaserMine.h
//  Lab10
//
//  Created by Clifford Lin on 11/8/21.
//

#ifndef LaserMine_h
#define LaserMine_h

#include <stdio.h>
#include "Actor.h"

class LaserMine : public Actor{
public:
    LaserMine(class Game* game, Actor* parent);
    
private:
    class MeshComponent* mc;
};

#endif /* LaserMine_h */
