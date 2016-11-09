#include "PointObject.h"
#include "GameConst.h"
#include "cocostudio/CocoStudio.h"


using namespace cocostudio;

PointObject::PointObject(Point p)//���캯����ʼ��
{
	this->point = p;
}
Point PointObject::getPoint()
{
	return this->point;
}

PointObject::~PointObject()
{

}
PointObject* PointObject::create(Point p)
{
	PointObject *pRet = new PointObject(p);
	if (pRet)
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}