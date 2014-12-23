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
    
    int GetType();

    cocos2d::Sprite *fishSprite;
    int type;
private:
    
    cocos2d::Layer *layer;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
//    cocos2d::Sprite *fishSprite;
    
    cocos2d::Vec2 fishPos;
    
    std::string GetRandomFishImage();
    
    bool isFalling;

};

#endif  // __FISH_H__
