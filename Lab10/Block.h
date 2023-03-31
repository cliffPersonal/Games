//
//  Block.h
//  Lab09
//
//  Created by Clifford Lin on 11/1/21.
//

#ifndef Block_h
#define Block_h

#include <stdio.h>
#include "Actor.h"

class Block : public Actor{
public:
    Block(class Game* game, Actor* parent);
    virtual ~Block();
    bool GetIsMirror() { return isMirror; };
    void SetIsMirror(bool m) {isMirror = m; };
    bool GetIsRotate() {return isRotate; };
    void SetIsRotate(bool r) {isRotate = r; };
private:
    void OnUpdate(float deltaTime) override;
    
    class MeshComponent* mc;
    class CollisionComponent* cc;
    bool isMirror = false;
    bool isRotate = false;
};

#endif /* Block_h */
