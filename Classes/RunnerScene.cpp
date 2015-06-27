#include "RunnerScene.h"

USING_NS_CC;
#define HERO_SPRITE_TAG 888
#define ANIMATION_DELAY 0.04f
#define MY_VELOCITY 0.2
#define DAEMON 666
#define ANGEL 777
#define FLYING 555
#define HEAVENMEN 77
#define HELLMEN 66
#define ENDING 1000

Scene* Runner::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // 'layer' is an autorelease object
    auto layer = Runner::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Runner::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Runner::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    blockWidth = visibleSize.height / 20;
    
    auto rectNode = DrawNode::create();
    auto rectBlockNode = DrawNode::create();
    Vec2 whiteRect[4];
    whiteRect[0] = Vec2(0, visibleSize.height/2 + blockWidth);
    whiteRect[1] = Vec2(visibleSize.width, visibleSize.height/2 + blockWidth);
    whiteRect[2] = Vec2(visibleSize.width, visibleSize.height);
    whiteRect[3] = Vec2(0, visibleSize.height);
 
    Vec2 lilWhiteRect[4];
    lilWhiteRect[0] = Vec2(0, visibleSize.height/2 - blockWidth);
    lilWhiteRect[1] = Vec2(visibleSize.width, visibleSize.height/2 - blockWidth);
    lilWhiteRect[2] = Vec2(visibleSize.width, visibleSize.height/2);
    lilWhiteRect[3] = Vec2(0, visibleSize.height/2);
    
    Vec2 blackRect[4];
    blackRect[0] = Vec2(0, 0);
    blackRect[1] = Vec2(visibleSize.width, 0);
    blackRect[2] = Vec2(visibleSize.width, visibleSize.height/2);
    blackRect[3] = Vec2(0, visibleSize.height/2);
    
    Vec2 lilBlackRect[4];
    lilBlackRect[0] = Vec2(0, visibleSize.height/2 - blockWidth);
    lilBlackRect[1] = Vec2(visibleSize.width, visibleSize.height/2 - blockWidth);
    lilBlackRect[2] = Vec2(visibleSize.width, visibleSize.height/2 + blockWidth);
    lilBlackRect[3] = Vec2(0, visibleSize.height/2 + blockWidth);
    
    
    Color4F white(1, 1, 1, 1);
    Color4F black(0, 0, 0, 0);
    
    rectNode->drawPolygon(whiteRect, 4, white, 0, white);
    rectNode->drawPolygon(blackRect, 4, black, 0, black);
    rectBlockNode->drawPolygon(lilWhiteRect, 4, white, 0, white);
    rectBlockNode->drawPolygon(lilBlackRect, 4, black, 0, black);
    
    
    auto physicsBody = PhysicsBody::createBox(Size(visibleSize.width, 2*blockWidth),
                                              PhysicsMaterial(1.0f, 0.0f, 0.0f), Vec2(origin.x + visibleSize.width/2,
                                                                                      origin.y + visibleSize.height/2));
    physicsBody->setRotationEnable(false);
    physicsBody->setGravityEnable(false);
    physicsBody->setDynamic(false);
    rectBlockNode->setPhysicsBody(physicsBody);

    prepareHero();
    prepareHell();
    prepareHeaven();
    // add the sprite as a child to this layer
    this->addChild(rectNode, 0);
    this->addChild(rectBlockNode);
    
    this->scheduleUpdate();
    return true;
}


