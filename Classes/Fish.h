//
//  Fish.h
//  FishDemo
//
//  Created by 张靖 on 14/12/13.
//
//

#ifndef __FISH_H__
#define __FISH_H__

class Fish
{
public:
    Fish(cocos2d::Vec2& pos, float size, cocos2d::Layer *layer);
    
    void Fall();
    
    void StopFalling();
    
    void QuitFromScene();
    
    cocos2d::Sprite* getSprite();
    
    cocos2d::Point GetFishPosition();
    
    cocos2d::Size GetFishSize();
    
    void MoveToTarget();
    
    void moveToTarget(cocos2d::CallFunc* func);
    
    int GetType();
    
    //int GetFishRow();
    
    
    void SetTarget(float x, float y);
    
    cocos2d::Sprite *fishSprite;
    
    cocos2d::Vec2 fishPos;
    
    int type;
private:
    std::string colors[5] = { "block_blue.png", "block_red.png", "block_pink.png", "block_yellow.png", "block_purple.png" };
    
    cocos2d::Layer *layer;
    
    cocos2d::Size visibleSize;
    
    cocos2d::Vec2 origin;
    
    std::string GetRandomFishImage();
    
    bool isFalling;
    
    void onActionFinished();

};

#endif  // __FISH_H__
