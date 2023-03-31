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
#include "Math.h"

class Player : public Actor{
public:
    Player(class Game* game, Actor* parent);
    void SetRespawnPosition(Vector3 rp) { respawnPos = rp ;};
    Vector3 GetRespawnPosition() { return respawnPos; };
    
private:
    class PlayerMove* pm;
    class CameraComponent* cc;
    class CollisionComponent* collc;
    
    Vector3 respawnPos;
};

#endif /* Player_h */