void Runner::prepareHell() {
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("daemonbot.plist");
    
    
    for (int i = 1; i <=3; i++) {
        char *res = new char[50];
        sprintf(res, "daemon01bot0%i.png", i);
        auto sprite = Sprite::createWithSpriteFrameName(res);
        auto h = sprite->getContentSize().height;
        auto scale_men = visibleSize.height/4/h;
        // position the sprite on the center of the screen
        sprite->setTag(HELLMEN);
        sprite->setPosition(Vec2(origin.x + visibleSize.width/4*3,
                                 origin.y - hideSize));
        sprite->setScale(scale_men);
        sprite->setFlippedY(true);
        
        Vector<SpriteFrame *> animHeavenFrames;
        animHeavenFrames.reserve(10);
        for (int j = 1; j <=6; j++) {
            sprintf(res, "daemon0%ibot0%i.png", j, i);
            animHeavenFrames.pushBack(spritecache->getSpriteFrameByName(res));
        }
        for (int j = 5; j >=2; j--) {
            sprintf(res, "daemon0%ibot0%i.png", j, i);
            animHeavenFrames.pushBack(spritecache->getSpriteFrameByName(res));
        }
        
        
        // create the animation out of the frames
        Animation *animationHeaven = Animation::createWithSpriteFrames(animHeavenFrames, ANIMATION_DELAY*3);
        auto animateHeaven = Animate::create(animationHeaven);
        
        
        auto physicsBody = PhysicsBody::createBox(Size(h * scale_hero, h * scale_hero),
                                                  PhysicsMaterial(1.0f, 0.0f, 0.0f), Vec2(0, 0));
        physicsBody->setRotationEnable(false);
        physicsBody->setDynamic(true);
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        //set the body isn't affected by the physics world's gravitational force
        physicsBody->setGravityEnable(false);
        
        sprite->setPhysicsBody(physicsBody);
        auto action = RepeatForever::create(animateHeaven);
        sprite->runAction(action);
        
        addChild(sprite, 1);
        hellmen.pushBack(sprite);
    }

}

void Runner::prepareHeaven() {
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("angelbot.plist");
    
    
    for (int i = 1; i <=3; i++) {
        char *res = new char[50];
        sprintf(res, "angel01bot0%i.png", i);
        auto sprite = Sprite::createWithSpriteFrameName(res);
        auto h = sprite->getContentSize().height;
        auto scale_men = visibleSize.height/4/h;
        // position the sprite on the center of the screen
        sprite->setTag(HEAVENMEN);
        sprite->setPosition(Vec2(origin.x + visibleSize.width/4*3,
                            origin.y + visibleSize.height + hideSize));
        sprite->setScale(scale_men);
        
        Vector<SpriteFrame *> animHeavenFrames;
        animHeavenFrames.reserve(10);
        for (int j = 1; j <=6; j++) {
            sprintf(res, "angel0%ibot0%i.png", j, i);
            animHeavenFrames.pushBack(spritecache->getSpriteFrameByName(res));
        }
        for (int j = 5; j >=2; j--) {
            sprintf(res, "angel0%ibot0%i.png", j, i);
            animHeavenFrames.pushBack(spritecache->getSpriteFrameByName(res));
        }

        
        // create the animation out of the frames
        Animation *animationHeaven = Animation::createWithSpriteFrames(animHeavenFrames, ANIMATION_DELAY*3);
        auto animateHeaven = Animate::create(animationHeaven);
        
        
        auto physicsBody = PhysicsBody::createBox(Size(h * scale_hero, h * scale_hero),
                                                    PhysicsMaterial(1.0f, 0.0f, 0.0f), Vec2(0, 0));
        physicsBody->setRotationEnable(false);
        physicsBody->setDynamic(true);
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        //set the body isn't affected by the physics world's gravitational force
        physicsBody->setGravityEnable(false);
        
        sprite->setPhysicsBody(physicsBody);
        auto action = RepeatForever::create(animateHeaven);
        sprite->runAction(action);
        
        addChild(sprite, 1);
        heavenmen.pushBack(sprite);
    }
}

