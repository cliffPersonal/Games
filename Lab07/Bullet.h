//
//  Bullet.h
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#ifndef Bullet_h
#define Bullet_h

#include <stdio.h>
#include "Actor.h"

class Bullet : public Actor{
public:
    Bullet(class Game* game);
    
private:
    class MeshComponent* meshc;
    class CollisionComponent* cc;
    class MoveComponent* movec;
    float existTimer = 0.0f;
    
    void OnUpdate(float deltaTime) override;
    void Explode(class Block* b);
};
#endif /* Bullet_h */
