//
//  SecurityCamera.cpp
//  Lab11
//
//  Created by Clifford Lin on 11/17/21.
//

#include "SecurityCamera.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "SecurityCone.h"

SecurityCamera::SecurityCamera(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
    new SecurityCone(game, this);
    game->AddCamera(this);
}

SecurityCamera::~SecurityCamera(){
    if(mSFX != -1){
        Mix_HaltChannel(mSFX);
    }
    GetGame()->RemoveCamera(this);
}

void SecurityCamera::OnUpdate(float deltaTime){
    if(onPlayer){
        if(musicPlaying){
            if(mSFX != -1){
                Mix_HaltChannel(mSFX);
            }
            mSFX = -1;
            int chan = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
            Mix_Volume(chan, volume);
            musicPlaying = false;
        }
    }
    else if(!paused){
        if(!musicPlaying){
            mSFX = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
            Mix_Volume(mSFX, 0);
            musicPlaying = true;
        }
        
        float dist = (GetWorldPos() - GetGame()->GetPlayer()->GetWorldPos()).Length();
        if(dist < 500.0f){
            volume = 128;
        }
        else if(dist > 1500.0f){
            volume = 0;
        }
        else if(dist < 1500.0f && dist > 500.0f){
            volume = (int)((1500 - dist) * 0.128);
        }
        Mix_Volume(mSFX, volume);
        
        if(towardEnd){
            timer += deltaTime;
            if(timer >= interpTime){
                paused = true;
                towardEnd = false;
            }
        }
        else{
            timer -= deltaTime;
            if(timer <= 0.0f){
                paused = true;
                towardEnd = true;
            }
        }
        float val = timer / interpTime;
        mQuat = Quaternion::Slerp(qStart, qEnd, val);

    }
    else{
        if(musicPlaying){
            if(mSFX != -1){
                Mix_HaltChannel(mSFX);
            }
            mSFX = -1;
            int chan = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
            Mix_Volume(chan, volume);
            musicPlaying = false;
        }
        
        pauseTimer += deltaTime;
        if(pauseTimer >= pauseTime){
            paused = false;
            pauseTimer = 0.0f;
        }
    }
}

void SecurityCamera::SetStart(Quaternion s){
    qStart = s;
    mQuat = qStart;
}

void SecurityCamera::SetEnd(Quaternion e){
    qEnd = e;
}
