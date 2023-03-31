//
//  Ship.cpp
//  Lab02
//
//  Created by Clifford Lin on 9/7/21.
//

#include "Ship.h"
#include "Laser.h"

Ship::Ship(class Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Ship.png"));
    mc = new MoveComponent(this);
    laserTimer = 1.0f;
}

void Ship::OnProcessInput(const Uint8 *keyState){
    mc->SetForwardSpeed(0.0f);
    mc->SetAngularSpeed(0.0f);
    sc->SetTexture(this->GetGame()->GetTexture("Assets/Ship.png"));
    
    if(keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN]){
        mc->SetForwardSpeed(200.0f);
        sc->SetTexture(this->GetGame()->GetTexture("Assets/ShipThrust.png"));
    }
    if(keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP]){
        mc->SetForwardSpeed(-200.0f);
        sc->SetTexture(this->GetGame()->GetTexture("Assets/ShipThrust.png"));
    }
    if(keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_UP]){
        mc->SetForwardSpeed(0.0f);
        sc->SetTexture(this->GetGame()->GetTexture("Assets/Ship.png"));
    }
    if(keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT]){
        mc->SetAngularSpeed(5.0f);
    }
    if(keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT]){
        mc->SetAngularSpeed(-5.0f);
    }
    if(keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_LEFT]){
        mc->SetAngularSpeed(0.0f);
    }
    if(keyState[SDL_SCANCODE_SPACE] && laserTimer >= 1.0f){
        Laser* laser = new Laser(this->GetGame());
        laser->SetPosition(this->GetPosition());
        laser->SetRotation(this->GetRotation());
        laserTimer = 0.0f;
    }
}

void Ship::OnUpdate(float deltaTime){
    laserTimer += deltaTime;
}
