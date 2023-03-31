#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
    :mGame(game)
    ,mState(ActorState::Active)
    ,mPosition(Vector3::Zero)
    ,mScale(1.0f)
    ,mRotation(0.0f)
    ,mParent(parent)
{
    if(parent == nullptr){
        mGame -> AddActor(this);
    }
    else{
        parent->AddChild(this);
    }
}

Actor::~Actor()
{
    while(!mChildren.empty()){
        delete mChildren.back();
    }
    if(mParent == nullptr){
        mGame -> RemoveActor(this);
    }
    else{
        mParent->RemoveChild(this);
    }
    for(Component* component : mComponents){
        delete component;
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    CalcWorldTransform();
    
    if(mState == ActorState::Active){
        for(int i = 0; i < (int)mComponents.size(); i++){
            mComponents[i] -> Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    
    CalcWorldTransform();
    
    for(Actor* aTemp: mChildren){
        aTemp->Update(deltaTime);
    }
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

Vector3 Actor::GetRight(){
    float x = Math::Cos(mRotation + Math::PiOver2);
    float y = Math::Sin(mRotation + Math::PiOver2);
    return Vector3(x, y, 0.0f);
}

Vector3 Actor::GetQuatForward(){
    Vector3 v = Vector3::Transform(Vector3::UnitX, mQuat);
    v.Normalize();
    return v;
}

void Actor::CalcWorldTransform(){
    Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 qRotationMatrix = Matrix4::CreateFromQuaternion(mQuat);
    Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
    mWorldTransform = scaleMatrix * rotationMatrix * qRotationMatrix * positionMatrix;
    
    if(mParent != nullptr){
        if(mInheritScale){
            mWorldTransform *= mParent->GetWorldTransform();
        }
        else{
            mWorldTransform *= mParent->GetWorldRotTrans();
        }
    }
}

Matrix4 Actor::GetWorldRotTrans(){
    Matrix4 temp;
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 qRotationMatrix = Matrix4::CreateFromQuaternion(mQuat);
    Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
    temp = rotationMatrix * qRotationMatrix * positionMatrix;
    if(mParent != nullptr){
        temp *= mParent->GetWorldRotTrans();
    }
    return temp;
}

Vector3 Actor::GetWorldPos(){
    return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward(){
    return mWorldTransform.GetXAxis();
}

void Actor::AddChild(Actor *a){
    mChildren.push_back(a);
}

void Actor::RemoveChild(Actor *a){
    mChildren.erase(std::find(mChildren.begin(), mChildren.end(), a));
}
