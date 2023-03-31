//
//  Boss.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#include "Boss.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "Fireball.h"
#include "Player.h"

Boss::Boss(Game* game)
:Actor(game)
{
    asc = new AnimatedSprite(this);
    std::vector<SDL_Texture*> idleAnim{
        game->GetTexture("Assets/Dragon/Idle0.png"),
        game->GetTexture("Assets/Dragon/Idle1.png")
    };
    asc->AddAnimation("idle", idleAnim);
    
    std::vector<SDL_Texture*> attackAnim{
        game->GetTexture("Assets/Dragon/Attack0.png"),
        game->GetTexture("Assets/Dragon/Attack1.png")
    };
    asc->AddAnimation("attack", attackAnim);
    
    asc->SetAnimation("idle");
    
    cc = new CollisionComponent(this);
    cc->SetSize(50.0f, 64.0f);
    
    ec = new EnemyComponent(this, 5);
}

void Boss::OnUpdate(float deltaTime){
    fireTimer += deltaTime;
    if(fireTimer >= 2.0f){
        fireTimer = 0.0f;
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Fireball.wav"), 0);
        
        Fireball* fballUp = new Fireball(this->GetGame());
        fballUp->SetPosition(this->GetPosition());
        fballUp->GetComponent<SpriteComponent>()->SetTexture(this->GetGame()->GetTexture("Assets/FireballGreen.png"));
        
        Fireball* fball = new Fireball(this->GetGame());
        fball->SetPosition(this->GetPosition());
        fball->GetComponent<SpriteComponent>()->SetTexture(this->GetGame()->GetTexture("Assets/FireballGreen.png"));
        
        Fireball* fballDown = new Fireball(this->GetGame());
        fballDown->SetPosition(this->GetPosition());
        fballDown->GetComponent<SpriteComponent>()->SetTexture(this->GetGame()->GetTexture("Assets/FireballGreen.png"));
        
        fballUp->SetRotation(Math::Pi - (Math::PiOver2/3.0f));
        fball->SetRotation(Math::Pi);
        fballDown->SetRotation(Math::Pi + (Math::PiOver2/3.0f));
        
        asc->SetAnimation("idle");
    }
    else if(fireTimer >= 1.75f){
        asc->SetAnimation("attack");
    }
}
