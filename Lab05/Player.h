//
//  Player.hpp
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
#include "Actor.h"

class Player : public Actor{
public:
    Player(class Game* game);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    void PickedUpKey(){numKeys++;};
    void OpenDoor(){numKeys--;};
    int GetKeyNum(){return numKeys;};
    void TakeDamage(int amount);
    
private:
    class AnimatedSprite* asc;
    class CollisionComponent* cc;
    class PlayerMove* pm;
    int numKeys = 0;
    int hitPoints = 10;
};

#endif /* Player_h */
