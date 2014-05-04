//
//  Role.cpp
//  HPaoKuGame
//
//  Created by DoubleHH on 14-3-6.
//
//

#include "Role.h"

Role::~Role() {
    
    CC_SAFE_RELEASE(goldArray);
    CC_SAFE_RELEASE_NULL(mParticleBatch);
    CC_SAFE_RELEASE(mCollisionArray);
}

bool Role::init() {
    if (!CCNode::init()) {
        return false;
    }
    
    return true;
}

void Role::extraInit() {
    //哪些东西需要retain//
    mParticleBatch = CCParticleBatchNode::create("goldEffect.png");
    CC_SAFE_RETAIN(mParticleBatch);
    getTiledMap()->addChild(mParticleBatch);
    
    mCollisionArray = CCArray::create();
    CC_SAFE_RETAIN(mCollisionArray);
    resetColissions();
    //主角人物//
    CCSpriteFrameCache *sharedFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    sharedFrameCache->addSpriteFramesWithFile("role_girl.plist");
    
    CCArray* animFrames = CCArray::createWithCapacity(6);
    char str[100] = {0};
    for(int i = 1; i < 7; i++)
    {
        sprintf(str, "girl_%02d.png", i);
        CCSpriteFrame *frame = sharedFrameCache->spriteFrameByName(str);
        animFrames->addObject(frame);
    }
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
    CCAnimate *anim = CCAnimate::create(animation);
    
    mSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame *)animFrames->objectAtIndex(0));
    mSprite->setAnchorPoint(CCPointZero);
    getTiledMap()->addChild(mSprite, ROLE_ZORDER);
    resetRoleSprite();
    mSprite->runAction(CCRepeatForever::create(anim));
    
    setRoleState(ROLE_STATE_RUN);
    setMapPointByRole(getTiledMap(), mSprite);
    
    this->schedule(schedule_selector(Role::roleUpdate), 0.016f);
    this->schedule(schedule_selector(Role::cameraUpdate), 0.016f);
    
}

void Role::resetColissions() {
    mCollisionArray->removeAllObjects();
    
    CCTMXObjectGroup *collisionGroup = getTiledMap()->objectGroupNamed("Collision");
    CCArray *collisions = collisionGroup->getObjects();
    mCollisionArray->addObjectsFromArray(collisions);
    //std::sort(mCollisionArray->data->arr, mCollisionArray->data->arr + mCollisionArray->data->num, Role::collisionCompare);
    for (int i=0; i<mCollisionArray->count(); ++i) {
        CCDictionary *dic = (CCDictionary *)mCollisionArray->objectAtIndex(i);
        CCLOG("sorted x: %f", dic->valueForKey("x")->floatValue());
    }
	//也没有setPosition//
}
//就是设置位置//
void Role::resetRoleSprite() {
    CCTMXObjectGroup *roleGroup = getTiledMap()->objectGroupNamed("Role");
    
    CCDictionary *playerDic = roleGroup->objectNamed("player");
    float x = playerDic->valueForKey("x")->floatValue();
    float y = playerDic->valueForKey("y")->floatValue();
    mSprite->setPosition(ccp(x, y));
}

Role *Role::create(cocos2d::CCTMXTiledMap *map) {
    Role *role = new Role();
    if (role && role->init()) {
        
        role->setTiledMap(map);
        role->extraInit();
        role->autorelease();
        return role;
    }
    
    CC_SAFE_DELETE(role);
    return NULL;
}

void Role::onEnter() {
    CCNode::onEnter();
    this->schedule(schedule_selector(Role::goldUpdate), 0.016f);
}


void Role::playAgain() {
    
    this->reset();
}

void Role::reset() {
    
    isWin = false;
    resetColissions();
    resetRoleSprite();
    setRoleState(ROLE_STATE_RUN);
}
//这几个状态关系真混乱//
void Role::jumpjump() {
    if (jumpCount < 2 && (getRoleState() == ROLE_STATE_JUMP_UP || getRoleState() == ROLE_STATE_JUMP_DOWN)) {
        setRoleState(ROLE_STATE_JUMP_UP);
    }
}
//这也单独一个函数，累不累//
void Role::setCurrentPosition(cocos2d::CCPoint var) {
    mSprite->setPosition(var);
}

CCPoint Role::getCurrentPosition() {
    return mSprite->getPosition();
}

bool Role::getIsWin() {
    return isWin;
}

