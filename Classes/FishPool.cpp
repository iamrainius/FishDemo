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
            fishes[i] = new Fish(pos, size, layer);
        }
        
    }
}

FishPool* FishPool::create(Size& visibleSize, int cols, int rows, cocos2d::Layer *layer)
{
    FishPool* fp = new FishPool(visibleSize, cols, rows, layer);
    
    
    
    return fp;
}

void FishPool::addEventListener(EventListenerTouchOneByOne* listener)
{
    for (int i = 0; i < rows * cols; i++) {
        if (fishes[i] != NULL) {
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), fishes[i]->getSprite());
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