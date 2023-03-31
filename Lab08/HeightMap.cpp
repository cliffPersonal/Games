//
//  HeightMap.cpp
//  Lab08
//
//  Created by Clifford Lin on 10/26/21.
//

#include "HeightMap.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>

HeightMap::HeightMap(){
    std::fstream fin;
    std::string fname = "Assets/HeightMap/HeightMap.csv";
    fin.open(fname);
    
    std::string line;
    while(std::getline(fin, line)){
        std::vector<std::string> temp = CSVHelper::Split(line);
        std::vector<int> intTemp;
        for(std::string s : temp){
            int translate = std::stoi(s);
            intTemp.push_back(translate);
        }
        grid.push_back(intTemp);
    }
}
bool HeightMap::IsCellOnTrack(int row, int col){
    if(row >= (int)grid.size()){
        return false;
    }
    if(col >= (int)grid[row].size()){
        return false;
    }
    if(grid[row][col] == -1){
        return false;
    }
    return true;
}

float HeightMap::GetHeightFromCell(int row, int col){
    float height = -40.0f + grid[row][col] * 5.0f;
    return height;
}

Vector3 HeightMap::CellToWorld(int row, int col){
    if(!IsCellOnTrack(row, col)){
        return Vector3::Zero;
    }
    else{
        float x = GRID_TOP - CELL_SIZE * row;
        float y = GRID_LEFT + CELL_SIZE * col;
        float z = GetHeightFromCell(row, col);
        return Vector3(x, y, z);
    }
}

Vector2 HeightMap::WorldToCell(float x, float y){
    Vector2 cell;
    cell.x = (abs(x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE;
    cell.y = (abs(y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE;
    return cell;
}

bool HeightMap::IsOnTrack(float x, float y){
    Vector2 cell = WorldToCell(x, y);
    return IsCellOnTrack((int)cell.x, (int)cell.y);
}

float HeightMap::GetHeight(float x, float y){
    if(IsOnTrack(x, y)){
        Vector2 cell = WorldToCell(x, y);
        return GetHeightFromCell((int)cell.x, (int)cell.y);
    }
    else{
        return -1000.0f;
    }
}
