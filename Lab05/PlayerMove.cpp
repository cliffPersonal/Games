//
//  PlayerMove.cpp
//  Lab04
//
//  Created by Clifford Lin on 9/16/21.
//

#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "SDL2/SDL_mixer.h"
#include "Collider.h"
#include "SecretBlock.h"
#include "Sword.h"
#include "EnemyComponent.h"

PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
{
    mYSpeed = 0.0f;
    direction = Direction::Up;
    directionVector = Vector2(0.0f, 0.0f);
    sword = new Sword(owner->GetGame());
}

void PlayerMove::ProcessInput(const Uint8 *keyState){
    bool thisFrame = false;
    AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
    if(!attacking){
        if(keyState[SDL_SCANCODE_UP]){
            direction = Direction::Up;
            directionVector = Vector2(0.0f, -1.0f);
            asc->SetIsPaused(false);
            asc->SetAnimation("walkUp");
        }
        else if(keyState[SDL_SCANCODE_DOWN]){
            direction = Direction::Down;
            directionVector = Vector2(0.0f, 1.0f);
            asc->SetIsPaused(false);
            asc->SetAnimation("walkDown");
        }
        else if(keyState[SDL_SCANCODE_LEFT]){
            direction = Direction::Left;
            directionVector = Vector2(-1.0f, 0.0f);
            asc->SetIsPaused(false);
            asc->SetAnimation("walkLeft");
        }
        else if(keyState[SDL_SCANCODE_RIGHT]){
            direction = Direction::Right;
            directionVector = Vector2(1.0f, 0.0f);
            asc->SetIsPaused(false);
            asc->SetAnimation("walkRight");
        }
        else if(keyState[SDL_SCANCODE_SPACE]){
            thisFrame = true;
            if(thisFrame && !mSpacePressed){
                timer = 0.25f;
                attacking = true;
                asc->SetIsPaused(false);
                this->UpdateSword();
                Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/SwordSlash.wav"), 0);
            }
        }
        else{
            asc->SetIsPaused(true);
            directionVector = Vector2(0.0f, 0.0f);
        }
    }
    else{
        if(keyState[SDL_SCANCODE_SPACE]){
            thisFrame = true;
        }
        asc->SetIsPaused(true);
        directionVector = Vector2(0.0f, 0.0f);
    }
    mSpacePressed = thisFrame;
}

