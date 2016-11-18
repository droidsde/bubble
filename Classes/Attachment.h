#ifndef ATTACHMENT_H_
#define ATTACHMENT_H_

#include "cocos2d.h"
#include "GameEnum.h"
USING_NS_CC;
//泡泡类，包含类型和自己所在的一行是否是左缺的状态
class Attachment : public Sprite
{
public:
	Attachment();
	~Attachment();
	static Attachment *initWithType(AttachmentType type = ATTACHMENT_SILVER);
	CREATE_FUNC(Attachment);
	CC_SYNTHESIZE(AttachmentType, _type, Type);
	CC_SYNTHESIZE(bool, _hasMoved, HasMoved);
	CC_SYNTHESIZE(bool, _hasWorked, HasWorked);
	CC_SYNTHESIZE(int, _stepStatus, StepStatus);
private:
	//static std::string getStringByType(AttachmentType type);
};

#endif /* defined(__paopaolong__PaoPao__) */