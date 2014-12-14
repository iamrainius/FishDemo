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
//    auto scene = Scene::createWithPhysics();
    auto scene = Scene::create();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    scene->getPhysicsWorld()->setGravity(Vect(0, -1000));

    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
//    layer->SetPhysicsWorld(scene->getPhysicsWorld());
    
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
    background->setColor(Color3B::GRAY);
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(background);
    
    fishPool = FishPool::create(visibleSize, HORIZONTAL_BLOCKS, VERTICAL_BLOCKS, this);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->scheduleUpdate();
    
    return true;
}


bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto target = event->getCurrentTarget();
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    // 点击范围判断检测
    if (rect.containsPoint(locationInNode))
    {
        log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
        fishPool->ProcessTouch(locationInNode);
        return true;
    }

    return false;
}


void GameScene::AddFish(cocos2d::Ref *sender)
{

}

//void GameScene::InitPhysicsEdge(const Size& size, const Vec2& pos)
//{
//    auto edgeBody = PhysicsBody::createEdgeBox(size, PHYSICSBODY_MATERIAL_DEFAULT, 3);
//    edgeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
//    edgeBody->setContactTestBitmask(true);
//    
//    auto edgeNode = Node::create();
//    edgeNode->setPosition(pos);
//    edgeNode->setPhysicsBody(edgeBody);
//    this->addChild(edgeNode);
//
//}

void GameScene::GoToGameOverScene(float dt)
{
    auto scene = GameOverScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}


//bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact)
//{
//    PhysicsBody *a = contact.getShapeA()->getBody();
//    PhysicsBody *b = contact.getShapeB()->getBody();
//    
//    if ((FISH_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask()) ||
//        (FISH_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
//    {
////        if (fish != NULL) {
////            a->setGravityEnable(false);
////        }
//    }
//    
//    return true;
//}

void GameScene::update(float dt)
{

}
