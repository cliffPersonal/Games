//
//  TiledBGComponent.h
//  Lab05
//
//  Created by Clifford Lin on 9/26/21.
//

#ifndef TiledBGComponent_h
#define TiledBGComponent_h

#include <stdio.h>
#include "SpriteComponent.h"
#include <vector>
#include <string>

class TiledBGComponent : public SpriteComponent
{
public:
    TiledBGComponent(class Actor* owner, int drawOrder = 50);
    void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
private:
    int tWidth;
    int tHeight;
    std::vector<std::vector<int>> csvInt;
    
    void Draw(SDL_Renderer *renderer) override;
};

#endif /* TiledBGComponent_h */
