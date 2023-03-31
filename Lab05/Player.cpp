//
//  Player.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#include "Player.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PlayerMove.h"

Player::Player(Game* game)
:Actor(game)
{
    asc = new AnimatedSprite(this, 200);
    cc = new CollisionComponent(this);
    cc->SetSize(20.0f, 20.0f);
    pm = new PlayerMove(this);
    
    std::vector<SDL_Texture*> attackUpAnim{
        GetGame()->GetTexture("Assets/Link/AttackUp.png")
    };
    asc->AddAnimation("attackUp", attackUpAnim);
    
    std::vector<SDL_Texture*> attackDownAnim{
        GetGame()->GetTexture("Assets/Link/AttackDown.png")
    };
    asc->AddAnimation("attackDown", attackDownAnim);
    
    std::vector<SDL_Texture*> attackLeftAnim{
        GetGame()->GetTexture("Assets/Link/AttackLeft.png")
    };
    asc->AddAnimation("attackLeft", attackLeftAnim);
    
    std::vector<SDL_Texture*> attackRightAnim{
        GetGame()->GetTexture("Assets/Link/AttackRight.png")
    };
    asc->AddAnimation("attackRight", attackRightAnim);
    
    std::vector<SDL_Texture*> walkUpAnim{
        GetGame()->GetTexture("Assets/Link/WalkUp0.png"),
        GetGame()->GetTexture("Assets/Link/WalkUp1.png")
    };
    asc->AddAnimation("walkUp", walkUpAnim);
    asc->SetAnimation("walkUp");
    asc->SetIsPaused(true);
    
    std::vector<SDL_Texture*> walkDownAnim{
        GetGame()->GetTexture("Assets/Link/WalkDown0.png"),
        GetGame()->GetTexture("Assets/Link/WalkDown1.png")
    };
    asc->AddAnimation("walkDown", walkDownAnim);
    
    std::vector<SDL_Texture*> walkLeftAnim{
        GetGame()->GetTexture("Assets/Link/WalkLeft0.png"),
        GetGame()->GetTexture("Assets/Link/WalkLeft1.png")
    };
    asc->AddAnimation("walkLeft", walkLeftAnim);
    
    std::vector<SDL_Texture*> walkRightAnim{
        GetGame()->GetTexture("Assets/Link/WalkRight0.png"),
        GetGame()->GetTexture("Assets/Link/WalkRight1.png")
    };
    asc->AddAnimation("walkRight", walkRightAnim);
}

void Player::TakeDamage(int amount){
    hitPoints -= amount;
    if(hitPoints <= 0){
        this->SetState(ActorState::Paused);
        Mix_HaltChannel(mGame->GetSoundValue());
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/LinkDie.wav"), 0);
    }
    Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/LinkHit.wav"), 0);
}
