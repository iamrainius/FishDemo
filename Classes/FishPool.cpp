//
//  FishPool.cpp
//  FishDemo
//
//  Created by 张靖 on 14/12/13.
//
//

#include "FishPool.h"
#include "Fish.h"
#include "Definitions.h"
#include <queue>
#include <map>

USING_NS_CC;
using namespace std;

typedef Fish* PFish;

FishPool::FishPool(Size& visibleSize, int cols, int rows, cocos2d::Layer* layer)
{
    score = 0;
    initPool(visibleSize, cols, rows, layer);
    setupFrames();
}

FishPool* FishPool::create(Size& visibleSize, int cols, int rows, cocos2d::Layer *layer)
{
    FishPool* fp = new FishPool(visibleSize, cols, rows, layer);
    return fp;
}

Fish* FishPool::getTouchedFish(cocos2d::Point location)
{
    int fishIndex = findFishIndexByPoint(location);
    if (fishIndex >= 0) {
        return fishes[fishIndex];
    }
    
    return NULL;
}

int FishPool::findFishIndexByPoint(Point point)
{
    for (int i = 0; i < rows * cols; i++) {
        if (fishes[i] == NULL) {
            continue;
        }
        
        Point pos = fishes[i]->GetFishPosition();
        const float verticalSize = fishSize * VERTICAL_FACTOR;
        Rect rect = Rect(pos.x - fishSize / 2, pos.y - verticalSize / 2, fishSize, verticalSize);
        
        if (rect.containsPoint(point)) {
            return i;
        }
    }
    
    return -1;
}

vector<int> FishPool::findContinuousFishes(int index, int type)
{
    vector<int> continuous;
    
    int cursor = 0;
    continuous.push_back(index);
    
    while (cursor < continuous.size()) {
        
        vector<int> neibours = getNeibours(continuous.at(cursor), type);
        log("neibours size: %lu", neibours.size());
        
        for (int i = 0; i < neibours.size(); i++) {
            int index = neibours.at(i);
            if (!contains(continuous, index)) {
                continuous.push_back(index);
            }
        }
        
        log("Continuous size: %lu", continuous.size());
        cursor++;
    }
    
    return continuous;
}


bool FishPool::checkRemoveFishes(std::vector<int> seeds)
{
    map<int, vector<int>> toRemove;
    
    int totalRemove = 0;
    
    for (int i = 0; i < seeds.size(); i++) {
        
        int index = seeds.at(i);
        
        // 1. 当前index是否已经统计
        bool hasCounted = false;
        map<int, vector<int>>::iterator it;
        for (it = toRemove.begin(); it != toRemove.end(); it++) {
            if (contains(it->second, index)) {
                hasCounted = true;
            }
        }
        
        if (hasCounted) {
            continue;
        }
        
        // 2. 查找相邻的同类
        vector<int> continuous;
        int cursor = 0;
        continuous.push_back(index);
        while (cursor < continuous.size()) {
            
            vector<int> neibours = getNeibours(continuous.at(cursor));
            log("neibours size: %lu", neibours.size());
            
            for (int i = 0; i < neibours.size(); i++) {
                int index = neibours.at(i);
                if (!contains(continuous, index)) {
                    continuous.push_back(index);
                }
            }
            
            log("Continuous size: %lu", continuous.size());
            cursor++;
        }
        
        // 3. 相邻同类数量少于下限则忽略
        if (continuous.size() < MIN_DELETE_NUM) {
            continue;
        }
        
        // 4. 计分
        size_t size = continuous.size();
        if (size == 2) {
            score += 20;
        } else if (size == 3) {
            score += 60;
        } else if (size == 4) {
            score += 80;
        } else if (size >= 5) {
            score += 180;
        }
        
        onScoreUpdate(score);
        
        // 5. 将当前index对应的连续区域存入map
        toRemove.insert(pair<int, vector<int>>(index, continuous));
        totalRemove += continuous.size();
    }
    
    map<int, vector<int>>::iterator it;
    removeCount = 0;
    toFall.empty();
    
    for (it = toRemove.begin(); it != toRemove.end(); it++) {

        for (int j = 0; j < it->second.size(); j++) {
            toFall.push_back(it->second.at(j));
            auto fish = fishes[it->second.at(j)];
            auto fishAnimation = Animation::createWithSpriteFrames(fishFrames[fish->type],0.018f);
            auto animate = Animate::create(fishAnimation);
            auto func = CallFunc::create(CC_CALLBACK_0(FishPool::funCallback, this, j, it->second, totalRemove));
            auto sequence = Sequence::create(animate, func, NULL);
            fish->fishSprite->setLocalZOrder(200);
            fish->fishSprite->runAction(sequence);
            
//            auto bubbleAnimation = Animation::createWithSpriteFrames(bubbleFrames,0.02f);
//            auto bbAnimate = Animate::create(bubbleAnimation);
//            auto bbSeq = Sequence::create(bbAnimate, NULL, NULL);
//            fish->fishSprite->runAction(bbSeq);
        }
    }
    
    return toRemove.size() > 0;
}

