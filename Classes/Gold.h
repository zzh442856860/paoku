//
//  Gold.h
//  HPaoKuGame
//
//  Created by DoubleHH on 14-3-6.
//
//

#ifndef __HPaoKuGame__Gold__
#define __HPaoKuGame__Gold__

#include "GHeaders.h"
USING_NS_CC;

class Gold : public CCObject {
public:
    
    Gold():goldValue(100), goldSprite(NULL) {};
    ~Gold();
    
    virtual bool init();
    
    CC_SYNTHESIZE(CCSprite *, goldSprite, GoldSprite);
    CC_SYNTHESIZE(int , goldValue, GoldValue);
    
    CREATE_FUNC(Gold);
};

#endif /* defined(__HPaoKuGame__Gold__) */
