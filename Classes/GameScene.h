#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "cocos2d.h"
#include "TouchLayer.h"
#include "LevelType1.h"
//#include "BubbleLayer.h"
#include "PropLayer.h"


USING_NS_CC;

typedef enum{
	k_Operate_Pause,
	k_Operate_Resume

}OperateFlag;

class GameScene : public Scene
{
public:
	static GameScene* create(int level);
	virtual bool init(int level);
	int _level;
	TouchLayer* _touchLayer;
	LevelType1* _bubbleLayer;
	PropLayer* _propLayer;
	Node* _bg;

	void onTouch(Point target);
	void setTouchEnable(bool enable);
	void swapBubble();
	void bombBubble();
	void colorBubble();
	void gameOver(bool over = false);
	void operateAllSchedulerAndActions(Node* node, OperateFlag flag);
	void mPause();
	void mResume();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void setCannonAngle(Point target);

};






#endif