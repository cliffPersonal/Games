//
//  TiledBGComponent.cpp
//  Lab05
//
//  Created by Clifford Lin on 9/26/21.
//

#include "TiledBGComponent.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
    :SpriteComponent(owner, drawOrder)
{

}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
    if (mTexture)
    {
        int w = GetTexWidth();
        int tilesPerRow = w / tWidth;
        for(int i = 0; i < (int)csvInt.size(); i++){
            for(int j = 0; j < (int)csvInt[i].size(); j++){
                int num = csvInt[i][j];
                if(num != -1){
                    SDL_Rect r;
                    r.w = tWidth;
                    r.h = tHeight;
                    SDL_Rect srcRect;
                    srcRect.w = tWidth;
                    srcRect.h = tHeight;
                    int row = num / tilesPerRow;
                    int col = num % tilesPerRow;
                    srcRect.x = col * tWidth;
                    srcRect.y = row * tHeight;
                    r.x = j * tWidth - (int)(mOwner->GetGame()->GetCameraPosition().x);
                    r.y = i * tHeight - (int)(mOwner->GetGame()->GetCameraPosition().y);
                    // Draw (have to convert angle from radians to degrees, and clockwise to counter)
                    SDL_RenderCopyEx(renderer,
                        mTexture,
                        &srcRect,
                        &r,
                        0.0f,
                        nullptr,
                        SDL_FLIP_NONE);
                }
            }
        }
    }
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
    tWidth = tileWidth;
    tHeight = tileHeight;
    
    std::fstream fin;
    fin.open(fileName.c_str());
    std::string line;
    while(std::getline(fin, line)){
        if(line.find_first_not_of(' ') == std::string::npos)
        {
            break;
        }
        std::vector<std::string> temp = CSVHelper::Split(line);
        std::vector<int> in;
        
        for(int i = 0; i < (int)temp.size(); i++){
            int t = std::stoi(temp[i]);
            in.push_back(t);
        }
        csvInt.push_back(in);
    }
}
