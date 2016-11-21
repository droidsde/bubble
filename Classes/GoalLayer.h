#ifndef GOAL_LAYER_H_
#define GOAL_LAYER_H_

#include "cocos2d.h"
#include "GameConst.h"
#include "ui/CocosGUI.h"
#include "GoalLabel.h"

USING_NS_CC;

class GoalLayer : public LayerColor
{
public:
	CREATE_FUNC(GoalLayer);
	virtual bool init();
	Vector<GoalLabel*> _goalLabels;
	void addAGoalLabel(Sprite* s,string name);
	void displayLabels(int visibleWidth,int positionY);
};



#endif
