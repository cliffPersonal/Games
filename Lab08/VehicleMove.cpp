//
//  VehicleMove.cpp
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#include "VehicleMove.h"
#include "Actor.h"
#include "HeightMap.h"
#include "Game.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>

VehicleMove::VehicleMove(class Actor* owner)
:Component(owner, 50)
{
    std::fstream fin;
    std::string fname = "Assets/HeightMap/Checkpoints.csv";
    fin.open(fname);
    
    std::string line;
    while(std::getline(fin, line)){
        std::vector<std::string> temp = CSVHelper::Split(line);
        if(temp[0] == "Checkpoint"){
            Vector2 min, max;
            min.x = std::stof(temp[1]);
            min.y = std::stof(temp[3]);
            max.x = std::stof(temp[2]);
            max.y = std::stof(temp[4]);
            std::pair<Vector2, Vector2> p = {min, max};
            checkpoints.push_back(p);
        }
    }
}

void VehicleMove::Update(float deltaTime)
{
    if(aPedalPressed){
        aTimer += deltaTime;
        float lerpFrac = aTimer / AccRampTime;
        if(lerpFrac > 1.0f){
            lerpFrac = 1.0f;
        }
        float AccMag = Math::Lerp(MinAccMagnitude, MaxAccMagnitude, lerpFrac);
        vVelocity += mOwner->GetForward() * AccMag * deltaTime;
    }
    else{
        aTimer = 0.0f;
    }
    
    Vector3 pos = mOwner->GetPosition();
    pos += vVelocity * deltaTime;
    
    if(mOwner->GetGame()->GetHeightMap()->IsOnTrack(pos.x, pos.y)){
        float newHeight = mOwner->GetGame()->GetHeightMap()->GetHeight(pos.x, pos.y);
        pos.z = Math::Lerp(pos.z, newHeight, 0.1f);
    }
    
    mOwner->SetPosition(pos);
    
    if(aPedalPressed){
        vVelocity *= LinearDragCo;
    }
    else{
        vVelocity *= LinearDragCoFalse;
    }
    
    if(vTurn != Turn::None){
        float TurnFactor;
        if(vTurn == Turn::Left){
            TurnFactor = -1.0f;
        }
        else{
            TurnFactor = 1.0f;
        }
        AngularVelocity += AngularAcceleration * TurnFactor * deltaTime;
    }
    
    float angle = mOwner->GetRotation();
    angle += AngularVelocity * deltaTime;
    mOwner->SetRotation(angle);
    
    AngularVelocity *= AngularDragCo;
    
    std::pair<Vector2, Vector2> nCheck;
    if(lastCheckpoint == 7){
        nCheck = checkpoints[0];
    }
    else
    {
        nCheck = checkpoints[lastCheckpoint + 1];
    }
    Vector2 min = nCheck.first;
    Vector2 max = nCheck.second;
    Vector2 cell = mOwner->GetGame()->GetHeightMap()->WorldToCell(mOwner->GetPosition().x, mOwner->GetPosition().y);
    if((int)cell.x >= (int)min.x && (int)cell.x <= (int)max.x && (int)cell.y >= (int)min.y && (int)cell.y <= (int)max.y){
        if(lastCheckpoint == 7 || lastCheckpoint == -1){
            lastCheckpoint = 0;
            currentlap++;
            OnLapChange(currentlap);
        }
        else{
            lastCheckpoint++;
        }
    }
}

float VehicleMove::DistanceToNextCheckpoint(){
    std::pair<Vector2, Vector2> nCheck;
    if(lastCheckpoint == 7 || lastCheckpoint == -1){
        nCheck = checkpoints[0];
    }
    else
    {
        nCheck = checkpoints[lastCheckpoint + 1];
    }
    Vector3 worldDis = mOwner->GetGame()->GetHeightMap()->CellToWorld((int)nCheck.second.x, (int)nCheck.second.y);
    Vector2 worldDis2d;
    worldDis2d.x = worldDis.x;
    worldDis2d.y = worldDis.y;
    Vector2 vehicleXY;
    vehicleXY.x = mOwner->GetPosition().x;
    vehicleXY.y = mOwner->GetPosition().y;
    
    float dist = (vehicleXY - worldDis2d).Length();
    
    return dist;
}
