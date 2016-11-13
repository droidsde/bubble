#include "LevelScene.h"
#include "GameScene.h"
#include "TurnplateLayer.h"
#include "UserData.h"
#include "GameConst.h"
#include "ui/CocosGUI.h"
#include "ShopLayer.h"
#include "GiftLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace CocosDenshion;

LevelLayer::LevelLayer()
{
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("main_music.mp3", true);
}

LevelLayer::~LevelLayer()
{
	
}

Scene* LevelLayer::scene()
{
    Scene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = Scene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        LevelLayer *layer = LevelLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LevelLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_bg = Sprite::create();
	_bg->initWithFile("levels_choosing.png");
	addChild(_bg);
	_bg->setPosition(320, 480);

	auto buttonLevelOne = ui::Button::create("level1.png");
	buttonLevelOne->addClickEventListener(CC_CALLBACK_1(LevelLayer::LevelButtonCallback, this,1));
	addChild(buttonLevelOne);
	buttonLevelOne->setPosition(Vec2(270, 680));

	auto buttonLevelTwo = ui::Button::create("level2.png");
	buttonLevelTwo->addClickEventListener(CC_CALLBACK_1(LevelLayer::LevelButtonCallback, this, 2));
	addChild(buttonLevelTwo);
	buttonLevelTwo->setPosition(Vec2(270, 580));

	auto buttonLevelThree = ui::Button::create("level3.png");
	buttonLevelThree->addClickEventListener(CC_CALLBACK_1(LevelLayer::LevelButtonCallback, this, 3));
	addChild(buttonLevelThree);
	buttonLevelThree->setPosition(Vec2(270, 480));
	

	return true;
}
void LevelLayer::LevelButtonCallback(Ref* pSender,int level)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.mp3");
	Director::getInstance()->pushScene(GameScene::create(level-1));//create(level-1)
	
}