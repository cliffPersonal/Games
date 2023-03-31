//
//  Laser.hpp
//  Lab02
//
//  Created by Clifford Lin on 9/8/21.
//

#ifndef Laser_h
#define Laser_h

#include <stdio.h>
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Asteroid.h"

class Laser : public Actor{
public:
    Laser(Game* game);
    
private:
    SpriteComponent* sc;
    MoveComponent* mc;
    float laserTimer;
    
    void OnUpdate(float deltaTime) override;
};

#endif /* Laser_h */
