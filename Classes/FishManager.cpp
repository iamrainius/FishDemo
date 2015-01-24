//
//  FishManager.cpp
//  FishDemo
//
//  Created by 张靖 on 15/1/19.
//
//

#include "FishManager.h"
#include "Definitions.h"
#include <vector>
#include <iterator>

USING_NS_CC;

using namespace std;

FishManager* FishManager::create(cocos2d::Size &visibleSize, int cols, int rows, cocos2d::Layer *layer)
{
    return new FishManager(visibleSize, cols, rows, layer);
}


FishManager::FishManager(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer)
{
    initFishes(visibleSize, cols, rows, layer);
    setupFrames();
}

void FishManager::ProcessTouch(cocos2d::Point location)
{
    for (int i = 0; i < cols; i++) {
        if (fishes[i] != NULL){
            int y = 0;
            for (list<Fish>::iterator it = fishes[i]->begin(); it != fishes[i]->end(); it++) {
                Point pos = it->GetFishPosition();
                Rect rect = Rect(pos.x - fishSize / 2, pos.y - fishSize / 2, fishSize, fishSize);
                
                if (rect.containsPoint(location)) {
//                    RemoveContinuousFishes(i);
                    // 击中，x = i,y = y;
                    // 寻找相邻鱼
                    
                    
                    vector<list<Fish>::iterator> neibours = findNeibours(i, y);
                    
                    log("num of neibours: %d", (int) neibours.size());
                    
                    break;
                }
                
                y++;
            }
        }
    }
}

vector<list<Fish>::iterator> FishManager::findNeibours(int x, int y)
{
    log("find neibour for: %d, %d", x, y);
    vector<list<Fish>::iterator> neibours;
    
    list<Fish>::iterator target;
    if (findFishByPos(x, y, target)) {
        neibours.push_back(target);
    }
    
    list<Fish>::iterator right;
    if (findFishByPos(x + 1, y, right)) {
        if (target->GetType() == right->GetType()) {
            log("neibour right: %d, %d", x + 1, y);
            neibours.push_back(right);
        }
    }
    
    list<Fish>::iterator left;
    if (findFishByPos(x - 1, y, left)) {
        if (target->GetType() == left->GetType()) {
            log("neibour left: %d, %d", x - 1, y);
            neibours.push_back(left);
        }
    }
    
    list<Fish>::iterator top;
    if (findFishByPos(x, y + 1, top)) {
        if (target->GetType() == top->GetType()) {
            log("neibour top: %d, %d", x, y + 1);
            neibours.push_back(top);
        }
    }
    
    list<Fish>::iterator bottom;
    if (findFishByPos(x, y - 1, bottom)) {
        if (target->GetType() == bottom->GetType()) {
            log("neibour bottom: %d, %d", x, y - 1);
            neibours.push_back(bottom);
        }
    }

    return neibours;
}

bool FishManager::findFishByPos(int x, int y, list<Fish>::iterator& target)
{
    list<Fish> *column = fishes[x];
    list<Fish>::iterator it = column->begin();
    for (int i = 0; i <= y && it != column->end(); i++) {
        it++;
    }
    
    target = it;
    
    if (target == column->end()) {
        return false;
    }
    
    return true;
}

void FishManager::initFishes(cocos2d::Size &visibleSize, int cols, int rows, cocos2d::Layer *layer)
{
    this->visibleSize = visibleSize;
    this->cols = cols;
    this->rows = rows;

    fishes = new list<Fish>*[cols];
    for (int i=0; i < cols; i++) {
        fishes[i] = new list<Fish>();
        for (int j = 0; j < rows; j++) {
            const float size = visibleSize.width / HORIZONTAL_BLOCKS;
            Vec2 pos(i * size + size / 2, j * size + size / 2 + visibleSize.height * 0.18);
            fishSize = size;
            Fish fish(pos, size, layer);
            fishes[i]->push_back(fish);
        }
    }
    
}

void FishManager::setupFrames()
{
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("yellow.plist", "yellow.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("red.plist", "red.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("purple.plist", "purple.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blue.plist", "blue.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("green.plist", "green.png");
    
    char filename[20];
    string patterns[5] = {
        "blue_%02d.png",
        "red_%02d.png",
        "green_%02d.png",
        "yellow_%02d.png",
        "purple_%02d.png"
    };
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 23; j++) {
            sprintf(filename, patterns[i].c_str(), j);
            log("%s", filename);
            fishFrames[i].pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
        }
    }
}


