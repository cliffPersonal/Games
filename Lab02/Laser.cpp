//
//  Laser.cpp
//  Lab02
//
//  Created by Clifford Lin on 9/8/21.
//

#include "Laser.h"

Laser::Laser(class Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Laser.png"));
    mc = new MoveComponent(this);
    mc->SetForwardSpeed(400.0f);
    laserTimer = 0.0f;
}

void Laser::OnUpdate(float deltaTime){
    laserTimer += deltaTime;
    if(laserTimer >= 1.0f){
        this->SetState(ActorState::Destroy);
    }
    
    std::vector<Asteroid*> asteroids = this->GetGame()->GetAsteroids();
    for(Asteroid* ast:asteroids){
        Vector2 lpos = this->GetPosition();
        Vector2 apos = ast->GetPosition();
        Vector2 subVector = lpos - apos;
        float dist = subVector.Length();
        if(dist <= 70){
            this->SetState(ActorState::Destroy);
            ast->SetState(ActorState::Destroy);
        }
    }
}
