#ifndef GOAL_LABEL_H_
#define GOAL_LABEL_H_

#include "cocos2d.h"
#include "GameConst.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class GoalLabel:public CCObject
{
public:
	static GoalLabel* create(Sprite* s,string name);
	virtual bool init(Sprite* s,string name);
	ui::TextBMFont* _label;
	Sprite* _sprite;
	string _name;
};


#endif
