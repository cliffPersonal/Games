//
//  SecurityCamera.h
//  Lab11
//
//  Created by Clifford Lin on 11/17/21.
//

#ifndef SecurityCamera_h
#define SecurityCamera_h

#include <stdio.h>
#include "Actor.h"

class SecurityCamera : public Actor{
public:
    SecurityCamera(class Game* game, Actor* parent);
    virtual ~SecurityCamera();
    void SetStart(Quaternion s);
    void SetEnd(Quaternion e);
    void SetInterp(float i){ interpTime = i; };
    void SetPause(float p){ pauseTime = p; };
    void SetPause(bool b){ onPlayer = b; };
    
private:
    class MeshComponent* mc;
    
    void OnUpdate(float deltaTime) override;
    
    Quaternion qStart;
    Quaternion qEnd;
    float interpTime;
    float pauseTime;
    
    bool towardEnd = true;
    bool paused = false;
    float timer = 0.0f;
    float pauseTimer = 0.0f;
    bool onPlayer = false;
    
    int mSFX = -1;
    bool musicPlaying = false;
    int volume = 0;
};

#endif /* SecurityCamera_h */
