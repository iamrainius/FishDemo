#include "GameScene.h"
#include "MainMenuScene.h"
#include "AppDelegate.h"
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
 //   score = 0;
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("game_over_bg.png");
    //auto background = Sprite::create("screenshot.png");
    //需要把path传进来
//    AppDelegate* app = (AppDelegate *)Application::getInstance();
//    bgPath = app->getScreenshotPath();

//    auto background = Sprite::create(bgPath);
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    
    //LIMENG
    const float ratio = visibleSize.width / background->getContentSize().width;
    background->setScale(ratio);
    this->addChild(background);
    
    
    //第X关，目标分数XXXX
    //需要判断胜利or失败，以下是胜利的逻辑
    auto checkpointItem = CCLabelTTF::create("第1关", "微软雅黑", 90);//能不能这么用？屏幕大小如果变了，这个字体也不一样了
    checkpointItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.88 + origin.y));
    checkpointItem->setScale(ratio);
    this->addChild(checkpointItem);
    
    auto targetItem = CCLabelTTF::create("目标分数：1000", "微软雅黑", 38);
    targetItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.84 + origin.y));
    targetItem->setScale(ratio);
    this->addChild(targetItem);
    
    auto yourscoreItem = CCLabelTTF::create("您的得分", "微软雅黑", 80);
    yourscoreItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75 + origin.y));
    yourscoreItem->setScale(ratio);
    this->addChild(yourscoreItem);
    
    //需要把分数传进来
   AppDelegate* app = (AppDelegate *)Application::getInstance();
    score = app->getScore();
    char scoreText[100];
    sprintf(scoreText, "%d", score);
    scoreBoard = Label::createWithTTF(scoreText, "Roboto-Thin.ttf", 180);
    scoreBoard->setString(scoreText);

    scoreBoard->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.68 + origin.y));
    scoreBoard->setScale(ratio);
    this->addChild(scoreBoard);
    
    auto victoryItem = MenuItemImage::create("胜利.png", "胜利.png",  CC_CALLBACK_1(GameOverScene::GoToGameScene, this));
    victoryItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.50 + origin.y));
    victoryItem->setScale(ratio);
    this->addChild(victoryItem);
    
    auto nextcheckpointItem = MenuItemImage::create("下一关.png", "下一关.png", CC_CALLBACK_1(GameOverScene::GoToGameScene, this));
    nextcheckpointItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.33 + origin.y));
    nextcheckpointItem->setScale(ratio);
    this->addChild(nextcheckpointItem);
    
    auto gotomainmenuItem = MenuItemImage::create("返回主菜单.png", "返回主菜单.png", CC_CALLBACK_1(GameOverScene::GoToMainMenuScene, this));
    gotomainmenuItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.20 + origin.y));
    gotomainmenuItem->setScale(ratio);
    this->addChild(gotomainmenuItem);
    
    auto menu = Menu::create(nextcheckpointItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    auto menu1 = Menu::create(gotomainmenuItem, NULL);
    menu1->setPosition(Point::ZERO);
    this->addChild(menu1);
    
    return true;
}

void GameOverScene::GoToMainMenuScene(cocos2d::Ref *sender)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameOverScene::GoToGameScene(cocos2d::Ref *sender)
{
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

