//
//  Checkpoint.h
//  Lab11
//
//  Created by Clifford Lin on 11/11/21.
//

#ifndef Checkpoint_h
#define Checkpoint_h

#include <stdio.h>
#include <string>
#include "Actor.h"

class Checkpoint : public Actor{
public:
    Checkpoint(class Game* game, Actor* parent);
    void SetActive(){ active = true; };
    void SetInactive(){ active = false; };
    void SetLevelString(std::string s){ mLevelString = s; };
    std::string GetLevelString(){ return mLevelString; };
    
private:
    void OnUpdate(float deltaTime) override;
    
    class MeshComponent* mc;
    class CollisionComponent* cc;
    
    bool active;
    std::string mLevelString;
};

#endif /* Checkpoint_h */
