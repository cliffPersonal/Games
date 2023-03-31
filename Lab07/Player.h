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
    Player(class Game* game);
    
private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
    class PlayerMove* pm;
};

#endif /* Player_h */
