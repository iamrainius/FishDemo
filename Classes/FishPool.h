//
//  FishPool.h
//  FishDemo
//
//  Created by 张靖 on 14/12/13.
//
//

#ifndef __FISH_POOL_H__
#define __FISH_POOL_H__

#include <list>
#include "Fish.h"

using namespace std;
class FishPool
{
public:
    static FishPool* create(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
    FishPool(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
    void PutFish(Fish* fish, int col, int row);
    
    Fish* GetFish(int col, int row);
    
    int GetFishNum();
    
    void clearFishes(int col, int row);
    
//    void ProcessTouch(cocos2d::Point location);
    
    Fish* getTouchedFish(cocos2d::Point location);
    
    std::function<void(int)> onScoreUpdate;
    
    int getFishSize();
    
    Fish* getNeibourFish(Fish* fish, int side);
    
    bool swapFishes(Fish* fish1, Fish* fish2);
    
    int findFish(Fish* fish);
    
    bool checkRemoveFishes(std::vector<int> seeds);
    
    void CheckBlink(Fish* fish);
    
    void CheckTouched(Fish* fish);
    
    vector<Fish*> getRandomFishes(int fishesNum);
    
    void blink(float blinkFishPercentage);
    
private:
    int cols;
    
    int rows;
    
    Fish** fishes;
    
    int fishSize;
    
    cocos2d::Vector<cocos2d::SpriteFrame*> fishFrames[5];
    
    cocos2d::Vector<cocos2d::SpriteFrame*> bubbleFrames;
    
    cocos2d::Vector<cocos2d::SpriteFrame*> blinkFrames[5];
    
    cocos2d::Vector<cocos2d::SpriteFrame*> touchedFrames[5];
    
    cocos2d::Size visibleSize;
    
    int score;
    
    int removeCount;
    
    vector<int> toFall;
    
    int FindIndex(int col, int row);
    
    cocos2d::Vec2 FindPosition(int index);
    
//    void RemoveContinuousFishes(int fishIndex);
    
    vector<int> getNeibours(int index);
    
    vector<int> getNeibours(int index, int type);
    
    bool contains(vector<int>& v, int val);
    
    int vectorSize(vector<int>& v);
    
    void setupFrames();
    
    void initPool(cocos2d::Size& visibleSize, int cols, int rows, cocos2d::Layer* layer);
    
    void removeFish(int index);

    std::vector<int> getAbove(int index);
    
    void funCallback(int index, std::vector<int>& fs, int total);
    
    void fall(std::vector<int> fs);
    
    void fall2(std::vector<int> fs);
    
    int findFishIndexByPoint(cocos2d::Point point);
    
    std::vector<int> findContinuousFishes(int index, int type);
    
    //定时调用函数
    void timeUpdate( float fDelta );
    
};

#endif /* __FISH_POOL_H__ */
