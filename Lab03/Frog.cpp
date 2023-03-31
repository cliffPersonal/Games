//
//  Frog.cpp
//  Lab03
//
//  Created by Clifford Lin on 9/9/21.
//

#include "Frog.h"
#include "CollisionComponent.h"
#include "WrappingMove.h"
#include "Vehicle.h"
#include "DeadFrog.h"
#include "Log.h"
#include "Game.h"
#include "SpriteComponent.h"

Frog::Frog(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Frog.png"));
    cc = new CollisionComponent(this);
    cc->SetSize(50.0f, 50.0f);
    onLog = false;
    reachedGoal = false;
}

void Frog::OnProcessInput(const Uint8 *keyState){
    float x = this->GetPosition().x;
    float y = this->GetPosition().y;
    std::unordered_map<std::string, bool> thisFrame = {{"UP", false}, {"DOWN", false}, {"LEFT", false}, {"RIGHT", false}};
    
    if(keyState[SDL_SCANCODE_UP]){
        thisFrame["UP"] = true;
        if(lastFrame["UP"] == false && thisFrame["UP"] == true){
            y -= 64.0f;
            if(y < 160.0f){
                y = 160.0f;
            }
            this->SetPosition(Vector2(x, y));
        }
    }
    if(keyState[SDL_SCANCODE_DOWN]){
        thisFrame["DOWN"] = true;
        if(lastFrame["DOWN"] == false && thisFrame["DOWN"] == true){
            y += 64.0f;
            if(y > 928.0f){
                y = 928.0f;
            }
            this->SetPosition(Vector2(x, y));
        }
    }
    if(keyState[SDL_SCANCODE_RIGHT]){
        thisFrame["RIGHT"] = true;
        if(lastFrame["RIGHT"] == false && thisFrame["RIGHT"] == true){
            x += 64.0f;
            if(x > 832.0f){
                x = 832.0f;
            }
            this->SetPosition(Vector2(x, y));
        }
    }
    if(keyState[SDL_SCANCODE_LEFT]){
        thisFrame["LEFT"] = true;
        if(lastFrame["LEFT"] == false && thisFrame["LEFT"] == true){
            x -= 64.0f;
            if(x < 64.0f){
                x = 64.0f;
            }
            this->SetPosition(Vector2(x, y));
        }
    }
    lastFrame = thisFrame;
}

void Frog::OnUpdate(float deltaTime){
    for(Vehicle* v: mGame->GetVehicles()){
        if(cc->Intersect(v->GetCollisionComp())){
            DeadFrog* dFrog = new DeadFrog(mGame);
            dFrog->SetPosition(this->GetPosition());
            this->SetPosition(Vector2(448.0f, 928.0f));
        }
    }
    
    Vector2 frogPos = mGame->GetFrog()->GetPosition();
    onLog = false;
    for(Log* l: mGame->GetLogs()){
        Vector2 offset;
        CollSide side = cc->GetMinOverlap(l->GetCollisionComp(), offset);
        if(side != CollSide::None){
            onLog = true;
            frogPos.y = l->GetPosition().y;
            WrappingMove* wm = l->GetWrappingMove();
            Vector2 newPos = frogPos + (wm->GetDirection() * wm->GetForwardSpeed() * deltaTime);
            if(side == CollSide::Left){
                newPos.x += offset.x + 32.0f;
            }
            if(side == CollSide::Right){
                newPos.x += offset.x - 32.0f;
            }
            this->SetPosition(newPos);
        }
    }
    
    Vector2 off;
    if(cc->GetMinOverlap(mGame->GetGoal(), off) != CollSide::None){
        Vector2 goalPos;
        goalPos.x = 448.0f;
        goalPos.y = 160.0f;
        mGame->GetFrog()->SetPosition(goalPos);
        mGame->GetFrog()->SetState(ActorState::Paused);
        reachedGoal = true;
    }
    
    if(!reachedGoal && !onLog && frogPos.y >= 150.0f && frogPos.y <= 510.0f){
        DeadFrog* dFrog = new DeadFrog(mGame);
        dFrog->SetPosition(this->GetPosition());
        this->SetPosition(Vector2(448.0f, 928.0f));
    }
    
    if(!reachedGoal && frogPos.y < 180.0f){
        DeadFrog* dFrog = new DeadFrog(mGame);
        dFrog->SetPosition(this->GetPosition());
        this->SetPosition(Vector2(448.0f, 928.0f));
    }
    
}
