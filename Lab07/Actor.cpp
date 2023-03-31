#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
    :mGame(game)
    ,mState(ActorState::Active)
    ,mPosition(Vector3::Zero)
    ,mScale(1.0f)
    ,mRotation(0.0f)
{
    mGame -> AddActor(this);
}

Actor::~Actor()
{
    mGame -> RemoveActor(this);
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    if(mState == ActorState::Active){
        for(int i = 0; i < (int)mComponents.size(); i++){
            mComponents[i] -> Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    
    Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
    mWorldTransform = scaleMatrix * rotationMatrix * positionMatrix;
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
    if(mState == ActorState::Active){
        for(int i = 0; i < (int)mComponents.size(); i++){
            mComponents[i] -> ProcessInput(keyState);
        }
        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
    mComponents.emplace_back(c);
    std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
        return a->GetUpdateOrder() < b->GetUpdateOrder();
    });
}

Vector3 Actor::GetForward(){
    float x = Math::Cos(mRotation);
    float y = Math::Sin(mRotation);
    return Vector3(x,y, 0.0f);
}

