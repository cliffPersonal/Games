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
    void SnapToIdeal();
    
private:
    const float HDIST = 60.0f;
    const float VDIST = 70.0f;
    const float TARGETDIST = 50.0f;
    const float SPRINGCONST = 256.0f;
    float DampenConst;
    Vector3 CameraPos = Vector3::Zero;
    Vector3 CameraVelocity = Vector3::Zero;
};
#endif /* CameraComponent_h */
