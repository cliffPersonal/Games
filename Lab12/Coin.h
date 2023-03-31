//
//  Coin.h
//  Lab11
//
//  Created by Clifford Lin on 11/16/21.
//

#ifndef Coin_h
#define Coin_h

#include <stdio.h>
#include "Actor.h"

class Coin : public Actor{
public:
    Coin(class Game* game, Actor* parent);
    
private:
    void OnUpdate(float deltaTime) override;
    
    class MeshComponent* mc;
    class CollisionComponent* cc;
    
    float angle = 0.0f;
};

#endif /* Coin_h */
