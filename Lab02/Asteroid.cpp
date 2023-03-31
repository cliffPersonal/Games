//
//  Asteroid.cpp
//  Lab02
//
//  Created by Clifford Lin on 9/8/21.
//

#include "Asteroid.h"
#include "Random.h"
#include "Game.h"

Asteroid::Asteroid(class Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
    
    mc = new MoveComponent(this);
    mc->SetForwardSpeed(150.0f);
    
    this->SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));
    this->SetPosition(Random::GetVector(Vector2(0.0f,0.0f), Vector2(WINDOW_WIDTH, WINDOW_HEIGHT)));
    mGame -> AddAsteroid(this);
}

Asteroid::~Asteroid()
{
    mGame -> RemoveAsteroid(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

void Asteroid::OnUpdate(float deltaTime){
    Vector2 pos = this->GetPosition();
    if(pos.x < 0.0f){
        this->SetPosition(Vector2(WINDOW_WIDTH, pos.y));
    }
    if(pos.x > WINDOW_WIDTH){
        this->SetPosition(Vector2(0.0f, pos.y));
    }
    if(pos.y < 0.0f){
        this->SetPosition(Vector2(pos.x, WINDOW_HEIGHT));
    }
    if(pos.y > WINDOW_HEIGHT){
        this->SetPosition(Vector2(pos.x, 0.0f));
    }
}
