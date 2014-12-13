#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

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
    
    void InitialBlocks(cocos2d::Size size);
    
    std::string colors[5] = { "block_blue.png","block_red.png","block_green.png","block_yellow.png","block_purple.png" };
};

#endif // __GAME_SCENE_H__
