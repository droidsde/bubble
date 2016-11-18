#include "Bubble.h"
#include "GameConst.h"
#include "cocostudio/CocoStudio.h"


using namespace cocostudio;

Bubble::Bubble()//构造函数初始化
{
	_attachment = NULL;
	_flag = false;
	_type = BUBBLE_TYPE_UNKNOW;
	_isSame = false;
	_isPass = false;
	_isMove = false;
	_isDie = false;
}

Bubble::~Bubble()
{
}
void Bubble::removeAttachment()
{
	_attachment->removeFromParentAndCleanup(true);
	_attachment = NULL;
}
Bubble * Bubble::initWithType(BubbleType type) //根据传过来的类型来生成泡泡
{
	Bubble *pRet = Bubble::create();
	pRet->setType(type);
	pRet->initWithSpriteFrameName(getStringByType(type));

	return pRet;
}
Bubble * Bubble::initWithBubbleType(BubbleType type) //根据传过来的类型来生成泡泡（使用InitWithFile）
{
	Bubble *pRet = Bubble::create();
	pRet->setType(type);
	pRet->initWithFile(StringUtils::format("Bubble/%d.png", (int)type));

	return pRet;
}


std::string Bubble::getStringByType(BubbleType type) //不同类型获取不同的图片资源
{
	std::string pRet;
	switch (type) {
	case BUBBLE_TYPE1:
	case BUBBLE_TYPE2:
	case BUBBLE_TYPE3:
	case BUBBLE_TYPE4:
	case BUBBLE_TYPE5:
	case BUBBLE_TYPE6:
	case BUBBLE_TYPE7:
		pRet = StringUtils::format(BUBBLE_NAME.c_str(), (int)type);
		break;
	case BUBBLE_TYPE_COLOR:
		pRet = BUBBLE_COLOR_NAME;
		break;
	default:
		break;
	}
	return pRet;
}

Attachment* Bubble::getAttachment()
{
	return _attachment;
}
void Bubble::addAttachment(AttachmentType type)
{
	auto *attachment = Attachment::initWithType(type);
	attachment->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	_attachment = attachment;
	this->addChild(attachment);
}
void Bubble::addAttachment(Attachment* attachment)
{
	auto *newAttach = Attachment::initWithType(attachment->getType());
	newAttach->setHasMoved(attachment->getHasMoved());
	newAttach->setHasWorked(attachment->getHasWorked());
	newAttach->setStepStatus(attachment->getStepStatus());
	newAttach->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	_attachment = newAttach;
	this->addChild(newAttach);
}