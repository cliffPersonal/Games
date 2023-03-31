//
//  SecretBlock.h
//  Lab05
//
//  Created by Clifford Lin on 9/27/21.
//

#ifndef SecretBlock_h
#define SecretBlock_h

#include <stdio.h>
#include "Actor.h"
#include "Door.h"

class SecretBlock : public Actor{
public:
    SecretBlock(class Game* game);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    void SetDirection(std::string);
    Direction GetDirection(){return dir;};
    void SetOXY(float x, float y){oX = x; oY = y;};
    float GetY(){return oY;};
    bool GetReached(){return reached;};
    void SetReached(){reached = true;};
    
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
    Direction dir;
    float oX;
    float oY;
    bool reached = false;
};

#endif /* SecretBlock_h */
