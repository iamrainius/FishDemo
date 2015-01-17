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
    this->fishPos = pos;
    
    isFalling = true;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    type = random(0, 4);
    fishSprite = Sprite::create(colors[type]);
    fishSprite->setPosition(Point(pos.x + origin.x, pos.y + origin.y));
    
    float scale = size / fishSprite->getContentSize().width;
    fishSprite->setScale(scale);
    fishPos = Vec2(fishSprite->getPositionX(), fishSprite->getPositionY());
    layer->addChild(fishSprite, 100);
    
    this->layer = layer;

}

void Fish::MoveToTarget()
{
    log("Move to %s, %f, %f", fishSprite->getName().c_str(), fishPos.x, fishPos.y);
    auto moveTo = MoveTo::create(0.5f, fishPos);
    auto func = CallFunc::create(CC_CALLBACK_0(Fish::onActionFinished, this));
    auto sequence = Sequence::create(moveTo, func, NULL);
    fishSprite->runAction(sequence);
//    fishSprite->setPosition(fishPos);
}

void Fish::onActionFinished()
{
    Vec2 pos = fishSprite->getPosition();
    log("Get to %s, %f, %f", fishSprite->getName().c_str(), pos.x, pos.y);
}

void Fish::SetTarget(float x, float y) {
    this->fishPos.x = x;
    this->fishPos.y = y;
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

