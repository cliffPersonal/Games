//
//  EnemyMove.h
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#ifndef EnemyMove_h
#define EnemyMove_h

#include <stdio.h>
#include "VehicleMove.h"
#include "Math.h"
#include <vector>

class EnemyMove : public VehicleMove{
public:
    EnemyMove(class Actor* owner);
    void Update(float deltaTime) override;
    
private:
    std::vector<Vector3> points;
    int nextTargetIndex = 1;
};

#endif /* EnemyMove_h */
