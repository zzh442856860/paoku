//
//  GameFatherLayer.cpp
//  HPaoKuGame
//
//  Created by DoubleHH on 14-3-6.
//
//

#include "GameFatherLayer.h"

GameFatherLayer::GameFatherLayer(){
    
}

GameFatherLayer::~GameFatherLayer() {
    
    CC_SAFE_RELEASE(mTMXTiledMap);
    CC_SAFE_RELEASE(mRole);
    CC_SAFE_RELEASE(mGoldBatch);
}

CCScene *GameFatherLayer::scene() {
    
    CCScene *scene = CCScene::create();
    GameFatherLayer *layer = GameFatherLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameFatherLayer::init() {
    
    if (!CCLayer::init()) {
        return false;
    }
    
    setTouchEnabled(true);
    
    initMap();
    initRole();
    initCompoments();
    //这么坑b。只是一个不服再来？专门写了个button？//
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    BBButton *button = BBButton::create(CCSizeMake(100, 50), this, bbButtonFunI_selector(GameFatherLayer::buttonCallbackAgain), NULL, NULL, NULL, NULL, "不服再来");
    button->mLabel->setFontSize(25);
    button->setPosition(CCPoint(size.width - 100, size.height - 50));
    button->mDependedTarget = this;
    this->addChild(button, 10);
    
    return true;
}

void GameFatherLayer::buttonCallbackAgain(int tag) {
    resetCompoments();
    mRole->playAgain();
}

void GameFatherLayer::initMap() {
	//什么情况是需要retain的//
    mTMXTiledMap = CCTMXTiledMap::create("game_map_1.tmx");
    mTMXTiledMap->retain();
    this->addChild(mTMXTiledMap);
}

void GameFatherLayer::initRole() {
    mRole = Role::create(mTMXTiledMap);
    this->addChild(mRole);
}

void GameFatherLayer::initCompoments() {
    
    mGoldBatch = CCSpriteBatchNode::create("gold.png");
    CC_SAFE_RETAIN(mGoldBatch);
    mTMXTiledMap->addChild(mGoldBatch);
    
    resetCompoments();
}

void GameFatherLayer::resetCompoments() {
    
    for (int i=0; i<mRole->getGoldArray()->count(); ++i) {
        Gold *gold = (Gold *)mRole->getGoldArray()->objectAtIndex(i);
        gold->getGoldSprite()->removeFromParentAndCleanup(true);
    }
    mRole->getGoldArray()->removeAllObjects();
    
    CCTMXObjectGroup *group = mTMXTiledMap->objectGroupNamed("Items");
    CCArray *items = group->getObjects();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(items, obj) {
        CCDictionary *dic = (CCDictionary *)obj;
        if ( 0 == dic->valueForKey("name")->compare("gold") ) {
            float x = dic->valueForKey("x")->floatValue();
            float y = dic->valueForKey("y")->floatValue();
            CCSprite *sp = CCSprite::create("gold.png");
            sp->setAnchorPoint(CCPointZero);
            sp->setPosition(ccp(x, y));
            mGoldBatch->addChild(sp);
            
            
            Gold *gold = Gold::create();
            gold->setGoldSprite(sp);
            mRole->getGoldArray()->addObject(gold);
        }
    }
}

//#pragma mark - Touch
void GameFatherLayer::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameFatherLayer::onExit() {
    CCNode::onExit();
}

bool GameFatherLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    this->dispatchTouchDispatcher();
    return true;
}
//游戏逻辑在这里//
void GameFatherLayer::dispatchTouchDispatcher() {
    
    if (mRole->getRoleState() == ROLE_STATE_RUN) {
        mRole->setRoleState(ROLE_STATE_JUMP_UP);
    } else {
        mRole->jumpjump();
    }
}


