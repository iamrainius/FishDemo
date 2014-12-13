#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameOverScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //this->scheduleOnce(schedule_selector(GameScene::GoToGameOverScene), 5);
    
    auto background = Sprite::create("mainmenu_bg.png");
    background->setColor(Color3B::GRAY);
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(background);
    
    InitialBlocks(visibleSize);
    
    
    return true;
}

void GameScene::GoToGameOverScene(float dt)
{
    auto scene = GameOverScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameScene::InitialBlocks(Size size)
{
    // calculate block size
    float blockSize = (size.width -  GAP_SIZE * ( HORIZONTAL_BLOCKS + 1 )) / HORIZONTAL_BLOCKS;
    
    // get visible origin of game scence
    Vec2 origin =  Director::getInstance()->getVisibleOrigin();
    
    // calculate anchor point
    float x0 = origin.x;
    float y0 = origin.y;
    
    for(int j = 0; j < VERTICAL_BLOCKS ; j++)
    {
        for(int i = 0 ; i < HORIZONTAL_BLOCKS ; i++)
        {
            // draw first block
            float blockOriginX = x0 + blockSize / 2 + GAP_SIZE + i*(blockSize + GAP_SIZE);
            float blockOriginY = y0 + blockSize / 2 + GAP_SIZE + j*(blockSize + GAP_SIZE);
        
            int blockColor = random(0, 4);

            auto block = Sprite::create(colors[blockColor]);
            block->setPosition(blockOriginX, blockOriginY);
            block->setScale(blockSize/120);
        
            this->addChild(block);
        }
    }
    
    
    
}