void Runner::prepareHero() {
    // load the Sprite Sheet
    auto spritecache = SpriteFrameCache::getInstance();
    
    // the .plist file can be generated with any of the tools mentioned below
    spritecache->addSpriteFramesWithFile("aod.plist");
    
    this->mysprite = Sprite::createWithSpriteFrameName("daemon01.png");
    //    auto w = mysprite->getContentSize().width;
    auto h = mysprite->getContentSize().height;
    scale_hero = visibleSize.height/4/h;
    
    // position the sprite on the center of the screen
    mysprite->setTag(HERO_SPRITE_TAG);
    mysprite->setPosition(Vec2(origin.x + visibleSize.width/4,
                               origin.y + visibleSize.height/2 + blockWidth + h/2*scale_hero));
    mysprite->setScale(scale_hero);
    
    
    Vector<SpriteFrame *> animAngelGoingFrames;
    animAngelGoingFrames.reserve(10);
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel01.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel02.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel03.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel04.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel05.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel06.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel05.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel04.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel03.png"));
    animAngelGoingFrames.pushBack(spritecache->getSpriteFrameByName("angel02.png"));
    
    Vector<SpriteFrame *> animDaemonGoingFrames;
    animDaemonGoingFrames.reserve(10);
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon01.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon02.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon03.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon04.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon05.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon06.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon05.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon04.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon03.png"));
    animDaemonGoingFrames.pushBack(spritecache->getSpriteFrameByName("daemon02.png"));
    
    Vector<SpriteFrame *> animAngelFlyingFrames;
    animAngelFlyingFrames.reserve(2);
    animAngelFlyingFrames.pushBack(spritecache->getSpriteFrameByName("angelfly01.png"));
    animAngelFlyingFrames.pushBack(spritecache->getSpriteFrameByName("angelfly02.png"));
    
    Vector<SpriteFrame *> animDaemonFlyingFrames;
    animDaemonFlyingFrames.reserve(2);
    animDaemonFlyingFrames.pushBack(spritecache->getSpriteFrameByName("daemonfly01.png"));
    animDaemonFlyingFrames.pushBack(spritecache->getSpriteFrameByName("daemonfly02.png"));
    
    // create the animation out of the frames
    Animation *animationAngelGoing = Animation::createWithSpriteFrames(animAngelGoingFrames, ANIMATION_DELAY);
    animateAngelGoing = Animate::create(animationAngelGoing);
    animateAngelGoing->retain();
    
    Animation *animationDaemonGoing = Animation::createWithSpriteFrames(animDaemonGoingFrames, ANIMATION_DELAY);
    animateDaemonGoing = Animate::create(animationDaemonGoing);
    animateDaemonGoing->retain();
    
    Animation *animationAngelFlying = Animation::createWithSpriteFrames(animAngelFlyingFrames, ANIMATION_DELAY*10);
    animateAngelFlying = Animate::create(animationAngelFlying);
    animateAngelFlying->retain();
    
    Animation *animationDaemonFlying = Animation::createWithSpriteFrames(animDaemonFlyingFrames, ANIMATION_DELAY*10);
    animateDaemonFlying = Animate::create(animationDaemonFlying);
    animateDaemonFlying->retain();
    
    auto myPhysicsBody = PhysicsBody::createBox(Size(h * scale_hero, h * scale_hero),
                                                PhysicsMaterial(1.0f, 0.0f, 0.0f), Vec2(0, 0));
    myPhysicsBody->setRotationEnable(false);
    myPhysicsBody->setDynamic(true);
    myPhysicsBody->setContactTestBitmask(0xFFFFFFFF);
    //set the body isn't affected by the physics world's gravitational force
    myPhysicsBody->setGravityEnable(false);
    addChild(this->mysprite, 2);
    
    mysprite->setPhysicsBody(myPhysicsBody);
    auto action = RepeatForever::create(animateDaemonGoing);
    action->setTag(DAEMON);
    mysprite->runAction(action);
    hideSize = mysprite->getContentSize().height/2*scale_hero;
}

void Runner::onEnter() {
    Layer::onEnter();
    
    // Register Touch Event
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(Runner::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(AbstractLabirint::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(AbstractLabirint::onTouchEnded, this);
//    listener->onTouchCancelled = CC_CALLBACK_2(AbstractLabirint::onTouchCancelled, this);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Runner::onContactBegin,
                                                    this);
//
//    contactListener->onContactSeperate = CC_CALLBACK_1(AbstractLabirint::onContactSeperate,
//                                                       this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(contactListener,
                                                       this);
}

