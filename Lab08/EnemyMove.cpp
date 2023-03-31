//
//  EnemyMove.cpp
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#include "EnemyMove.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "CSVHelper.h"
#include "HeightMap.h"
#include "PlayerMove.h"
#include <iostream>
#include <fstream>

EnemyMove::EnemyMove(Actor* owner)
:VehicleMove(owner)
{
    std::fstream fin;
    std::string fname = "Assets/HeightMap/Path.csv";
    fin.open(fname);
    
    std::string line;
    while(std::getline(fin, line)){
        std::vector<std::string> temp = CSVHelper::Split(line);
        if(temp[0] == "Node"){
            Vector3 ctw = mOwner->GetGame()->GetHeightMap()->CellToWorld(std::stoi(temp[1]), std::stoi(temp[2]));
            points.push_back(ctw);
        }
    }
    
    mOwner->SetPosition(points[0]);
}

void EnemyMove::Update(float deltaTime){
    Vector3 enemyToTarget = points[nextTargetIndex] - mOwner->GetPosition();
    if(enemyToTarget.Length() < 100.0f){
        nextTargetIndex++;
        if(nextTargetIndex == (int)points.size()){
            nextTargetIndex = 0;
        }
    }
    
    enemyToTarget.Normalize();
    float dot = Vector3::Dot(enemyToTarget, mOwner->GetForward());
    float angle = Math::Acos(dot);
    if(angle < Math::PiOver2/3.0f){
        mOwner->GetComponent<VehicleMove>()->SetPedalPressed(true);
    }
    else{
        mOwner->GetComponent<VehicleMove>()->SetPedalPressed(false);
    }
    
    if(dot >= 0.7f){
        Vector3 cross = Vector3::Cross(mOwner->GetForward(), enemyToTarget);
        if(cross.z < 0.0f){
            mOwner->GetComponent<VehicleMove>()->SetTurn(Turn::Left);
        }
        else if(cross.z > 0.0f){
            mOwner->GetComponent<VehicleMove>()->SetTurn(Turn::Right);
        }
        else{
            mOwner->GetComponent<VehicleMove>()->SetTurn(Turn::None);
        }
    }
    
    
    VehicleMove::Update(deltaTime);
}
