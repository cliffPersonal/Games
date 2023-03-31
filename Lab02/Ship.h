//
//  Ship.h
//  Lab02
//
//  Created by Clifford Lin on 9/7/21.
//

#ifndef Ship_h
#define Ship_h

#include <stdio.h>
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

class Ship : public Actor{
public:
    Ship(Game* game);
    
private:
    SpriteComponent* sc;
    MoveComponent* mc;
    float laserTimer;
    
    void OnProcessInput(const Uint8 *keyState) override;
    void OnUpdate(float deltaTime) override;
};

#endif /* Ship_h */
