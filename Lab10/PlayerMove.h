//
//  PlayerMove.h
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#ifndef PlayerMove_h
#define PlayerMove_h

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"

class PlayerMove : public MoveComponent{
public:
    enum MoveState {
        OnGround,
        Jump,
        Falling,
        WallClimb,
        WallRun
    };
    PlayerMove(class Actor* owner);
    
    void ChangeState(MoveState m){ mCurrentState = m; };
    Vector3 GetVelocity() { return mVelocity; };
    MoveState GetCurrentState() { return mCurrentState; };
    CollSide GetWallRunColl() { return mWallRunColl; };
    
protected:
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    void UpdateWallClimb(float deltaTime);
    void UpdateWallRun(float deltaTime);
    enum CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
    
private:
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void PhysicsUpdate(float deltaTime);
    void AddForce(const Vector3& force);
    void FixXYVelocity();
    
    MoveState mCurrentState;
    Vector3 mVelocity;
    Vector3 mAcceleration;
    Vector3 mPendingForces;
    float mMass = 1.0f;
    Vector3 mGravity;
    Vector3 mJumpForce;
    Vector3 mWallClimbForce;
    Vector3 mWallRunForce;
    float mWallRunTimer;
    float mWallClimbTimer;
    bool mSpacePressed = false;
    CollSide mWallRunColl;
};

#endif /* PlayerMove_h */