void FishPool::CheckBlink( Fish* fish )
{
    if(fish != NULL)
    {
        auto blinkAnimation = Animation::createWithSpriteFrames(blinkFrames[fish->type],0.06f);
        auto animate = Animate::create(blinkAnimation);
        auto sequence = Sequence::create(animate, NULL, NULL);
        fish->fishSprite->runAction(sequence);
    }
}

void FishPool::blink(float percentage)
{
    //统计当前剩下鱼的数量
    int fishNum = rows * cols;
    for( int i=0; i< rows * cols; i++ )
    {
        if(fishes[i]==NULL)
        {
            fishNum--;
        }
    }
    
    log("fish number: %d", fishNum);
    
    //计算出要眨眼的鱼的数量
    if(fishNum * percentage>1){
        fishNum = fishNum * percentage;
        
    }else{
        fishNum = 1;
    }
    
    log("blink number: %d", fishNum);
    
    vector<Fish*> randomFishes = this->getRandomFishes(fishNum);
    
    for( int i=0; i<randomFishes.size(); i++ )
    {
        //执行眨眼
        this->CheckBlink(randomFishes[i]);
    }
    log("this is fishBlink function.");
}

int FishPool::GetFishNum()
{
    return rows * cols;
}

vector<Fish*> FishPool::getRandomFishes( int fishesNum )
{
    vector<Fish*> randomFishes;
    for (int i = 0; i < fishesNum; i++) {
        int index = random(0,cols*rows-1);
        vector<Fish*>::iterator it;
        for (it = randomFishes.begin(); it != randomFishes.end(); it++) {
            if (fishes[index] == *it) {
                break;
            }
        }
        
        if (it == randomFishes.end() && fishes[index] != NULL) {
            randomFishes.push_back(fishes[index]);
        }
    }
    
    return randomFishes;
}


void FishPool::CheckTouched( Fish* fish )
{
    if (fish!=NULL)
    {
        auto touchedAnimation = Animation::createWithSpriteFrames(touchedFrames[fish->type],0.06f);
        auto animate = Animate::create(touchedAnimation);
        auto sequence = Sequence::create(animate, NULL, NULL);
        fish->fishSprite->runAction(sequence);
    }
}


void FishPool::funCallback(int index, std::vector<int>& fs, int total)
{
    log("funCallback: index=%d, size=%d, count=%d, total=%d", index, (int) fs.size(), removeCount, total);
    
    removeFish(fs[index]);
    
    removeCount++;
    
    if (removeCount >= total) {
        log("funCallback: fall %d", (int) toFall.size());
        fall2(toFall);
    }
}

void FishPool::fall2(std::vector<int> fs)
{
    // 1. 找到存在下落的列
    set<int> columns;
    for (int i = 0; i < fs.size(); i++) {
        Vec2 pos = FindPosition(fs.at(i));
        if (pos.x >= 0 && pos.y < cols) {
            log("column needs falling: %f", pos.x);
            columns.insert(pos.x);
        }
    }
    
    for (set<int>::iterator it = columns.begin(); it != columns.end(); it++) {
        for (int j = 0; j < rows; j++) {
            int index = FindIndex(*it, j);
            if (fishes[index] == NULL) {
                continue;
            }
            
            int numEmpty = 0;
            for (int k = 0; k < j; k++) {
                int idx = FindIndex(*it, k);
                if (fishes[idx] == NULL) {
                    numEmpty++;
                }
            }
            
            if (numEmpty <= 0) {
                continue;
            }
            
            int swapIndex = FindIndex(*it, j - numEmpty);
            Fish* fish = fishes[index];
            fishes[index] = fishes[swapIndex];
            fishes[swapIndex] = fish;
            
            fish->fishPos.y -= numEmpty * fishSize * VERTICAL_FACTOR;
            fish->MoveToTarget();
        }
    }
    
}

