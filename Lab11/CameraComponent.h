//
//  CameraComponent.h
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#ifndef CameraComponent_h
#define CameraComponent_h

#include <stdio.h>
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
    CameraComponent(class Actor* owner);
    // Update the move component
    void Update(float deltaTime) override;
    Vector3 IdealPosition();
    float GetPitchSpeed() { return mPitchSpeed; };
    void SetPitchSpeed(float speed) { mPitchSpeed = speed; };
    
private:
    const float HDIST = 60.0f;
    const float VDIST = 70.0f;
    const float TARGETDIST = 50.0f;
    
    float mPitchAngle = 0.0f;
    float mPitchSpeed = 0.0f;
    
    Vector3 zVector = Vector3::UnitZ;
    Vector2 sideNormal;
};
#endif /* CameraComponent_h */
