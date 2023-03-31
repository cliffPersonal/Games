//
//  Arrow.h
//  Lab11
//
//  Created by Clifford Lin on 11/16/21.
//

#ifndef Arrow_h
#define Arrow_h

#include <stdio.h>
#include "Actor.h"

class Arrow : public Actor{
public:
    Arrow(class Game* game, Actor* parent);
    
private:
    void OnUpdate(float deltaTime) override;
    
    class MeshComponent* mc;
};

#endif /* Arrow_h */
