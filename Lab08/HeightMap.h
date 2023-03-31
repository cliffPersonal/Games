//
//  HeightMap.h
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#ifndef HeightMap_h
#define HeightMap_h

#include <stdio.h>
#include <vector>
#include "Math.h"

class HeightMap{
public:
    HeightMap();
    Vector3 CellToWorld(int row, int col);
    Vector2 WorldToCell(float x, float y);
    bool IsOnTrack(float x, float y);
    float GetHeight(float x, float y);
private:
    std::vector<std::vector<int>> grid;
    const float CELL_SIZE = 40.55f;
    const float GRID_TOP = 1280.0f;
    const float GRID_LEFT = -1641.0f;
    
    bool IsCellOnTrack(int row, int col);
    float GetHeightFromCell(int row, int col);
    
};

#endif /* HeightMap_h */