//#pragma mark - RoleState
void Role::setRoleState(ROLE_STATE var) {
    
    switch (var) {
        case ROLE_STATE_STANDBY: {
            jumpCount = 0;
            xSpeed = 0;
            ySpeed = 0;
            break;
        }
        case ROLE_STATE_RUN: {
            jumpCount = 0;
            xSpeed = getConstXSpeed();
            ySpeed = 0;
            break;
        }
        case ROLE_STATE_JUMP_UP: {
            if (ROLE_STATE_JUMP_UP == roleState) {
                jumpCount = 2;
            }
            else if(ROLE_STATE_JUMP_DOWN == roleState) {
                jumpCount = 2;
            }
            else {
                jumpCount = 1;
            }
            xSpeed = getConstXSpeed();//这个speed的变化规律是？//
            ySpeed = getConstYSpeed();
            break;
        }
        case ROLE_STATE_JUMP_DOWN: {
            xSpeed = getConstXSpeed();
            ySpeed = 0;
            break;
        }
        case ROLE_STATE_FREE_FALL: {
            jumpCount = 0;
            xSpeed = 0;
            ySpeed = getConstYSpeed();
            break;
        }
        default:
            break;
    }
    roleState = var;
}

ROLE_STATE Role::getRoleState() {
    return roleState;
}

void Role::setGoldArray(cocos2d::CCArray *var) {
    if (goldArray != var) {
        var->retain();
        goldArray->release();
        goldArray = var;
    }
}

CCArray *Role::getGoldArray() {
    
    if (!goldArray) {
        goldArray = CCArray::create();
        CC_SAFE_RETAIN(goldArray);
    }
    return goldArray;
}

void Role::roleUpdate(float dt) {
    
    this->updateCollisions();
    switch (getRoleState()) {
        case ROLE_STATE_STANDBY: {
            this->roleStandbyLogic();
            break;
        }
        case ROLE_STATE_RUN: {
            this->roleRunLogic();
            break;
        }
        case ROLE_STATE_JUMP_UP: {
            this->roleJumpUpLogic();
            break;
        }
        case ROLE_STATE_JUMP_DOWN: {
            this->roleJumpDownLogic();
            break;
        }
        case ROLE_STATE_FREE_FALL: {
            this->roleFreeFallLogic();
            break;
        }
        default:
            break;
    }
    
    if (getCurrentPosition().y < -10 && getRoleState() != ROLE_STATE_STANDBY && !isWin) {
        CCMessageBox("Game over", "Game over");
        setRoleState(ROLE_STATE_STANDBY);
        return;
    }
    
    if (!isWin) {
        if (mCollisionArray->count() == 0) {
            isWin = true;
        }
        if (isWin) {
            CCMessageBox("Good Job", "Good Job");
            setRoleState(ROLE_STATE_STANDBY);
        }
    }
}

//空的这么坑爹//
void Role::roleStandbyLogic() {
    
}

void Role::roleRunLogic() {
    
    /**
     
     1.先判断向右的方向
     2.判断向下方向
     
     **/
    
    CCRect collisionBox = getCurrentCollisionBox();
    
    // 向右撞到墙体//
    if (isCollisionArea(COLL_STATE_RIGHT, collisionBox)) {
        
        //CCLOG("切换到静止状态");
        this->setRoleState(ROLE_STATE_STANDBY);
        return;
    }
    
    // 如果没有与地接触，则切换到下落状态//
    if (!isCollisionArea(COLL_STATE_BOTTOM, collisionBox)) {
        
        //CCLOG("切换到下落状态");
        setRoleState(ROLE_STATE_JUMP_DOWN);
        return;
    }
    
    CCPoint loc = getCurrentPosition();
    this->setCurrentPosition(ccpAdd(loc, CCPoint(getXSpeed(), 0)));
}

void Role::roleJumpUpLogic() {
    
    /**
     1.先判断向上的碰撞
     2.判断向右的碰撞
     3.判断速度是否为零
     
     **/
    
    CCRect collisionBox = getCurrentCollisionBox();
    
    if (this->isCollisionArea(COLL_STATE_TOP, collisionBox)) {
        
        //CCLOG("向上碰到物体，切换到下落状态");
        setRoleState(ROLE_STATE_JUMP_DOWN);
        return;
    }
    
    if (this->isCollisionArea(COLL_STATE_RIGHT, collisionBox)) {
        
        //CCLOG("向右碰到物体，切换到自由落体状态");
        setRoleState(ROLE_STATE_FREE_FALL);
        return;
    }
    
    // 如果速度为零，切换到下落状态//
    if (ySpeed <= 0) {
        ySpeed = 0;
        setRoleState(ROLE_STATE_JUMP_DOWN);
        return;
    }
    
    CCPoint loc = mSprite->getPosition();
    loc.x += xSpeed;
    loc.y += ySpeed;
    mSprite->setPosition(loc);
    
    ySpeed -= accelerate;//这里y速度变化//
}

