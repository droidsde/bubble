#include "GoalLabel.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UserData.h"
#include "LevelScene.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;
using namespace cocostudio;

GoalLabel* GoalLabel::create(Sprite* s,string name)
{ 
	GoalLabel *pRet = new(std::nothrow) GoalLabel();
	if (pRet && pRet->init(s,name))
	{ 
		pRet->autorelease();
		return pRet;
	}	 
	else
	{ 
		delete pRet;
		pRet = nullptr;
		return nullptr;
	} 
}
bool GoalLabel::init(Sprite* s,string name)
{
	_label = ui::TextBMFont::create("0", "Fnt/write_fnt.fnt");
	_sprite = s;
	_name = name;
	return true;
}