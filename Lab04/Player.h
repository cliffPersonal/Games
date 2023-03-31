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
    Player(class Game* game, char inputLetter);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    
    
private:
    class AnimatedSprite* asc;
    class CollisionComponent* cc;
    class PlayerMove* pm;
    char letter;
};

#endif /* Player_h */
