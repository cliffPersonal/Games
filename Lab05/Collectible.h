//
//  Collectible.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef Collectible_h
#define Collectible_h

#include <stdio.h>
#include "Component.h"
#include <functional>

class Collectible : public Component
{
public:
    Collectible(class Actor* owner);
    void SetOnCollect(std::function<void()> onCollect);
private:
    std::function<void()> mOnCollect;
    void Update(float deltaTime) override;
};

#endif /* Collectible_h */
