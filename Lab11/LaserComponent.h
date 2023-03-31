//
//  LaserComponent.h
//  Lab10
//
//  Created by Clifford Lin on 11/8/21.
//

#ifndef LaserComponent_h
#define LaserComponent_h

#include <stdio.h>
#include "MeshComponent.h"
#include "SegmentCast.h"
#include "Math.h"

class LaserComponent : public MeshComponent
{
public:
    LaserComponent(class Actor* owner);
private:
    void Update(float deltaTime) override;
    void Draw(class Shader* shader) override;
    Matrix4 SegToMat(LineSegment);
    
    std::vector<LineSegment> lsVector;
};

#endif /* LaserComponent_h */
