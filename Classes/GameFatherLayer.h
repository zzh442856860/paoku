//
//  GameFatherLayer.h
//  HPaoKuGame
//
//  Created by DoubleHH on 14-3-6.
//
//

#ifndef __HPaoKuGame__GameFatherLayer__
#define __HPaoKuGame__GameFatherLayer__

#include "GHeaders.h"
#include "Role.h"
#include "BBButton.h"

class GameFatherLayer : public CCLayer {
public:
    GameFatherLayer();
    ~GameFatherLayer();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    static CCScene *scene();
    CREATE_FUNC(GameFatherLayer);
    
private:
    CCTMXTiledMap *mTMXTiledMap;
    Role *mRole;
    CCSpriteBatchNode *mGoldBatch;
    
private:
    void initMap();
    void initRole();
    void initCompoments();
    void resetCompoments();
    
private:
    void dispatchTouchDispatcher();
    
private:
    void buttonCallbackAgain(int tag);
};

#endif /* defined(__HPaoKuGame__GameFatherLayer__) */
