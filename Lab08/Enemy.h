//
//  Enemy.h
//  Lab08
//
//  Created by Clifford Lin on 10/27/21.
//

#ifndef Enemy_h
#define Enemy_h

#include <stdio.h>
#include "Actor.h"

class Enemy : public Actor{
public:
    Enemy(class Game* game);
    
private:
    class MeshComponent* mc;
    class EnemyMove* em;
};

#endif /* Enemy_hpp */
