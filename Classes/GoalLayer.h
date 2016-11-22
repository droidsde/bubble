#ifndef GOAL_LAYER_H_
#define GOAL_LAYER_H_

#include "cocos2d.h"
#include "GameConst.h"
#include "ui/CocosGUI.h"
#include "GoalLabel.h"

USING_NS_CC;
//使用方法：auto gl = GoalLayer::create(); gl->addAlabel();gl->displayLabels(visibleWidth,positionY);addChild(gl);
class GoalLayer : public LayerColor
{
public:
	CREATE_FUNC(GoalLayer);
	virtual bool init();
	Vector<GoalLabel*> _goalLabels;
	void addAGoalLabel(Sprite* s,string name);
	void displayLabels(int visibleWidth,int positionY);
	void updateALabel(string name, int newValue);
};



#endif
