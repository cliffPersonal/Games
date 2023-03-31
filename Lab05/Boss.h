//
//  Boss.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef Boss_h
#define Boss_h

#include <stdio.h>
#include "Actor.h"

class Boss : public Actor{
public:
    Boss(class Game* game);
    
private:
    class AnimatedSprite* asc;
    class CollisionComponent* cc;
    class EnemyComponent* ec;
    void OnUpdate(float deltaTime) override;
    float fireTimer = 0.0f;
};

#endif /* Boss_h */
