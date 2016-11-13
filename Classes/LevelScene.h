#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio;

class LevelLayer : public cocos2d::Layer
{
public:
	Sprite* _bg;
	LevelLayer();
	~LevelLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
	void LevelButtonCallback(Ref* pSender,int level);
	CREATE_FUNC(LevelLayer);

};

#endif  