void Role::roleJumpDownLogic() {
    // coding
    
    /**
     1.先判断向下的碰撞
     2.判断向右的碰撞
     
     **/
    
    CCRect collisionBox = getCurrentCollisionBox();
    
    if (this->isCollisionArea(COLL_STATE_BOTTOM, collisionBox)) {
        
        //CCLOG("向下碰到物体，切换到跑动状态");
        setRoleState(ROLE_STATE_RUN);
        return;
    }
    
    if (this->isCollisionArea(COLL_STATE_RIGHT, collisionBox)) {
        
        //CCLOG("向右碰到物体，切换到自由落体状态");
        setRoleState(ROLE_STATE_FREE_FALL);
        return;
    }
    
    CCPoint loc = mSprite->getPosition();
    loc.x += xSpeed;
    loc.y -= ySpeed;
    setCurrentPosition(loc);
    
    ySpeed += accelerate;
    ySpeed = MIN(10, ySpeed);
}

void Role::roleFreeFallLogic() {
    
    /**
     
     1.先判断主角是否挂掉（是否调到屏幕外），若是，提示游戏结束
     2.判断是否落到地面上
     
     **/
    
    if (getCurrentPosition().y <= 0 && !isWin) {
        // 主角挂了//
        CCMessageBox("Game over!", "Game over!");
        setRoleState(ROLE_STATE_STANDBY);
        return;
    }
    
//    CCTMXObjectGroup *group = getTiledMap()->objectGroupNamed("Collision");
//    CCDictionary *groundDic = group->objectNamed("ground");
//    float x = groundDic->valueForKey("x")->floatValue();
//    float y = groundDic->valueForKey("y")->floatValue();
//    float width = groundDic->valueForKey("width")->floatValue();
//    float height = groundDic->valueForKey("height")->floatValue();
//    CCRect collisionBox = CCRect(x, y, width, height);
    
    CCRect collisionBox = getCurrentCollisionBox();
    
    // coding
    if (isCollisionArea(COLL_STATE_BOTTOM, collisionBox)) {
        
        //CCLOG("掉到地面，切换到跑动状态");
        setRoleState(ROLE_STATE_RUN);
        return;
    }
    
    CCPoint loc = getCurrentPosition();
    loc.y -= ySpeed;
    setCurrentPosition(loc);
}

//#pragma mark - Camera
void Role::cameraUpdate(float dt) {
    
    setMapPointByRole(getTiledMap(), mSprite);
}

/**
 
 移动摄像头位置：
 x方向：
        1.若小于屏幕的四分之一，则地图在零的位置
        2.若大于（整个地图减屏幕的四分之一）则取（整个地图减屏幕的四分之一）
        3.否则，用目标的x值减去（屏幕的四分之一）
 y方向：同理，只不过是屏幕高度的三分之一而已
 
 **/
void Role::setMapPointByRole(cocos2d::CCTMXTiledMap *map, cocos2d::CCNode *node) {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSize mapTiledNum = map->getMapSize();
    CCSize tiledSize = map->getTileSize();
    CCSize mapSize = CCSize(mapTiledNum.width * tiledSize.width, mapTiledNum.height * tiledSize.height);
    
    CCPoint rolePosition = node->getPosition();
    
    float x = 0;
    float y = 0;
    float xOffset = visibleSize.width * 0.25;
    float yOffset = visibleSize.height / 3;
    if (rolePosition.x < xOffset) {
        x = 0;
    }
    else if (rolePosition.x > mapSize.width - xOffset) {
        x = mapSize.width - visibleSize.width;
    }
    else {
        x = rolePosition.x - xOffset;
    }
    
    if (rolePosition.y < yOffset) {
        y = 0;
    }
    else if (rolePosition.y > mapSize.height - yOffset) {
        y = mapSize.height - visibleSize.height;
    }
    else {
        y = rolePosition.y - yOffset;
    }
    
    // 因为是Map移动所以是负值//
	CCLOG("map= %f, %f", x, y);
    map->setPosition(CCPoint(-x, -y));
}

//#pragma mark - Collision
bool Role:: isCollisionArea(COLL_STATE state, cocos2d::CCRect box) {
    bool bFlag = false;
    
    switch (state) {
        case COLL_STATE_LEFT: {
            bFlag = isCollisionLeft(box);
            break;
        }
        case COLL_STATE_RIGHT: {
            bFlag = isCollisionRight(box);
            break;
        }
        case COLL_STATE_TOP: {
            bFlag = isCollisionTop(box);
            break;
        }
        case COLL_STATE_BOTTOM: {
            bFlag = isCollisionBottom(box);
            break;
        }
        default:
            break;
    }
    
    if (bFlag) {
        fixCollision(state, box);
    }
    
    return bFlag;
}

bool Role::isCollisionLeft(cocos2d::CCRect box) {
    
    CCRect roleBox = mSprite->boundingBox();
    CCPoint targetPoint = ccp(roleBox.getMinX(), roleBox.getMidY());
    return box.containsPoint(targetPoint);
}

