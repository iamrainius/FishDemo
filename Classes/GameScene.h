#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Fish.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    void GoToGameOverScene(float dt);
    void AddFish(cocos2d::Ref *sender);
//    void InitPhysicsEdge(const cocos2d::Size& size, const cocos2d::Vec2& pos);
    void InitialBlocks(cocos2d::Size size);
    
//    void SetPhysicsWorld(cocos2d::PhysicsWorld *world) { sceneWorld = world; }
//    bool onContactBegin(cocos2d::PhysicsContact &contact);
//    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void update(float dt);
    
    std::string colors[5] = { "block_blue.png","block_red.png","block_green.png","block_yellow.png","block_purple.png" };
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
//    cocos2d::PhysicsWorld *sceneWorld;

};

#endif // __GAME_SCENE_H__
