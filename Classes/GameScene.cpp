#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameOverScene.h"
#include "Definitions.h"
#include "FishPool.h"

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
    
    auto background = Sprite::create("game_bg.png");
    const float ratio = visibleSize.width / background->getContentSize().width;
    background->setColor(Color3B::GRAY);
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    background->setScale(ratio);
    
    this->addChild(background);
    
    scoreBoard = Label::createWithTTF("Score: 0", "Roboto-Thin.ttf", 60);
    scoreBoard->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.9);
    this->addChild(scoreBoard);
    
    fishPool = FishPool::create(visibleSize, HORIZONTAL_BLOCKS, VERTICAL_BLOCKS, this);
    fishPool->onScoreUpdate = CC_CALLBACK_1(GameScene::onScoreUpdate, this);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->scheduleUpdate();

    
    return true;
}

void GameScene::onScoreUpdate(int score)
{
    char scoreText[100];
    sprintf(scoreText, "Score: %d", score);
    scoreBoard->setString(scoreText);
    
    if (score >= 300) {
        // 过关
        auto scene = GameOverScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
    }
}


bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    moveCount = 0;
    neibourFish = NULL;
    auto target = event->getCurrentTarget();
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    touchedPos = locationInNode;
    touchStarPos.x = touchedPos.x;
    touchStarPos.y = touchedPos.y;
    
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    // 点击范围判断检测
    if (rect.containsPoint(locationInNode))
    {
        log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
//        fishPool->ProcessTouch(locationInNode);
        touchedFish = fishPool->getTouchedFish(locationInNode);
        
        return true;
    }

    return false;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    moveCount++;
    if (moveCount <= 5) {
        return;
    }
    
    auto target = event->getCurrentTarget();
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    float deltaX = locationInNode.x - touchedPos.x;
    float deltaY = locationInNode.y - touchedPos.y;
    
    touchedPos.x = locationInNode.x;
    touchedPos.y = locationInNode.y;
    
    if (moveCount == 6) {
        touchStarPos.x = touchedPos.x;
        touchStarPos.y = touchedPos.y;
        if (abs(deltaY) > abs(deltaX)) {
            isHorizontal = false;
            if (deltaY < 0) {
                neibourFish = fishPool->getNeibourFish(touchedFish, 2);
            } else {
                neibourFish = fishPool->getNeibourFish(touchedFish, 0);
            }
        } else {
            isHorizontal = true;
            if (deltaX < 0) {
                neibourFish = fishPool->getNeibourFish(touchedFish, 3);
            } else {
                neibourFish = fishPool->getNeibourFish(touchedFish, 1);
            }
        }
        return;
    }
    
    if (touchedFish != NULL) {
        auto fishSprite = touchedFish->fishSprite;
        Vec2 pos = fishSprite->getPosition();
        
        const float fishSize = fishPool->getFishSize();
        if (isHorizontal) {
            float destX = pos.x + deltaX;
            
            if (abs(destX - touchedFish->fishPos.x) > fishSize) {
                destX = touchedFish->fishPos.x + (deltaX / abs(deltaX) * fishSize);
            }
            
            
            if (neibourFish != NULL) {
                Vec2 npos = neibourFish->fishSprite->getPosition();
                npos.x = npos.x - (destX - pos.x);
                neibourFish->fishSprite->setPosition(npos);
            }
            
            fishSprite->setPosition(Vec2(destX, pos.y));
            
        } else {
            float destY = pos.y + deltaY;
            if (abs(destY - touchedFish->fishPos.y) > fishSize) {
                destY = touchedFish->fishPos.y + (deltaY / abs(deltaY) * fishSize);
            }
            
            if (neibourFish != NULL) {
                Vec2 npos = neibourFish->fishSprite->getPosition();
                npos.y = npos.y - (destY - pos.y);
                neibourFish->fishSprite->setPosition(npos);
            }
            
            
            fishSprite->setPosition(Vec2(pos.x, destY));
        }
        
    }
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (touchedFish != NULL) {
        touchedFish->fishSprite->setPosition(touchedFish->fishPos);
    }
    
    if (neibourFish != NULL) {
        neibourFish->fishSprite->setPosition(neibourFish->fishPos);
    }
}

void GameScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (touchedFish != NULL) {
        touchedFish->fishSprite->setPosition(touchedFish->fishPos);
    }
    
    if (neibourFish != NULL) {
        neibourFish->fishSprite->setPosition(neibourFish->fishPos);
    }
}

void GameScene::AddFish(cocos2d::Ref *sender)
{

}

void GameScene::GoToGameOverScene(float dt)
{
    auto scene = GameOverScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameScene::update(float dt)
{

}
