//
//  Door.h
//  Lab05
//
//  Created by Clifford Lin on 9/27/21.
//

#ifndef Door_h
#define Door_h

#include <stdio.h>
#include <string>
#include "Actor.h"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

enum class State
{
    Open,
    Closed,
    Locked
};

class Door : public Actor{
public:
    Door(class Game* game);
    void SetUp(std::string d, std::string s, std::string dest);
    void UpdateTexture(std::string d, std::string s);
    class CollisionComponent* GetCollisionComponent(){return cc;};
    std::string GetDest(){return des;};
    State GetState(){return st;};
    void SetState(State s);
    
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
    Direction dir;
    State st;
    std::string des;
};

#endif /* Door_h */
