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
    
    fishSprite = Sprite::create(GetRandomFishImage());
    fishSprite->setPosition(Point(pos.x + origin.x, pos.y + origin.y));
    
    float scale = size / fishSprite->getContentSize().width;
    fishSprite->setScale(scale, scale);
    
    auto fishBody = PhysicsBody::createBox(Size(Point(fishSprite->getContentSize().width, fishSprite->getContentSize().width)));
    fishBody->setCollisionBitmask(FISH_COLLISION_BITMASK);
    fishBody->setContactTestBitmask(true);
    fishBody->setGravityEnable(true);
    fishSprite->setPhysicsBody(fishBody);
    layer->addChild(fishSprite, 100);
    
    this->layer = layer;

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

std::string Fish::GetRandomFishImage()
{
    int blockColor = random(0, 4);
    return colors[blockColor];
}

void Fish::Fall()
{
    
}

void Fish::StopFalling()
{
    isFalling = false;
}

