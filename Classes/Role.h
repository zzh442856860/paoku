//
//  Role.h
//  HPaoKuGame
//
//  Created by DoubleHH on 14-3-6.
//
//

#ifndef __HPaoKuGame__Role__
#define __HPaoKuGame__Role__

// 主角状态//
typedef enum {
    ROLE_STATE_INVALID = 0,
    ROLE_STATE_STANDBY, // 静止站立//
    ROLE_STATE_RUN,     // 直线跑动//
    ROLE_STATE_JUMP_UP, // 跳起//
    ROLE_STATE_JUMP_DOWN,//跳起后下落抛物线//
    ROLE_STATE_FREE_FALL,//自由落体//
}ROLE_STATE;

typedef enum {
    COLL_STATE_INVALID = 0,
    COLL_STATE_LEFT,
    COLL_STATE_RIGHT,
    COLL_STATE_TOP,
    COLL_STATE_BOTTOM,
}COLL_STATE;

#include "GHeaders.h"

class Role : public CCNode {
public:
    Role():constXSpeed(3), constYSpeed(20), xSpeed(constXSpeed), ySpeed(constYSpeed), accelerate(0.9), goldArray(NULL), isWin(false), jumpCount(0) {};
    ~Role();
    
    virtual bool init();
    static Role *create(CCTMXTiledMap *map);
    void extraInit();
    void resetColissions();
    void resetRoleSprite();
    virtual void onEnter();
    
    CC_SYNTHESIZE(CCTMXTiledMap *, tiledMap, TiledMap);
    CC_PROPERTY(ROLE_STATE, roleState, RoleState);
    CC_PROPERTY(CCPoint, currentPosition, CurrentPosition);
    CC_SYNTHESIZE(float, constXSpeed, ConstXSpeed);
    CC_SYNTHESIZE(float, constYSpeed, ConstYSpeed);
    CC_SYNTHESIZE(float, xSpeed, XSpeed);
    CC_SYNTHESIZE(float, ySpeed, YSpeed);
    CC_SYNTHESIZE(float, accelerate, Accelerate);
    CC_PROPERTY(CCArray *, goldArray, GoldArray);
    CC_PROPERTY_READONLY(bool, isWin, IsWin);
    CC_SYNTHESIZE(int , jumpCount, JumpCount);
    
public:
    void playAgain();
    void reset();
    void jumpjump();
    
private:
    CCSprite *mSprite;
    CCParticleBatchNode *mParticleBatch;
    
private:
    void roleUpdate(float dt);
    
    void roleRunLogic();
    void roleStandbyLogic();
    void roleJumpUpLogic();
    void roleJumpDownLogic();
    void roleFreeFallLogic();
    
private:
    // 摄像头跟踪目标//
    void setMapPointByRole(CCTMXTiledMap *map, CCNode *node);
    void cameraUpdate(float dt);
    
private:
    // 碰撞检测//
    bool isCollisionArea(COLL_STATE state, CCRect box);
    bool isCollisionLeft(CCRect box);
    bool isCollisionRight(CCRect box);
    bool isCollisionTop(CCRect box);
    bool isCollisionBottom(CCRect box);
    // 碰撞修复//
    void fixCollision(COLL_STATE state, CCRect box);
    
    CCArray *mCollisionArray;
    static int collisionCompare(CCObject *obj1, CCObject *obj2);
    void updateCollisions(); // 根据地图所在区域，移除相应的碰撞物体//
    
    // 当前主角需要检测的Collision//
    CC_PROPERTY(CCRect, currentCollisionBox, CurrentCollisionBox);
    
private:
    // 金币碰撞帧//
    void goldUpdate(float dt);
    // 金币的碰撞检测//
    void collisionWithGold();
};

#endif /* defined(__HPaoKuGame__Role__) */
