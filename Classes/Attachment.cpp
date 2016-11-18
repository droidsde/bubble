#include "Attachment.h"
#include "GameConst.h"
#include "cocostudio/CocoStudio.h"


using namespace cocostudio;

Attachment::Attachment()//构造函数初始化
{
	_type = ATTACHMENT_SILVER;
	_hasMoved = false;
	_hasWorked = false;
	_stepStatus = 0;

}

Attachment::~Attachment()
{

}
Attachment * Attachment::initWithType(AttachmentType type) //根据传过来的类型来生成附着物
{
	Attachment *pRet = Attachment::create();
	pRet->setType(type);
	pRet->initWithFile(StringUtils::format("Attachment/%d.png",type));
	//pRet->initWithSpriteFrameName(getStringByType(type));

	return pRet;
}


/*
std::string Attachment::getStringByType(AttachmentType type) //不同类型获取不同的图片资源
{
	std::string pRet;
	switch (type) {
	case Attachment_TYPE1:
	case Attachment_TYPE2:
	case Attachment_TYPE3:
	case Attachment_TYPE4:
	case Attachment_TYPE5:
	case Attachment_TYPE6:
	case Attachment_TYPE7:
		pRet = StringUtils::format(Attachment_NAME.c_str(), (int)type);
		break;
	case Attachment_TYPE_COLOR:
		pRet = Attachment_COLOR_NAME;
		break;
	default:
		break;
	}
	return pRet;
}
*/