void Runner::update(float delta) {
    if (!isRunning) {
        if (!isStop)
            stopMen();
        
        else if(isStop and mysprite->getNumberOfRunningActions() <= 0) {
            auto isAllMenStop = true;
            for (auto h:hellmen)
                if (h->getActionByTag(ENDING)) {
                    isAllMenStop = false;
                }
            if (isAllMenStop) {
                for (auto h:heavenmen)
                    if (h->getActionByTag(ENDING)) {
                        isAllMenStop = false;
                    }
            }
            if (isAllMenStop) {
                for (auto h:hellmen)
                    h->stopAllActions();
                
                for (auto h:heavenmen)
                    h->stopAllActions();
            }
            
        }
    }
    else {
        if (mysprite->getActionByTag(FLYING)) {
            if (mysprite->getPositionY() == origin.y + visibleSize.height/2 + blockWidth +
                hideSize) {
                mysprite->stopAllActions();
                auto action = RepeatForever::create(animateDaemonGoing);
                action->setTag(DAEMON);
                mysprite->runAction(action);
                
            }
            else if (mysprite->getPositionY() == origin.y + visibleSize.height/2 - blockWidth -
                     hideSize) {
                mysprite->stopAllActions();
                auto action = RepeatForever::create(animateAngelGoing);
                action->setTag(ANGEL);
                mysprite->runAction(action);
                
            }
            else if (mysprite->getPositionY() <= -hideSize) {
                mysprite->stopAllActions();
                mysprite->setFlippedY(false);

                mysprite->setPosition(Vec2(origin.x + visibleSize.width/4, origin.y + visibleSize.height +
                                           mysprite->getContentSize().height*2/6*scale_hero));
                auto flyaction = MoveTo::create(MY_VELOCITY, Vec2(origin.x + visibleSize.width/4,
                                                        origin.y + visibleSize.height/2 + blockWidth +
                                                        hideSize));
                mysprite->runAction(flyaction);
                auto action = RepeatForever::create(animateDaemonFlying);
                action->setTag(FLYING);
                mysprite->runAction(action);

            }
            else if (mysprite->getPositionY()  >= visibleSize.height + hideSize) {
                mysprite->stopAllActions();
                mysprite->setFlippedY(true);
                mysprite->setPosition(Vec2(origin.x + visibleSize.width/4, origin.y - mysprite->getContentSize().height*2/6*scale_hero));
                
                auto flyaction = MoveTo::create(MY_VELOCITY, Vec2(origin.x + visibleSize.width/4,
                                                        origin.y + visibleSize.height/2 - blockWidth -
                                                        hideSize));
                mysprite->runAction(flyaction);
                auto action = RepeatForever::create(animateAngelFlying);
                action->setTag(FLYING);
                mysprite->runAction(action);
            }
        }
        if (isHell) {
            auto curSprite = hellmen.at(numOfHeaven);
            if (curSprite->getPositionY() <= origin.y - hideSize) {
                
                curSprite->setPosition(Vec2(origin.x + visibleSize.width,
                                            origin.y - curSprite->getContentSize().height*2/6*scale_hero));
                
                auto flyaction = Sequence::create(MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                                                     origin.y + visibleSize.height/2 - blockWidth -
                                                                                     hideSize)),
                                                  MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/4,
                                                                                     origin.y + visibleSize.height/2 - blockWidth -
                                                                                     hideSize)), NULL);
                flyaction->setTag(FLYING);
                curSprite->runAction(flyaction);
                
            }
            else if (curSprite->getPositionX()  == origin.x + visibleSize.width/4) {
                
                auto flyaction = MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                                    origin.y - hideSize));
                flyaction->setTag(FLYING);
                curSprite->runAction(flyaction);
                
             //   numOfHeaven = (numOfHeaven + 1) % 3;
                isHell = false;
            }
            
        } else {
            auto curSprite = heavenmen.at(numOfHeaven);
            if (curSprite->getPositionY() >= origin.y + visibleSize.height + hideSize) {
                
                curSprite->setPosition(Vec2(origin.x + visibleSize.width,
                                            origin.y + visibleSize.height + curSprite->getContentSize().height*2/6*scale_hero));
                
                auto flyaction = Sequence::create(MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                                                     origin.y + visibleSize.height/2 + blockWidth +
                                                                                     hideSize)),
                                                  MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/4,
                                                                                     origin.y + visibleSize.height/2 + blockWidth +
                                                                                     hideSize)), NULL);
                flyaction->setTag(FLYING);
                curSprite->runAction(flyaction);
                
            }
            else if (curSprite->getPositionX()  == origin.x + visibleSize.width/4) {
                
                auto flyaction = MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                                  origin.y + visibleSize.height + hideSize));
                flyaction->setTag(FLYING);
                curSprite->runAction(flyaction);
                
                numOfHeaven = (numOfHeaven + 1) % 3;
                isHell = true;
            }
        }
    }
}

