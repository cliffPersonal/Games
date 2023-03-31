//
//  Log.h
//  Lab03
//
//  Created by Clifford Lin on 9/9/21.
//

#ifndef Log_h
#define Log_h

#include <stdio.h>
#include "Actor.h"

class Log : public Actor{
public:
    Log(class Game* game, char inputLetter);
    virtual ~Log();
    class WrappingMove* GetWrappingMove();
    class CollisionComponent* GetCollisionComp(){return cc;};
    
private:
    class SpriteComponent* sc;
    class WrappingMove* wm;
    class CollisionComponent* cc;
    char letter;
};

#endif /* Log_h */
