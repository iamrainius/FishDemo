#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"
#include "MainMenuScene.h"
#include "SplashScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->scheduleOnce(schedule_selector(SplashScene::GoToMainMenu), DISPLAY_TIME_SPLASH_SCENE);
    
    auto background = Sprite::create("splash_bg.png");
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    //LIMENG
    const float ratio = visibleSize.width / background->getContentSize().width;
    background->setScale(ratio);
    this->addChild(background);

    return true;
}

void SplashScene::GoToMainMenu(float delayTime)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
