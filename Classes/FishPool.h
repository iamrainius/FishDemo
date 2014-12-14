//
//  FishPool.h
//  FishDemo
//
//  Created by 张靖 on 14/12/13.
//
//

#ifndef __FISH_POOL_H__
#define __FISH_POOL_H__

#include "Fish.h"

class FishPool
{
public:
    static FishPool* create(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
    FishPool(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    void PutFish(Fish* fish, int col, int row);
    Fish* GetFish(int col, int row);
    void clearFishes(int col, int row);
    void ProcessTouch(cocos2d::Point location);
    
private:
    int cols;
    int rows;
    Fish** fishes;
    
    int FindIndex(int col, int row);
    cocos2d::Vec2 FindPosition(int index);
};

#endif /* __FISH_POOL_H__ */
