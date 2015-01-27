#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

//static int score;

class GameOverScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    int score = 0;
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);
    
private:
    cocos2d::Label *scoreBoard;
    void GoToGameScene(cocos2d::Ref *sender);
    void GoToMainMenuScene(cocos2d::Ref *sender);
    string bgPath;
};

#endif // __GAME_OVER_SCENE_H__
