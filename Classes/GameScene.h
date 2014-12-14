#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "FishPool.h"

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
    private FishPool* fishPool;
    
    void GoToGameOverScene(float dt);
    void AddFish(cocos2d::Ref *sender);
    void InitialBlocks(cocos2d::Size size);
    void update(float dt);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

};

#endif // __GAME_SCENE_H__
