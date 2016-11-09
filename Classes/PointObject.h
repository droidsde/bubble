#ifndef POINTOBJECT_H_
#define POINTOBJECT_H_

#include "cocos2d.h"
#include "GameEnum.h"
USING_NS_CC;
//����cocos2d Vector�޷�����ԭ��Point����˷�װ���࣬����release
class PointObject : public CCObject
{
public:
	static PointObject* create(Point p);
	~PointObject();
	Point getPoint();
private:
	Point point;
	PointObject(Point p);
};

#endif /* defined(__paopaolong__PaoPao__) */