bool Role::isCollisionRight(cocos2d::CCRect box) {
    
    CCRect roleBox = mSprite->boundingBox();
    CCPoint targetPoint = ccp(roleBox.getMaxX(), roleBox.getMidY());
    return box.containsPoint(targetPoint);
}

bool Role::isCollisionBottom(cocos2d::CCRect box) {
    
    CCRect roleBox = mSprite->boundingBox();
    CCPoint targetPoint = ccp(roleBox.getMidX(), roleBox.getMinY());
    return box.containsPoint(targetPoint);
}

bool Role::isCollisionTop(cocos2d::CCRect box) {
    
    CCRect roleBox = mSprite->boundingBox();
    CCPoint targetPoint = ccp(roleBox.getMidX(), roleBox.getMaxY());
    return box.containsPoint(targetPoint);
}

void Role::fixCollision(COLL_STATE state, cocos2d::CCRect box) {
    CCRect roleBox = mSprite->boundingBox();
    
    switch (state) {
        case COLL_STATE_LEFT: {
            float fixedX = mSprite->getPositionX();
            fixedX += box.getMaxX() - roleBox.getMinX();
            mSprite->setPositionX(fixedX);
            break;
        }
        case COLL_STATE_RIGHT: {
            float fixedX = mSprite->getPositionX();
            fixedX -= roleBox.getMaxX() - box.getMinX();
            mSprite->setPositionX(fixedX);
            break;
        }
        case COLL_STATE_TOP: {
            float fixedY = mSprite->getPositionY();
            fixedY -= roleBox.getMaxY() - box.getMinY();
            mSprite->setPositionY(fixedY);
            break;
        }
        case COLL_STATE_BOTTOM: {
            float fixedY = mSprite->getPositionY();
            fixedY += box.getMaxY() - roleBox.getMinY();
            mSprite->setPositionY(fixedY);
            break;
        }
        default:
            break;
    }
}

int Role::collisionCompare(cocos2d::CCObject *obj1, cocos2d::CCObject *obj2) {
    CCDictionary *dic1 = (CCDictionary *)obj1;
    CCDictionary *dic2 = (CCDictionary *)obj2;
    float x1 = dic1->valueForKey("x")->floatValue();
    float x2 = dic2->valueForKey("x")->floatValue();
    return x1 < x2;
}

void Role::updateCollisions() {
    
    CCPoint rolePosition = getCurrentPosition();
    
    for (int i=0; i<mCollisionArray->count(); ++i) {
        CCDictionary *dic = (CCDictionary *)mCollisionArray->objectAtIndex(i);
        float lx = dic->valueForKey("x")->floatValue();
        float ly = lx + dic->valueForKey("width")->floatValue();
        if (rolePosition.x > ly) {
            mCollisionArray->removeObjectAtIndex(i);
            --i;
        } else {
            if ( !(lx == getCurrentCollisionBox().origin.x && ly - lx == getCurrentCollisionBox().size.width) ) {
                
                float y = dic->valueForKey("y")->floatValue();
                float width = ly - lx;
                float height = dic->valueForKey("height")->floatValue();
                setCurrentCollisionBox(CCRect(lx, y, width, height));
            }
            break;
        }
    }
    
    if (!mCollisionArray->count()) {
        setCurrentCollisionBox(CCRectZero);
    }
}

void Role::setCurrentCollisionBox(cocos2d::CCRect var) {
    hhPrintRect(var, "Current Box");
    currentCollisionBox = var;
}

CCRect Role::getCurrentCollisionBox() {
    return currentCollisionBox;
}

#pragma mark - Gold
void Role::goldUpdate(float dt) {
    collisionWithGold();
}

void Role::collisionWithGold() {
    
    CCRect roleBox = mSprite->boundingBox();
    for (int i=0; i<getGoldArray()->count(); ++i) {
        Gold *gold = (Gold *)(getGoldArray()->objectAtIndex(i));
        if (gold->getGoldSprite() && roleBox.intersectsRect(gold->getGoldSprite()->boundingBox())) {
            
            // 碰撞效果//
            CCParticleSystemQuad *quad = CCParticleSystemQuad::create("goldEffect.plist");
            quad->setPosition(gold->getGoldSprite()->getPosition());
            quad->setAnchorPoint(CCPointZero);
            quad->setAutoRemoveOnFinish(true);
            mParticleBatch->addChild(quad);
            
            // 移除碰撞金币//
            gold->getGoldSprite()->removeFromParentAndCleanup(true);
            gold->setGoldSprite(NULL);
            getGoldArray()->removeObjectAtIndex(i);
            --i;
        }
    }
}
