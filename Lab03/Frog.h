//
//  Frog.h
//  Lab03
//
//  Created by Clifford Lin on 9/9/21.
//

#ifndef Frog_h
#define Frog_h

#include <stdio.h>
#include <unordered_map>
#include <string>
#include "Actor.h"

class Frog : public Actor{
public:
    Frog(class Game* game);
    
private:
    class SpriteComponent* sc;
    std::unordered_map<std::string, bool> lastFrame;
    class CollisionComponent* cc;
    bool onLog;
    bool reachedGoal;
    
    void OnProcessInput(const Uint8 *keyState) override;
    void OnUpdate(float deltaTime) override;
};

#endif /* Frog_h */
