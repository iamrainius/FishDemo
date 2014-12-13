#include "GameScene.h"
#include "GameOverScene.h"
#include "cocostudio/CocoStudio.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("game_over_bg.png");
    background->setColor(Color3B::GRAY);
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(background);
    
    return true;
}

void GameOverScene::GoToGameScene(cocos2d::Ref *sender)
{
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

