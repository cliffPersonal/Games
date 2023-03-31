//
//  Spawner.h
//  Lab04
//
//  Created by Clifford Lin on 9/22/21.
//

#ifndef Spawner_h
#define Spawner_h

#include <stdio.h>
#include <string>
#include "Actor.h"

class Spawner : public Actor{
public:
    Spawner(class Game* game);
    void SetType(std::string s){type = s;};
    void Trigger();
private:
    void OnUpdate(float deltaTime) override;
    std::string type;
};
#endif /* Spawner_h */
