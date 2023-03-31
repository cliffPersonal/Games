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

class PlayerMove : public MoveComponent{
public:
    enum MoveState {
        OnGround,
        Jump,
        Falling
    };
    PlayerMove(class Actor* owner);
    
    void ChangeState(MoveState m){ mCurrentState = m; };
    
protected:
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    enum class CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
    
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
    bool mSpacePressed = false;
    
    const float GRAVITYZ = -980.0f;
    const float JUMPZ = 35000.0f;
    const float FORCEMULT = 700.0f;
    const float XYMULT = 0.9f;
    const float XYMAXLENGTH = 400.0f;
    const float MOUSEDENOM = 500.0f;
    const float MOUSEMULT = 10.0f;
};

#endif /* PlayerMove_h */
