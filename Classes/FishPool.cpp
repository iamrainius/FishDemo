//
//  FishPool.cpp
//  FishDemo
//
//  Created by 张靖 on 14/12/13.
//
//

#include "FishPool.h"
#include "Fish.h"
#include "Definitions.h"

USING_NS_CC;
using namespace std;

typedef Fish* PFish;

FishPool::FishPool(Size& visibleSize, int cols, int rows, cocos2d::Layer* layer)
{
    this->cols = cols;
    this->rows = rows;
    fishes = new PFish[cols * rows];
    
    for (int i = 0; i < cols * rows; i++)
    {
        Vec2 logicPos = FindPosition(i);
        if (logicPos.x >= 0 && logicPos.y >= 0) {
            int col = logicPos.x;
            int row = logicPos.y;
            
            float size = visibleSize.width / HORIZONTAL_BLOCKS;
            Vec2 pos(col * size + size / 2, row * size + size / 2 + visibleSize.height * 0.12);
            fishSize = size;
            fishes[i] = new Fish(pos, size, layer);
        }
        
    }
}

FishPool* FishPool::create(Size& visibleSize, int cols, int rows, cocos2d::Layer *layer)
{
    FishPool* fp = new FishPool(visibleSize, cols, rows, layer);
    return fp;
}


void FishPool::ProcessTouch(cocos2d::Point location)
{
    for (int i = 0; i < rows * cols; i++) {
        if (fishes[i] == NULL) {
            continue;
        }
        
        Point pos = fishes[i]->GetFishPosition();
        Rect rect = Rect(pos.x - fishSize / 2, pos.y - fishSize / 2, fishSize, fishSize);
        
        if (rect.containsPoint(location)) {
            log("click at %f, %f, %d", location.x, location.y, i);
            RemoveContinuousFishes(i);
            break;
        }
    }
}

int FishPool::FindIndex(int col, int row)
{
    if (col < 0 || col >= cols || row < 0 || row >= rows) {
        return -1;
    }
    
    return row * cols + col;
}

cocos2d::Vec2 FishPool::FindPosition(int index)
{
    if (index < 0 || index >= cols * rows) {
        return Vec2(-1, -1);
    }
    
    int row = index / cols;
    int col = index % cols;
    
    return cocos2d::Vec2(col, row);
}

void FishPool::PutFish(Fish *fish, int col, int row)
{
    int position = this->FindIndex(col, row);
    if (position < 0) {
        return;
    }
    
    Fish *currFish = fishes[position];
    if (currFish != NULL) {
        currFish->QuitFromScene();
        delete currFish;
    }
    
    fishes[position] = fish;
}

Fish* FishPool::GetFish(int col, int row)
{
    int position = FindIndex(col, row);
    if (position < 0) {
        return NULL;
    }
    
    return fishes[position];
}


bool FishPool::contains(vector<int>& v, int val)
{
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == val) {
            return true;
        }
    }
    
    return false;
}

void FishPool::RemoveContinuousFishes(int fishIndex)
{
    vector<int> continuous;
    
    int cursor = 0;
    int count = 0;
    continuous.push_back(fishIndex);
    
    while (cursor < continuous.size()) {
        vector<int> neibours = GetNeibours(continuous.at(cursor));
        log("neibours size: %lu", neibours.size());
        for (int i = 0; i < neibours.size(); i++) {
            int index = neibours.at(i);
            if (!contains(continuous, index)) {
                continuous.push_back(index);
            }
        }
        log("Continuous size: %lu", continuous.size());
        cursor++;
    }
    
    for (int j = 0; j < continuous.size(); j++) {
        fishes[continuous.at(j)]->QuitFromScene();
//        Fish* fish = fishes[j];
//        fishes[j] = NULL;
//        delete fish;
    }

}



vector<int> FishPool::GetNeibours(int index)
{
    vector<int> neibours;
    Vec2 pos = FindPosition(index);
    
    int neibour;
    neibour = FindIndex(pos.x + 1, pos.y);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    neibour = FindIndex(pos.x - 1, pos.y);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    neibour = FindIndex(pos.x, pos.y + 1);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    neibour = FindIndex(pos.x, pos.y - 1);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    return neibours;
}

int FishPool::vectorSize(vector<int>& v)
{
    return 1;
}