void PlayerMove::Update(float deltaTime){
    if(attacking){
        timer -= deltaTime;
    }
    if(timer <= 0.0f){
        attacking = false;
    }
    
    Vector2 playerPos = mOwner->GetPosition();
    playerPos -= Vector2(300.0f, 224.f);
    mOwner->GetGame()->SetCameraPos(playerPos);
    
    Vector2 pos = mOwner->GetPosition();
    pos += directionVector * deltaTime * 130.0f;
    mOwner->SetPosition(pos);
    
    std::vector<Actor*>& roomActors = mOwner->GetGame()->GetEnemies(mOwner->GetGame()->GetCurrRoom());
    CollisionComponent* swordcc = sword->GetCollisionComponent();
    for(Actor* a:roomActors){
        if(attacking){
            if(swordcc->Intersect(a->GetComponent<CollisionComponent>())){
                a->GetComponent<EnemyComponent>()->TakeDamage(1);
            }
        }
    }
    
    CollisionComponent* playercc = mOwner->GetGame()->GetPlayer()->GetCollisionComponent();
    std::vector<Door*>& roomDoors = mOwner->GetGame()->GetDoors(mOwner->GetGame()->GetCurrRoom());

    SecretBlock* temp;
    for(SecretBlock* sb:mOwner->GetGame()->GetSecretBlocks())
    {
        CollisionComponent* sbcc = sb->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = playercc->GetMinOverlap(sbcc, offset);
        if(collision == CollSide::Bottom && direction == Direction::Up){
            Vector2 oPos = sb->GetPosition();
            if(!sb->GetReached()){
                oPos += directionVector * deltaTime * 30.0f;
            }
            sb->SetPosition(oPos);
            pos += offset;
            mOwner->SetPosition(pos);
            temp = sb;
            if(temp->GetPosition().y <= temp->GetY() - 32.0f && !temp->GetReached()){
                temp->SetReached();
                Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Secret.wav"), 0);
                std::vector<Door*>& rDoors = mOwner->GetGame()->GetDoors(mOwner->GetGame()->GetCurrRoom());
                for(Door* dr: rDoors)
                {
                    if(dr->GetState() == State::Closed){
                        dr->SetState(State::Open);
                        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                    }
                }
            }
            else{
                pos += directionVector * deltaTime * 30.0f;
            }
        }
    }
    
    for(Door* dr: roomDoors)
    {
        CollisionComponent* dcc = dr->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = playercc->GetMinOverlap(dcc, offset);
        if(dr->GetState() == State::Open){
            if(collision == CollSide::Bottom && direction == Direction::Up){
                mOwner->SetPosition(Vector2(dr->GetPosition()) - Vector2(0.0f, 112.0f));
                mOwner->GetGame()->SetCurrRoom(dr->GetDest());
            }
            else if(collision == CollSide::Top && direction == Direction::Down){
                mOwner->SetPosition(Vector2(dr->GetPosition()) + Vector2(0.0f, 112.0f));
                mOwner->GetGame()->SetCurrRoom(dr->GetDest());
            }
            else if(collision == CollSide::Right && direction == Direction::Left){
                mOwner->SetPosition(Vector2(dr->GetPosition()) - Vector2(112.0f, 0.0f));
                mOwner->GetGame()->SetCurrRoom(dr->GetDest());
            }
            else if(collision == CollSide::Left && direction == Direction::Right){
                mOwner->SetPosition(Vector2(dr->GetPosition()) + Vector2(112.0f, 0.0f));
                mOwner->GetGame()->SetCurrRoom(dr->GetDest());
            }
        }
        else if(dr->GetState() == State::Locked){
            int keyNum = mOwner->GetGame()->GetPlayer()->GetKeyNum();
            if(keyNum > 0){
                if(collision == CollSide::Bottom && direction == Direction::Up){
                    dr->SetState(State::Open);
                    mOwner->GetGame()->GetPlayer()->OpenDoor();
                    Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                }
                else if(collision == CollSide::Top && direction == Direction::Down){
                    dr->SetState(State::Open);
                    mOwner->GetGame()->GetPlayer()->OpenDoor();
                    Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                }
                else if(collision == CollSide::Right && direction == Direction::Left){
                    dr->SetState(State::Open);
                    mOwner->GetGame()->GetPlayer()->OpenDoor();
                    Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                }
                else if(collision == CollSide::Left && direction == Direction::Right){
                    dr->SetState(State::Open);
                    mOwner->GetGame()->GetPlayer()->OpenDoor();
                    Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                }
            }
        }
    }
    for(Collider* c:mOwner->GetGame()->GetColliders())
    {
        CollisionComponent* ccc = c->GetCollisionComponent();
        Vector2 offset;
        CollSide collision = playercc->GetMinOverlap(ccc, offset);
        if(collision != CollSide::None){
            mOwner->SetPosition(mOwner->GetPosition() + offset);
        }
    }
    this->UpdateSword();
}

void PlayerMove::UpdateSword(){
    if(!attacking){
        sword->GetComponent<SpriteComponent>()->SetIsVisible(false);
    }
    else{
        sword->GetComponent<SpriteComponent>()->SetIsVisible(true);
        Vector2 offset;
        if(direction == Direction::Up){
            sword->GetComponent<SpriteComponent>()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Up.png"));
            offset.x = -8.0f;
            offset.y = -16.0f;
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("attackUp");
        }
        else if(direction == Direction::Down){
            sword->GetComponent<SpriteComponent>()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Down.png"));
            offset.x = 8.0f;
            offset.y = 16.0f;
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("attackDown");
        }
        else if(direction == Direction::Left){
            sword->GetComponent<SpriteComponent>()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Left.png"));
            offset.x = -16.0f;
            offset.y = 4.0f;
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("attackLeft");
        }
        else if(direction == Direction::Right){
            sword->GetComponent<SpriteComponent>()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Right.png"));
            offset.x = 16.0f;
            offset.y = 4.0f;
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("attackRight");
        }
        Vector2 pos = mOwner->GetPosition();
        Vector2 tot = pos + offset;
        sword->SetPosition(tot);
    }
}
