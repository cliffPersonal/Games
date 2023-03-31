//
//  HUD.h
//  Lab12
//
//  Created by Clifford Lin on 11/26/21.
//

#ifndef HUD_h
#define HUD_h

#include <stdio.h>
#include "UIComponent.h"
#include <vector>

class HUD : public UIComponent{
public:
    HUD(class Actor* owner);
    ~HUD();
    
    void Update(float deltaTime) override;
    void Draw(class Shader* shader) override;
    
    void CoinUpdate();
    void CheckpointText();
    void RestartTimer(){ textTimerRestart = true; };
    
private:
    class Font* mFont;
    class Texture* mTimerText;
    class Texture* mCoinText;
    bool textTimerRestart = false;
    bool textShowing = false;
    float textTimer = 0.0f;
    
    class Texture* radar;
    class Texture* arrow;
    class Texture* blip;
    
    const float BOTPOSX = 400.0f;
    const float BOTPOSY = -285.0f;
    std::vector<Vector2> blips;
    std::vector<float> angles;
    
};

#endif /* HUD_h */
