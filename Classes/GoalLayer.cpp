#include "GoalLayer.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UserData.h"
#include "LevelScene.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;
using namespace cocostudio;

bool GoalLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
	return true;
}

void GoalLayer::addAGoalLabel(Sprite* s,string name)
{
	auto gl = GoalLabel::create(s,name);
	_goalLabels.pushBack(gl);
}
void GoalLayer::displayLabels(int visibleWidth,int positionY)
{
	this->setPosition(Vec2(0, positionY));
	int n = _goalLabels.size();
	if (n%2 == 0)
	{
		for (int i = 0; i < n; i++)
		{
			auto gl = _goalLabels.at(i);
			if (i % 2 == 0)
			{
				gl->_sprite->setPosition(Vec2(visibleWidth/2-100*i-75,0));
				gl->_label->setPosition(Vec2(visibleWidth / 2 - 100 * i-35,0));
			}
			else
			{
				gl->_sprite->setPosition(Vec2(visibleWidth / 2 + 100 * (i-1)+25, 0));
				gl->_label->setPosition(Vec2(visibleWidth / 2 + 100 * (i-1)+65, 0));
			}
			addChild(gl->_sprite);
			addChild(gl->_label);
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			auto gl = _goalLabels.at(i);
			if (i == 0)
			{
				gl->_sprite->setPosition(Vec2(visibleWidth / 2-25, 0));
				gl->_label->setPosition(Vec2(visibleWidth / 2+15, 0));
			}
			else
			{
				if (i % 2 == 0)
				{
					gl->_sprite->setPosition(Vec2(visibleWidth / 2 - 100 * (i-1) - 40, 0));
					gl->_label->setPosition(Vec2(visibleWidth / 2 - 100 * (i-1), 0));
				}
				else
				{
					gl->_sprite->setPosition(Vec2(visibleWidth / 2 + 100 * i, 0));
					gl->_label->setPosition(Vec2(visibleWidth / 2 + 100 * i + 40, 0));
				}
			}
			addChild(gl->_sprite);
			addChild(gl->_label);
		}
	}
}
void GoalLayer::updateALabel(string name, int newValue)
{
	for (int i = 0; i < _goalLabels.size(); i++)
	{
		if (_goalLabels.at(i)->_name == name)
		{
			_goalLabels.at(i)->_label->setText(StringUtils::format("%d", newValue));
		}
	}
}
