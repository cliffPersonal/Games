//
//  VehicleMove.h
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#ifndef VehicleMove_h
#define VehicleMove_h

#include <stdio.h>
#include <vector>
#include <utility>
#include "Math.h"
#include "Component.h"

enum class Turn
{
    None,
    Left,
    Right
};

class VehicleMove : public Component
{
public:
    VehicleMove(class Actor* owner);
    
    void SetPedalPressed(bool pressed){ aPedalPressed = pressed; }
    void SetTurn(Turn t){ vTurn = t; }
    int GetCurrentLap(){ return currentlap; }
    int GetLastCheck(){ return lastCheckpoint; }
    float DistanceToNextCheckpoint();
    // Update the move component
    void Update(float deltaTime) override;
    virtual void OnLapChange(int newLap) { };
    
private:
    bool aPedalPressed = false;
    Turn vTurn = Turn::None;
    Vector3 vVelocity = Vector3::Zero;
    float AngularVelocity = 0.0f;
    float aTimer = 0.0f;
    
    float MinAccMagnitude = 1000.0f;
    float MaxAccMagnitude = 2500.0f;
    float AccRampTime = 2.0f;
    float AngularAcceleration = 5 * Math::Pi;
    float LinearDragCo = 0.9f;
    float LinearDragCoFalse = 0.975f;
    float AngularDragCo = 0.9f;
    
    std::vector<std::pair<Vector2, Vector2>> checkpoints;
    int currentlap = 0;
    int lastCheckpoint = -1;
};

#endif /* VehicleMove_h */
