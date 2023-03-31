//
//  Asteroid.hpp
//  Lab02
//
//  Created by Clifford Lin on 9/8/21.
//

#ifndef Asteroid_h
#define Asteroid_h

#include <stdio.h>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

class Asteroid : public Actor{
public:
    Asteroid(Game* game);
    virtual ~Asteroid();
    
private:
    SpriteComponent* sc;
    MoveComponent* mc;
    const float WINDOW_WIDTH = 1024.0f;
    const float WINDOW_HEIGHT = 768.0f;
    
    void OnUpdate(float deltaTime) override;
};

#endif /* Asteroid_h */
