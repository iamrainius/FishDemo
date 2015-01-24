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

USING_NS_CC;
using namespace std;

typedef Fish* PFish;

FishPool::FishPool(Size& visibleSize, int cols, int rows, cocos2d::Layer* layer)
{
    initPool(visibleSize, cols, rows, layer);
    setupFrames();
}

FishPool* FishPool::create(Size& visibleSize, int cols, int rows, cocos2d::Layer *layer)
{
    FishPool* fp = new FishPool(visibleSize, cols, rows, layer);
    return fp;
}


void FishPool::ProcessTouch(cocos2d::Point location)
{
    for (int i = 0; i < rows * cols; i++) {
        if (fishes[i] == NULL) {
            continue;
        }
        
        Point pos = fishes[i]->GetFishPosition();
        Rect rect = Rect(pos.x - fishSize / 2, pos.y - fishSize / 2, fishSize, fishSize);
        
        if (rect.containsPoint(location)) {
            RemoveContinuousFishes(i);
            break;
        }
    }
}


void FishPool::RemoveContinuousFishes(int fishIndex)
{
    vector<int> continuous;
    
    int cursor = 0;
    continuous.push_back(fishIndex);
    
    while (cursor < continuous.size()) {
        
        vector<int> neibours = GetNeibours(continuous.at(cursor));
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
    
    // 从场景中移除点中的鱼
    for (int j = 0; j < continuous.size(); j++) {
        
        auto fish = fishes[continuous.at(j)];
        auto fishAnimation = Animation::createWithSpriteFrames(fishFrames[fish->type],0.018f);
        auto animate = Animate::create(fishAnimation);
        auto func = CallFunc::create(CC_CALLBACK_0(FishPool::funCallback, this, j, continuous));
        auto sequence = Sequence::create(animate, func, NULL);
        fish->fishSprite->setLocalZOrder(200);
        fish->fishSprite->runAction(sequence);
    }
    
}

void FishPool::funCallback(int index, std::vector<int>& fs)
{
    log("funCallback: index=%d, size=%d", index, (int) fs.size());
    removeFish(fs[index]);
    
    if (index == fs.size() - 1) {
        log("funCallback: fall");
        fall(fs);
    }
}

void FishPool::fall(std::vector<int> fs)
{
    // 计算下落
    
    // 先构建一个队列
    queue<int> spaces;
    set<int> pool;
    for (int i = 0; i < fs.size(); i++)
    {
        spaces.push(fs.at(i));
        pool.insert(fs.at(i));
    }
    
    set<int> toFall;
    
    while (!spaces.empty()) {
        int index = spaces.front();
        spaces.pop();
        
        // 检查上方是否也是待删除单元
        Vec2 curPos = FindPosition(index);
        int aboveIndex = FindIndex(curPos.x, curPos.y + 1);
        
        if (aboveIndex < 0) {
            pool.erase(index);
            continue;
        }
        
        if(std::find(pool.begin(), pool.end(), aboveIndex) != pool.end()) {
            spaces.push(index);
        } else {
            vector<int> aboves = getAbove(index);
            
            if (aboves.size() <= 1)
            {
                pool.erase(index);
                continue;
            }
            
            for (int j = 0; j < aboves.size() - 1; j++) {
                int aboveIndex = aboves.at(j);
                fishes[aboveIndex] = fishes[aboves.at(j + 1)];
                fishes[aboves.at(j + 1)] = NULL;
                
                if (fishes[aboveIndex] != NULL) {
                    Vec2 logicPos = FindPosition(aboveIndex);
                    Vec2 pos(logicPos.x * fishSize + fishSize / 2, logicPos.y * fishSize + fishSize / 2 + visibleSize.height * 0.18);
                    fishes[aboveIndex]->SetTarget(pos.x, pos.y);
                    toFall.insert(aboveIndex);
                }
                
            }
            pool.erase(index);
            
        }
        
    }
    
    log("Falling size: %d", (int) toFall.size());
    
    for (set<int>::iterator it = toFall.begin(); it != toFall.end(); it++) {
        
        Fish* ff = fishes[*it];
        if (ff != NULL) {
            ff->MoveToTarget();
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

vector<int> FishPool::GetNeibours(int index)
{
    vector<int> neibours;
    Vec2 pos = FindPosition(index);
    
    int neibour;
    neibour = FindIndex(pos.x + 1, pos.y);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    neibour = FindIndex(pos.x - 1, pos.y);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    neibour = FindIndex(pos.x, pos.y + 1);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
    }
    
    neibour = FindIndex(pos.x, pos.y - 1);
    if (neibour >= 0 && fishes[neibour] != NULL && (fishes[neibour]->GetType() == fishes[index]->GetType())) {
        neibours.push_back(neibour);
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
            Vec2 pos(col * size + size / 2, row * size + size / 2 + visibleSize.height * 0.18);
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
}