vector<int> FishPool::getAbove(int index)
{
    vector<int> aboves;
    aboves.push_back(index);
    Vec2 pos = FindPosition(index);
    
    int up = 0;
    int count = 0;
    while ((up = FindIndex(pos.x, pos.y + (++count))) < cols * rows && up >= 0) {
        aboves.push_back(up);
    }
    
    return aboves;
}

void FishPool::removeFish(int index)
{
    if (fishes[index] == NULL) {
        return;
    }
    
    fishes[index]->QuitFromScene();
    delete fishes[index];
    fishes[index] = NULL;
}

vector<int> FishPool::getNeibours(int index)
{
//    vector<int> neibours;
//    Vec2 pos = FindPosition(index);
//    
//    int neibour;
//    neibour = FindIndex(pos.x + 1, pos.y);
//    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
//        neibours.push_back(neibour);
//    }
//    
//    neibour = FindIndex(pos.x - 1, pos.y);
//    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
//        neibours.push_back(neibour);
//    }
//    
//    neibour = FindIndex(pos.x, pos.y + 1);
//    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
//        neibours.push_back(neibour);
//    }
//    
//    neibour = FindIndex(pos.x, pos.y - 1);
//    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
//        neibours.push_back(neibour);
//    }
    
//    return neibours;
    int type = -1;
    
    if (index >= 0 && index < cols * rows)
    {
        auto fish = fishes[index];
        if (fish != NULL)
        {
            type = fish->type;
        }
    }

    return getNeibours(index, type);
}

vector<int> FishPool::getNeibours(int index, int type) {
    vector<int> neibours;
    
    if (index >= 0 && index < cols * rows)
    {
        auto fish = fishes[index];
        if (fish != NULL)
        {
            Vec2 pos = FindPosition(index);
            
            int neibour;
            neibour = FindIndex(pos.x + 1, pos.y);
            if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == type)) {
                neibours.push_back(neibour);
            }
            
            neibour = FindIndex(pos.x - 1, pos.y);
            if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == type)) {
                neibours.push_back(neibour);
            }
            
            neibour = FindIndex(pos.x, pos.y + 1);
            if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == type)) {
                neibours.push_back(neibour);
            }
            
            neibour = FindIndex(pos.x, pos.y - 1);
            if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == type)) {
                neibours.push_back(neibour);
            }
        }
    }
    
    
    return neibours;
}

int FishPool::FindIndex(int col, int row)
{
    if (col < 0 || col >= cols || row < 0 || row >= rows) {
        return -1;
    }
    
    return row * cols + col;
}

cocos2d::Vec2 FishPool::FindPosition(int index)
{
    if (index < 0 || index >= cols * rows) {
        return Vec2(-1, -1);
    }
    
    int row = index / cols;
    int col = index % cols;
    
    return cocos2d::Vec2(col, row);
}

void FishPool::PutFish(Fish *fish, int col, int row)
{
    int position = this->FindIndex(col, row);
    if (position < 0) {
        return;
    }
    
    Fish *currFish = fishes[position];
    if (currFish != NULL) {
        currFish->QuitFromScene();
        delete currFish;
    }
    
    fishes[position] = fish;
}

Fish* FishPool::GetFish(int col, int row)
{
    int position = FindIndex(col, row);
    if (position < 0) {
        return NULL;
    }
    
    return fishes[position];
}


bool FishPool::contains(vector<int>& v, int val)
{
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == val) {
            return true;
        }
    }
    
    return false;
}


int FishPool::vectorSize(vector<int>& v)
{
    return 1;
}


void FishPool::initPool(Size& visibleSize, int cols, int rows, cocos2d::Layer* layer)
{
    this->visibleSize = visibleSize;
    this->cols = cols;
    this->rows = rows;
    fishes = new PFish[cols * rows];
    
    for (int i = 0; i < cols * rows; i++)
    {
        Vec2 logicPos = FindPosition(i);
        if (logicPos.x >= 0 && logicPos.y >= 0) {
            int col = logicPos.x;
            int row = logicPos.y;
            
            float size = visibleSize.width / HORIZONTAL_BLOCKS;
            
            // 鱼的位置，以左下角为基准
            const float verticalSize = size * VERTICAL_FACTOR;
            Vec2 pos(col * size + size / 2, row * verticalSize + verticalSize / 2 + visibleSize.height * 0.18);
            fishSize = size;
            fishes[i] = new Fish(pos, size, layer);
        }
        
    }
}

