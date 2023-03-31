//
//  SecurityCone.h
//  Lab11
//
//  Created by Clifford Lin on 11/17/21.
//

#ifndef SecurityCone_h
#define SecurityCone_h

#include <stdio.h>
#include "Actor.h"

class SecurityCone : public Actor{
public:
    SecurityCone(class Game* game, Actor* parent);
    
private:
    class MeshComponent* mc;
    float coneTimer = 0.0f;
    bool inCone = false;
    float deathTimer = 0.0f;
    bool death = false;
    int mSFX = -1;
    bool musicPlaying = false;
    
    void OnUpdate(float deltaTime) override;
};

#endif /* SecurityCone_h */
