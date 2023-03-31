//
//  Player.h
//  Lab07
//
//  Created by Clifford Lin on 10/19/21.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
#include "Actor.h"

class Player : public Actor{
public:
    Player(class Game* game, Actor* parent);
    
private:
    class PlayerMove* pm;
    class CameraComponent* cc;
    class CollisionComponent* collc;
};

#endif /* Player_h */
