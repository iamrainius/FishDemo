//
//  FishManager.h
//  FishDemo
//
//  Created by 张靖 on 15/1/19.
//
//

#ifndef __FISH_MANAGER__
#define __FISH_MANAGER__

#include <vector>
#include <list>
#include "Fish.h"

class FishManager
{
// static member
public:
    static FishManager* create(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
public:
    FishManager(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
    void ProcessTouch(cocos2d::Point location);
    
private:
    void initFishes(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
    void setupFrames();
    
    std::vector<std::list<Fish>::iterator> findNeibours(int x, int y);
    
    bool findFishByPos(int x, int y, std::list<Fish>::iterator& target);
    
    void remove();
    
private:
    cocos2d::Vec2 visibleSize;
    int cols;
    int rows;
    
    std::list<Fish>** fishes;
    float fishSize;
    
    cocos2d::Vector<cocos2d::SpriteFrame*> fishFrames[5];
};



#endif // __FISH_MANAGER__
