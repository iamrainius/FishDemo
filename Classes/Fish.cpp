//
//  Fish.cpp
//  FishDemo
//
//  Created by 张靖 on 14/12/13.
//
//

#include "Fish.h"
#include "Definitions.h"

USING_NS_CC;

Fish::Fish(cocos2d::Vec2& pos, float size, Layer *layer)
{
    
    isFalling = true;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    type = random(0, 4);
    fishSprite = Sprite::create(colors[type]);
    fishSprite->setPosition(Point(pos.x + origin.x, pos.y + origin.y));
    
    float scale = size / fishSprite->getContentSize().width;
    fishSprite->setScale(scale, scale);
    fishPos = Vec2(fishSprite->getPositionX(), fishSprite->getPositionY());
    layer->addChild(fishSprite, 100);
    this->layer = layer;

}

int Fish::GetType()
{
    return type;
}

Point Fish::GetFishPosition()
{
    return fishPos;
}

Size Fish::GetFishSize()
{
    return fishSprite->getContentSize();
}

void Fish::QuitFromScene()
{
    if (layer != NULL) {
        layer->removeChild(fishSprite);
    }
}

Sprite* Fish::getSprite()
{
    return fishSprite;
}

void Fish::Fall()
{
    
}

void Fish::StopFalling()
{
    isFalling = false;
}

