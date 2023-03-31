//
//  Vehicle.cpp
//  Lab03
//
//  Created by Clifford Lin on 9/9/21.
//

#include "Vehicle.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Frog.h"
#include "Math.h"

Vehicle::Vehicle(class Game* game, char inputLetter)
:Actor(game),
letter(inputLetter)
{
    sc = new SpriteComponent(this);
    if(letter == 'A'){
        sc->SetTexture(game->GetTexture("Assets/CarA.png"));
    }
    else if(letter == 'B'){
        sc->SetTexture(game->GetTexture("Assets/CarB.png"));
    }
    else if(letter == 'C'){
        sc->SetTexture(game->GetTexture("Assets/CarC.png"));
    }
    else if(letter == 'D'){
        sc->SetTexture(game->GetTexture("Assets/CarD.png"));
    }
    else if(letter == 'T'){
        sc->SetTexture(game->GetTexture("Assets/Truck.png"));
    }
    wm = new WrappingMove(this);
    wm->SetForwardSpeed(100.0f);
    
    cc = new CollisionComponent(this);
    if(letter == 'T'){
        cc->SetSize(128.0f, 48.0f);
    }
    else{
        cc->SetSize(64.0f, 64.0f);
    }
    mGame -> AddVehicle(this);
}

Vehicle::~Vehicle(){
    mGame -> RemoveVehicle(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

WrappingMove* Vehicle::GetWrappingMove(){
    return wm;
}

CollisionComponent* Vehicle::GetCollisionComp(){
    return cc;
}

void Vehicle::OnUpdate(float deltaTime){
    Frog* frog = mGame->GetFrog();
    Vector2 frogPos = frog->GetPosition();
    Vector2 vehiclePos = this->GetPosition();
    Vector2 vehicleDirection = this->GetWrappingMove()->GetDirection();
    Vector2 vehicleToFrog(frogPos.x - vehiclePos.x, frogPos.y - vehiclePos.y);
    
    vehicleToFrog.Normalize();
    float dot = Vector2::Dot(vehicleToFrog, vehicleDirection);
    float angle = Math::Acos(dot);
    
    if(angle < Math::PiOver2/3.0f){
        this->GetWrappingMove()->SetForwardSpeed(50.0f);
    }
    else{
        this->GetWrappingMove()->SetForwardSpeed(100.0f);
    }
}
