#ifndef POINTOBJECT_H_
#define POINTOBJECT_H_

#include "cocos2d.h"
#include "GameEnum.h"
USING_NS_CC;
//由于cocos2d Vector无法储存原生Point，因此封装此类，可以release
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