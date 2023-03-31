//
//  Door.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/27/21.
//

#include "Door.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

Door::Door(Game* game)
:Actor(game)
{
    sc = new SpriteComponent(this, 100);
    cc = new CollisionComponent(this);
}

void Door::SetUp(std::string d, std::string s, std::string dest)
{
    if(d == "Up"){
        dir = Direction::Up;
        cc->SetSize(30.0f, 64.0f);
    }
    else if (d == "Down"){
        dir = Direction::Down;
        cc->SetSize(30.0f, 64.0f);
    }
    else if (d == "Left"){
        dir = Direction::Left;
        cc->SetSize(64.0f, 30.0f);
    }
    else if (d == "Right"){
        dir = Direction::Right;
        cc->SetSize(64.0f, 30.0f);
    }
    
    if(s == "Open"){
        st = State::Open;
    }
    else if(s == "Closed"){
        st = State::Closed;
    }
    else if(s == "Locked"){
        st = State::Locked;
    }
    
    des = dest;
    
    UpdateTexture(d, s);
    
}

void Door::UpdateTexture(std::string d, std::string s)
{
    std::string scImage = "Assets/Door/" + d + s + ".png";
    sc->SetTexture(this->GetGame()->GetTexture(scImage.c_str()));
}

void Door::SetState(State s)
{
    st = s;
    std::string d;
    if(dir == Direction::Up){
        d = "Up";
    }
    else if(dir == Direction::Down){
        d = "Down";
    }
    else if(dir == Direction::Left){
        d = "Left";
    }
    else if(dir == Direction::Right){
        d = "Right";
    }
    
    std::string so;
    if(s == State::Open){
        so = "Open";
    }
    else if(s == State::Closed){
        so = "Closed";
    }
    else if(s == State::Locked){
        so = "Locked";
    }
    
    UpdateTexture(d, so);
    
}