void FishPool::setupFrames()
{
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("yellow.plist", "yellow.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("red.plist", "red.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("purple.plist", "purple.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blue.plist", "blue.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pink.plist", "pink.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bubbles.plist", "bubbles.png");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("yellow_touch.plist", "yellow_touch.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("red_touch.plist", "red_touch.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("purple_touch.plist", "purple_touch.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blue_touch.plist", "blue_touch.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pink_touch.plist", "pink_touch.png");
    
    char bbFilename[20];
    
    for (int j = 1; j <= 30; j++) {
        sprintf(bbFilename, "bubbleA_0%02d.png", j);
        log("%s", bbFilename);
        bubbleFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(bbFilename));
    }
    
    //鱼的眨眼动画
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blue_blink.plist", "blue_blink.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("red_blink.plist", "red_blink.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pink_blink.plist", "pink_blink.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("yellow_blink.plist", "yellow_blink.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("purple_blink.plist", "purple_blink.png");
    
    char blinkFile[20];
    string blinks[5] = {
        "blue_blink_000%02d.png",
        "red_blink_000%02d.png",
        "pink_blink_000%02d.png",
        "yellow_blink_000%02d.png",
        "purple_blink_000%02d.png"
    };
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            sprintf(blinkFile, blinks[i].c_str(), j);
            log("%s", blinkFile);
            blinkFrames[i].pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(blinkFile));
        }
    }

    //鱼的点击膨胀动画
    char filename[20];
    string patterns[5] = {
        "blue_000%02d.png",
        "red_000%02d.png",
        "pink_000%02d.png",
        "yellow_000%02d.png",
        "purple_000%02d.png"
    };
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 22; j++) {
            sprintf(filename, patterns[i].c_str(), j);
            log("%s", filename);
            fishFrames[i].pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
        }
    }
    
    string touchedPatterns[5] = {
        "blue_touch_000%02d.png",
        "red_touch_000%02d.png",
        "pink_touch_000%02d.png",
        "yellow_touch_000%02d.png",
        "purple_touch_000%02d.png"
    };
    
    for (int i = 0; i < 5; i++) {
        for (int j = 24; j < 42; j++) {
            sprintf(filename, touchedPatterns[i].c_str(), j);
            log("%s", filename);
            touchedFrames[i].pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
        }
    }
}

int FishPool::getFishSize()
{
    return fishSize;
}

Fish* FishPool::getNeibourFish(Fish *fish, int side) {
    
    if (fish == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < cols * rows; i++) {
        if (fish == fishes[i]) {
            Vec2 pos = FindPosition(i);
            int index = -1;
            if (side == 0) {
                pos.y += 1;
            } else if (side == 1) {
                pos.x += 1;
            } else if (side == 2) {
                pos.y -= 1;
            } else if (side == 3) {
                pos.x -= 1;
            }
            
            index = FindIndex(pos.x, pos.y);
            if (index >= 0) {
                return fishes[index];
            }
        }
    }
    
    return NULL;
}

bool FishPool::swapFishes(Fish *fish1, Fish *fish2)
{
    if (fish1 != NULL && fish2 != NULL && fish1->type == fish2->type) {
        return true;
    }
    
    int index1 = findFish(fish1);
    int index2 = findFish(fish2);
    
    if (index1 < 0 || index1 >= cols * rows || index2 < 0 || index2 >= cols * rows)
    {
        return false;
    }
    
    fishes[index1] = fish2;
    fishes[index2] = fish1;
    
    Vec2 tmpPos = fish1->fishPos;
    fish1->fishPos = fish2->fishPos;
    fish2->fishPos = tmpPos;
    
    return true;
}

int FishPool::findFish(Fish* fish)
{
    if (fish == NULL)
    {
        return -1;
    }
    
    for (int i = 0; i < cols * rows; i++) {
        if (fish == fishes[i]) {
            return i;
        }
    }
    
    return -1;
}
