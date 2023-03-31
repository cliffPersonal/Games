//
//  Bullet.cpp
//  Lab07
//
//  Created by Clifford Lin on 10/20/21.
//

#include "Bullet.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Block.h"
#include "Player.h"
#include "PlayerMove.h"



Bullet::Bullet(Game* game)
:Actor(game)
{
    SetScale(0.5f);
    
    meshc = new MeshComponent(this);
    meshc->SetMesh(game->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
    
    cc = new CollisionComponent(this);
    cc->SetSize(10.0f, 10.0f, 10.0f);
    
    movec = new MoveComponent(this);
    float forward = game->GetPlayer()->GetComponent<PlayerMove>()->GetForwardSpeed();
    movec->SetForwardSpeed(forward + 500.0f);
}

void Bullet::OnUpdate(float deltaTime){
    existTimer += deltaTime;
    if(existTimer >= 1.0f){
        SetState(ActorState::Destroy);
    }
    
    for(Block* b: mGame->GetBlocks()){
        CollisionComponent* bcc = b->GetComponent<CollisionComponent>();
        if(cc->Intersect(bcc)){
            if(!b->GetExploding()){
                SetState(ActorState::Destroy);
            }
            else{
                Explode(b);
                Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
                SetState(ActorState::Destroy);
            }
        }
    }
}

void Bullet::Explode(Block* b){
    Vector3 bpos = b->GetPosition();
    b->SetState(ActorState::Destroy);
    for(Block* temp: mGame->GetBlocks()){
        Vector3 curr = bpos - temp->GetPosition();
        float distance = curr.Length();
        if(distance <= 50.0f && temp->GetState() != ActorState::Destroy){
            if(!temp->GetExploding()){
                temp->SetState(ActorState::Destroy);
            }
            else{
                Explode(temp);
            }
        }
    }
    return;
}
