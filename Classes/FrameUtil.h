#ifndef _frameutil_h_
#define _frameutil_h_

#include "cocos2d.h"

USING_NS_CC;
//这个类应该不错，实用性很强//
CCRect getFrame(CCNode* node);
CCPoint getCenter(CCNode* node);
float getWidth(CCNode* node);
float getHeight(cocos2d::CCNode *node);
float getLeft(cocos2d::CCNode *node);
float getRight(cocos2d::CCNode *node);
float getTop(cocos2d::CCNode *node);
float getBottom(cocos2d::CCNode *node);

void hhPrintRect(cocos2d::CCRect rect, const char *a);
void hhPrintSize(cocos2d::CCSize size, const char *a);
void hhPrintPoint(cocos2d::CCPoint point, const char *a);
void hhPrintRect(cocos2d::CCRect rect);
void hhPrintSize(cocos2d::CCSize size);
void hhPrintPoint(cocos2d::CCPoint point);

#endif