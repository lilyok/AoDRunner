#ifndef __Runner_SCENE_H__
#define __Runner_SCENE_H__

#include "cocos2d.h"

class Runner : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual void onEnter();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void update(float delta) override;
    bool onContactBegin(const cocos2d::PhysicsContact& contact);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Runner);
    
    cocos2d::Vector<cocos2d::Sprite*> heavenmen;
    cocos2d::Vector<cocos2d::Sprite*> hellmen;
    cocos2d::Vector<cocos2d::Sprite*> clouds;
    cocos2d::Vector<cocos2d::Sprite*> fires;
    
    
    cocos2d::Sprite* mysprite;
    cocos2d::Animate* animateDaemonGoing;
    cocos2d::Animate* animateAngelGoing;
    cocos2d::Animate* animateDaemonFlying;
    cocos2d::Animate* animateAngelFlying;
    
    cocos2d::Label* score_label_black;
    cocos2d::Label* score_label_white;
    float time = 0;
    int score = 0;
    
    
    float scale_hero = 1.0;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    float blockWidth;
    float hideSize = 0;
    
    int numOfHeaven = 0;
    bool isHell = false;
    bool isRunning = true;
    bool isStop = false;
    
    void prepareHero();
    void prepareHeaven();
    void prepareHell();
    void prepareClouds();
    void prepareFires();
    void stopMen();
};

#endif // __Runner_SCENE_H__