bool Runner::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (isRunning and ! mysprite->getActionByTag(FLYING)){
        if (mysprite->getActionByTag(DAEMON)) {
            mysprite->stopActionByTag(DAEMON);
            mysprite->setPosition(Vec2(origin.x + visibleSize.width/4,
                                       origin.y + visibleSize.height/2 + blockWidth + hideSize + 1));
            auto action = RepeatForever::create(animateDaemonFlying);
            action->setTag(FLYING);
            mysprite->runAction(action);
            mysprite->runAction(MoveTo::create(MY_VELOCITY, Vec2(visibleSize.width/4, visibleSize.height + hideSize)));
        } else {
            mysprite->stopActionByTag(ANGEL);
            mysprite->setPosition(Vec2(origin.x + visibleSize.width/4,
                                       origin.y + visibleSize.height/2 - blockWidth - hideSize - 1));
            auto action = RepeatForever::create(animateAngelFlying);
            action->setTag(FLYING);
            mysprite->runAction(action);
            mysprite->runAction(MoveTo::create(MY_VELOCITY, Vec2(visibleSize.width/4, -hideSize)));

        }
    }
    
    return true;
}

bool Runner::onContactBegin(const cocos2d::PhysicsContact &contact) {
    if (isRunning) {
        auto nodeA = contact.getShapeA()->getBody()->getNode();
        auto nodeB = contact.getShapeB()->getBody()->getNode();
        
        if ((nodeA->getTag() == HERO_SPRITE_TAG and nodeB->getTag() == HEAVENMEN) or
            (nodeB->getTag() == HERO_SPRITE_TAG and nodeA->getTag() == HEAVENMEN)) {
            mysprite->stopAllActions();
            mysprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("daemondeath.png"));
            mysprite->runAction(MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                               origin.y + visibleSize.height/2 + blockWidth + hideSize)));
            isRunning = false;
            return false;
        } else if ((nodeA->getTag() == HERO_SPRITE_TAG and nodeB->getTag() == HELLMEN) or
                   (nodeB->getTag() == HERO_SPRITE_TAG and nodeA->getTag() == HELLMEN)) {
            mysprite->stopAllActions();
            mysprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("angeldeath.png"));
            mysprite->runAction(MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                                   origin.y + visibleSize.height/2 - blockWidth - hideSize)));
            isRunning = false;
            return false;
        } else if ((nodeA->getTag() == HEAVENMEN and nodeB->getTag() == HEAVENMEN) or
                   (nodeA->getTag() == HELLMEN and nodeB->getTag() == HELLMEN)) {
            return false;
        }
        return true;
    }
    return false;
}


void Runner::stopMen() {
    for (auto hellboy : hellmen) {
        if (hellboy->getPositionY() > origin.y - hideSize){
            auto action = MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                             origin.y - hideSize));
            action->setTag(ENDING);
            hellboy->stopActionByTag(FLYING);
            hellboy->runAction(action);
        } else {
            hellboy->stopAllActions();
        }
    }
    
        
    for (auto heavenboy : heavenmen) {
        if (heavenboy->getPositionY() < origin.y + visibleSize.height + hideSize) {
            auto action = MoveTo::create(MY_VELOCITY*5, Vec2(origin.x + visibleSize.width/2,
                                                             origin.y + visibleSize.height + hideSize));
            action->setTag(ENDING);
            heavenboy->stopActionByTag(FLYING);
            heavenboy->runAction(action);
        } else {
            heavenboy->stopAllActions();
        }
    }
    
    isStop = true;
    
}

void Runner::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
