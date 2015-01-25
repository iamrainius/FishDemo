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
    FishPool* fishPool;
    int score;
    
    cocos2d::Label *scoreBoard;
    
    void GoToGameOverScene(float dt);
    void AddFish(cocos2d::Ref *sender);
    void InitialBlocks(cocos2d::Size size);
    void update(float dt);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    void onScoreUpdate(int score);
    void onSwappingFinished1();
    void onSwappingFinished2();
    
    void updateSwapping(int which);
    
    void resetSwapping();
    
    Fish* touchedFish;
    
    Fish* neibourFish;
    
    bool touchedFishDone;
    bool neibourFishDone;
    
    cocos2d::Point touchedPos;
    
    cocos2d::Point touchStarPos;
    
    int moveCount;
    
    bool isHorizontal = false;
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};

#endif // __GAME_SCENE_H__
