//
//  Skeleton.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "Skeleton.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "EnemyComponent.h"
#include "Fireball.h"
#include "Random.h"
#include "Math.h"
#include "Player.h"

Skeleton::Skeleton(Game* game)
:Actor(game)
{
    asc = new AnimatedSprite(this);
    std::vector<SDL_Texture*> sAnim{
        GetGame()->GetTexture("Assets/Skeleton0.png"),
        GetGame()->GetTexture("Assets/Skeleton1.png")
    };
    asc->AddAnimation("skeletonMove", sAnim);
    asc->SetAnimation("skeletonMove");
    cc = new CollisionComponent(this);
    cc->SetSize(25.0f, 25.0f);
    ec = new EnemyComponent(this);
    
    fireTimer = Random::GetFloatRange(1.0f, 3.0f);
}

void Skeleton::OnUpdate(float deltaTime){
    fireTimer += deltaTime;
    if(fireTimer >= 4.0f){
        fireTimer = 0.0f;
        Fireball* fball = new Fireball(this->GetGame());
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Fireball.wav"), 0);
        fball->SetPosition(this->GetPosition());
        
        Vector2 lPos = this->GetGame()->GetPlayer()->GetPosition();
        Vector2 sPos = this->GetPosition();
        Vector2 res = lPos - sPos;
        float angle = Math::Atan2(-res.y, res.x);
        fball->SetRotation(angle);
    }
    
}
