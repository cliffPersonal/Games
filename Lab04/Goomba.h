//
//  Goomba.h
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#ifndef Goomba_h
#define Goomba_h

#include <stdio.h>
#include "Actor.h"

class Goomba : public Actor{
public:
    Goomba(class Game* game);
    virtual ~Goomba();
    class CollisionComponent* GetCollisionComponent(){return cc;};
    void SetStomped(){stomped = true;};
    
private:
    class AnimatedSprite* asc;
    class CollisionComponent* cc;
    class GoombaMove* gm;
    bool stomped;
    float deathTimer;
    
    void OnUpdate(float deltaTime) override;
};
#endif /* Goomba_h */
