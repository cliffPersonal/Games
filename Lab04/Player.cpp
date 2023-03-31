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

Player::Player(Game* game, char inputLetter)
:Actor(game),
letter(inputLetter)
{
    asc = new AnimatedSprite(this, 200);
    cc = new CollisionComponent(this);
    cc->SetSize(32.0f, 32.0f);
    pm = new PlayerMove(this);
    
    std::vector<SDL_Texture*> idleAnim{
        GetGame()->GetTexture("Assets/Mario/Idle.png")
    };
    asc->AddAnimation("idle", idleAnim);
    asc->SetAnimation("idle");
    
    std::vector<SDL_Texture*> deadAnim{
        GetGame()->GetTexture("Assets/Mario/Dead.png")
    };
    asc->AddAnimation("dead", deadAnim);
    
    std::vector<SDL_Texture*> jLeftAnim{
        GetGame()->GetTexture("Assets/Mario/JumpLeft.png")
    };
    asc->AddAnimation("jumpLeft", jLeftAnim);
    
    std::vector<SDL_Texture*> jRightAnim{
        GetGame()->GetTexture("Assets/Mario/JumpRight.png")
    };
    asc->AddAnimation("jumpRight", jRightAnim);
    
    std::vector<SDL_Texture*> rLeftAnim{
        GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft2.png")
    };
    asc->AddAnimation("runLeft", rLeftAnim);
    
    std::vector<SDL_Texture*> rRightAnim{
        GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight2.png")
    };
    asc->AddAnimation("runRight", rRightAnim);
}
