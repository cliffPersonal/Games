//
//  Spawner.h
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#ifndef Spawner_h
#define Spawner_h

#include <stdio.h>
#include "Actor.h"

class Spawner : public Actor{
public:
    Spawner(class Game* game);
    
private:
    void OnUpdate(float deltaTime) override;
};
#endif /* Spawner_h */
