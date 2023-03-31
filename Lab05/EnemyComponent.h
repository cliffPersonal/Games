//
//  EnemyComponent.h
//  Lab05
//
//  Created by Clifford Lin on 9/28/21.
//

#ifndef EnemyComponent_h
#define EnemyComponent_h

#include <stdio.h>
#include "Component.h"
#include <functional>

class EnemyComponent : public Component
{
public:
    EnemyComponent(class Actor* owner);
    EnemyComponent(class Actor* owner, int health);
    ~EnemyComponent();
    void SetOnDeath(std::function<void()> onDeath);
    void SetOnTakeDamage(std::function<void()> onDamage);
    void TakeDamage(int amount);
private:
    int hitPoint = 1;
    float invulnerableTimer = 1.0f;
    void Update(float deltaTime) override;
    std::function<void()> mOnDeath;
    std::function<void()> mOnTakeDamage;
    bool takenDamage = false;
    bool boss = false;
};

#endif /* EnemyComponent_h */
