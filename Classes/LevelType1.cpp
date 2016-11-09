#include "LevelType1.h"
#include "GameScene.h"
#include "UserData.h"
#include "SimpleAudioEngine.h"
#include "PropLayer.h"
#include <thread>

using namespace CocosDenshion;
using namespace cocostudio::timeline;


static float waitTime = 0.1f;
static int sameSum = 0;

LevelType1* LevelType1:: create(int level)
{
	LevelType1 *pRet = new  LevelType1();
	if (pRet && pRet->init(level))
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
/*
Scene* LevelType1::scene()
{
		Scene *scene = Scene::create();

		LevelType1 *layer = LevelType1::create();

		scene->addChild(layer);

		return scene;
}
*/
bool LevelType1::init(int level)
{
	if (!Layer::init())
	{
		return false;
	}

	_level = level;
	
	//UserData::getInstance()->setLevel(1);
	//UserData::getInstance()->setScore(0);


	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(LevelType1::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(LevelType1::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(LevelType1::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);

	initTheBoard(_level);
	initWaitPaoPao();
	initReadyPaoPao();
	



	return true;
}

bool LevelType1::initTheBoard(int level)
{
	bool bRet = false;
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			board[i][j] = NULL;
		}
		bRet = true;
	}
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (i%2 && j == MAX_COLS - 1 || customs[level][i][j] == 0)
			{
				continue;
			}
			if (customs[level][i][j] == -1)
			{
				board[i][j] = randomPaoPao();
			}
			else if (customs[level][i][j] != 0)
			{
				board[i][j] = Bubble::initWithType((BubbleType)customs[level][i][j]);
			}
			bool flag = (i % 2) == 0 ? true : false;
			board[i][j]->setFlag(flag);
			addChild(board[i][j],1);
			initBubbleAction(board[i][j], i, j);
		}
	}

	return bRet;
}

Bubble * LevelType1::randomPaoPao()
{
	Bubble *pRet = NULL;
	BubbleType type = static_cast<BubbleType>(rand() % BUBBLE_COUNT + 1);
	pRet = Bubble::initWithType(type);
	return pRet;
}
Bubble * LevelType1::randomPaoPao(int num)
{
	Bubble *pRet = NULL;
	BubbleType type = static_cast<BubbleType>(rand() % num + 1);
	pRet = Bubble::initWithType(type);
	return pRet;
}
Point LevelType1::getPointByRowAndCol(int row, int col)
{
	bool flag = row % 2 == 0 ? true : false;
	Size winSize = Director::getInstance()->getWinSize();
	Point pos = Point((col * 2+ 1) * (BUBBLE_RADIUS + 1) + (flag ? 0 : BUBBLE_RADIUS), TOUCH_TOP * winSize.height - row * (BUBBLE_RADIUS * 2 - 5) - BUBBLE_RADIUS);
	
	return pos;
}
Vec2 LevelType1::getRowAndColByPoint(Point target)
{
	Size winSize = Director::getInstance()->getWinSize();
	int x = (TOUCH_TOP * winSize.height - target.y) / (BUBBLE_RADIUS * 2 - 5);
	int y = ((target.x - (x % 2) * BUBBLE_RADIUS) / ((BUBBLE_RADIUS + 1) * 2));
	return Vec2(x, y);
}
void LevelType1::initWaitPaoPao()
{
	for (int i = 0; i < MAX_WAIT_PAOPAO; ++i)
	{
		Bubble *obj = randomPaoPao(3);
		obj->setPosition(WAIT_PAOPAO_POS);
		wait[i] = obj;
		this->addChild(obj);
	}
}
void LevelType1::initReadyPaoPao()
{
	ready = randomPaoPao(3);
	ready->setPosition(READY_PAOPAO_POS);
	this->addChild(ready);
}
void LevelType1::onTouch(Point target)
{
	this->real = (target - ready->getPosition()).getNormalized();
	auto gameScene = (GameScene*)this->getParent();
	auto proplayer = (PropLayer*)gameScene->_propLayer;
	switch (ready->getType())
	{
		case BUBBLE_TYPE_COLOR: UserData::getInstance()->addColorBubble(-1); 
			proplayer->setColorBubbleNum();
			break;
		case BUBBLE_TYPE_BOMB: UserData::getInstance()->addBomb(-1); 
			proplayer->setBombNum();
			break;
		default: break;
	}
	this->scheduleUpdate();
}


void LevelType1::setEnable(){
	_listener->setEnabled(true);
}

void LevelType1::setDisable(){
	_listener->setEnabled(false);
}

void LevelType1::update(float fDelta) {
	
	if (isCollideBorder()) 	
	{
		real.x = -real.x; 	
	}
	setDisable();
	Point pos = ready->getPosition();
	ready->setPosition(Point(pos.x + real.x * PAOPAO_SPEED, pos.y + real.y * PAOPAO_SPEED));
	if (checCollideBoard()) 
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Hit.mp3");
		this->unscheduleUpdate();
		real = Point::ZERO;
		correctReadyPosition();
		return;
	}
}

bool LevelType1::isCollideBorder()
{
	bool bRet = false;
	Size size = Director::getInstance()->getWinSize();
	Point pos = ready->getPosition();
	if (pos.x + BUBBLE_RADIUS > size.width || pos.x - BUBBLE_RADIUS / 2 < 0)
	{
		bRet = true;
	}
	return bRet;
}

bool LevelType1::checCollideBoard()
{
	bool bRet = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto point = ready->getPosition();
// 	Vec2 rowCol = getRowAndColByPoint(ready->getPosition());
// 	int row = rowCol.x;
// 	int col = rowCol.y;
	if (point.y < TOUCH_DOWN* visibleSize.height)
	{
		return false;
	}


	if (ready->getPosition().y > TOUCH_TOP*visibleSize.height - BUBBLE_RADIUS)
	{
		if (ready->getType() == BUBBLE_TYPE_COLOR)
		{
			BubbleType type = (BubbleType)((int)(CCRANDOM_0_1() * 7 + 1));
			ready->setType(type);
			ready->initWithSpriteFrameName(StringUtils::format(BUBBLE_NAME.c_str(), type));
		}
		bRet = true;
		return true;
	}

	std::vector<Vec2> rowCol;
	rowCol.push_back(getRowAndColByPoint(Point(point.x, point.y + BUBBLE_RADIUS)));
	rowCol.push_back(getRowAndColByPoint(Point(point.x - BUBBLE_RADIUS, point.y)));
	rowCol.push_back(getRowAndColByPoint(Point(point.x + BUBBLE_RADIUS, point.y)));

	for (auto &ti : rowCol)
	{
		if (board[int(ti.x)][int(ti.y)] != nullptr)
		{
			return true;
		}
	}


	//Vec2 rowCol = getRowAndColByPoint(ready->getPosition());
	/*
	for (int i = rowCol.x + 1; i >= 0 && i >= rowCol.x - 1; --i)
	{
		for (int j = rowCol.y + 1; j >= 0 && j >= rowCol.y - 1; --j)
		{
			if (i >= MAX_ROWS || j >= MAX_COLS)
			{
				continue;
			}
			Bubble *obj = board[i][j];
			if (obj && ready && isCircleCollision(obj->getPosition(), BUBBLE_RADIUS, ready->getPosition(), BUBBLE_RADIUS))
			{
				if (ready->getType() == BUBBLE_TYPE_COLOR)
				{
					ready->setType(obj->getType());
					ready->initWithSpriteFrameName(StringUtils::format(BUBBLE_NAME.c_str(), obj->getType()));
				}
				return true;
			}
		}
 	}*/
// 	 	for (int i = MAX_ROWS - 1; i >= 0; --i)
// 	 	{
// 	 		for (int j = 0; j < MAX_COLS; ++j)
// 	 		{
// 	 			Bubble *obj = board[i][j];
// 	 			if (obj && ready && isCircleCollision(obj->getPosition(), BUBBLE_RADIUS, ready->getPosition(), BUBBLE_RADIUS))
// 	 			{   if (ready->getType() == BUBBLE_TYPE_COLOR)
// 					{
// 						ready->setType(obj->getType());
// 	 					ready->initWithSpriteFrameName(StringUtils::format(BUBBLE_NAME.c_str(), obj->getType()));
// 	 				}
// 	 				bRet = true;
// 	 				break;
// 	 			}
// 	 		}
// 	 		if (bRet) break;
// 	 	}
	
// 	if (board[(int)rowCol.x][(int)rowCol.y] != nullptr)
// 	{
// 		bRet = true;
// 	}
// 	if (row >= 12)
// 	{
// 		return false;
// 	}
// 	if (board[row][col] != nullptr)
// 	{
// 		bRet = true;
// 	}
// 	CCLOG("row %d, col %d", row, col);
	return false;
}

void LevelType1::changeWaitToReady()
{
	ready = wait[0];
	auto jumpAction = JumpTo::create(0.1f, READY_PAOPAO_POS, 30.0f, 1);

	auto callFunc = CallFunc::create(CC_CALLBACK_0(LevelType1::jumpActionCallBack, this));

	auto seq = Sequence::create(jumpAction, callFunc, nullptr);

	ready->runAction(seq);
	conditionsCheck();
}
void LevelType1:: conditionsCheck()
{
	if (isPass() && _havePass == false)
	{
		_havePass = true;
		setDisable();
		auto gameScene = (GameScene*)this->getParent();
		gameScene->_propLayer->setTime(60);

		Armature* armature = Armature::create("paopaolong");
		armature->getAnimation()->play("gongxiguoguan");
		this->addChild(armature);
		SimpleAudioEngine::getInstance()->playEffect("Music/Guoguan.mp3");
		armature->setPosition(270, 580);
		armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(LevelType1::movementPassCallBack, this));
	}

	bubbleNumber++;
	if (levelSettings[_level][1][0] == 1)//如有限泡泡数启用
	{
		if (bubbleNumber == levelSettings[_level][1][1])
		{
			gameOver(true);
		}
	}
	if (levelSettings[_level][2][0] == 1)//如泡泡下降启用
	{
		if (bubbleNumber%levelSettings[_level][2][1] == 0)
		{
			addTwoRows();
		}
	}
	if (levelSettings[_level][2][0] == 2)//如泡泡下降启用
	{
		if (checkRemainRows(levelSettings[_level][2][1]))
		{
			addTwoRows();
		}
	}
}
bool LevelType1::checkRemainRows(int row)
{
	if (row < MAX_ROWS)
	{
		for (int i = 0; i < MAX_COLS; i++)
		{
			if (row % 2 && i == (MAX_COLS - 1))
			{
				continue;
			}
			if (board[row][i] != NULL)
			{
				return false;
			}
		}
	}
	return true;
}
void LevelType1::addTwoRows()
{
		for (int j = 0; j < MAX_COLS - 1; ++j)//如果bubble[9]不为空，则加行后游戏结束
		{
			if (board[9][j] == NULL)
			{
				continue;
			}
			else
			{
				gameOver(true);
			}
		}
		for (int i = MAX_ROWS - 1; i >= 2; i--)
		{
			for (int j = 0; j < MAX_COLS; ++j)
			{
				if ((i % 2 && j == MAX_COLS - 1) || (board[i][j] == NULL&&board[i - 2][j] == NULL))
				{
					continue;
				}
				if (board[i][j] != NULL)
				{
					board[i][j]->removeFromParentAndCleanup(true);
				}
				board[i][j] = NULL;
				if (board[i - 2][j] != NULL)
				{
					board[i][j] = Bubble::initWithType(board[i - 2][j]->getType());
					board[i][j]->setFlag(board[i - 2][j]->getFlag());
					addChild(board[i][j],1);
					addTwoRowsOriginalBubbleAction(board[i][j], i, j);
				}
			}
		}
		for (int j = 0; j < MAX_COLS; ++j)//bubble[0] Random
		{
			if (board[0][j] != NULL)
			{
				board[0][j]->removeFromParentAndCleanup(true);
			}
			board[0][j] = randomPaoPao(3);
			bool flag = true;
			board[0][j]->setFlag(flag);
			addChild(board[0][j]);
			addTwoRowsRandomBubbleAction(board[0][j], 0, j);
		}
		for (int j = 0; j < MAX_COLS - 1; ++j)//bubble[1] Random
		{
			if (board[1][j] != NULL)
			{
				board[1][j]->removeFromParentAndCleanup(true);
			}
			board[1][j] = randomPaoPao(3);
			bool flag = false;
			board[1][j]->setFlag(flag);
			addChild(board[1][j]);
			addTwoRowsRandomBubbleAction(board[1][j], 1, j);
		}
}
void LevelType1::correctReadyPosition()
{
	int offX = 0, offY = 0;
	int row = 0, col = 0;
	Point pos = ready->getPosition();
	Vec2 rowCol = getRowAndColByPoint(pos);
	offX = rowCol.x == 0 ? 0 : rowCol.x - 1;
	offY = rowCol.y == 0 ? 0 : rowCol.y - 1;
	float length = FLT_MAX;
	bool flag = (int)(rowCol.x + 1) % 2 == 0 ? true : false;
	//bool flag = getFirstRowFlag();
	bool tempFlag = flag;

// 
// 	for (int i = 0; i < MAX_ROWS; ++i) 	{
// 		for (int j = 0; j < MAX_COLS; ++j)
// 		{
// 			if (!board[i][j])
// 			{
// 				if (j == MAX_COLS - 1 && !flag)
// 				{
// 					continue;
// 				}
// 				Point tPos = getPointByRowAndCol(i, j, flag);
// 				if (tPos.getDistance(pos) < length)
// 				{
// 					offX = i;
// 					offY = j;
// 					tempFlag = flag;
// 					length = tPos.getDistance(pos);
// 				}
// 			}
// 		}
// 		flag = !flag;
// 	}
// 	if (board[offX][offY] == nullptr)
// 	{
// 		board[offX][offY] = ready;
// 	}
	
	for (int i = offX; i < MAX_ROWS && i < offX + 4; i++)
	{
		for (int j = offY; j < MAX_COLS && j < offY + 4; j++)
		{
			if (board[i][j] == nullptr)
			{
				if (j == MAX_COLS - 1 && !flag)
				{
					continue;
				}
				Point tPos = getPointByRowAndCol(i, j);
				if (tPos.getDistance(pos) < length)
				{
					row = i;
					col = j;
					tempFlag = flag;
					length = tPos.getDistance(pos);
				}
			}
		}
		flag = !flag;
	}

	board[row][col] = ready;
	ready->setFlag(tempFlag);
	if (getPointByRowAndCol(row, col).y <= TOUCH_DOWN * (Director::getInstance()->getVisibleSize().height))
	{
		gameOver(true);
		return;
	}
	std::thread moveBubble(&LevelType1::moveTheBubble, this, row, col, tempFlag, MOVE_DISTANCE);
	
	//std::thread readyAction(&[=](){ready->runAction(MoveTo::create(0.2f, getPointByRowAndCol(row, col, tempFlag))); }, this);
	moveBubble.join();
	//readyAction.join();
	ready->runAction(Sequence::create(MoveTo::create(0.2f, getPointByRowAndCol(row, col)), CallFunc::create(CC_CALLBACK_0(LevelType1::readyAction, this)), nullptr));
// 	if (ready->getType() == BUBBLE_TYPE_BOMB)
// 	{
// 		bubbleBlast(row, col, tempFlag);
// 	}
// 	else
// 	{
// 		sameSum = 0;
// 		waitTime = 0.1f;
// 		findTheSameBubble(row, col, tempFlag, board[row][col]->getType());
// 		deleteTheSameBubble(row,col,tempFlag);
// 	}
// 	ready = NULL;
}

void LevelType1::readyAction()
{
	setDisable();
	Vec2 RowAndCol = getRowAndColByPoint(ready->getPosition());
	int row = RowAndCol.x;
	int col = RowAndCol.y;
	bool tempFlag = row % 2 == 0 ? true : false;
	if (ready->getType() == BUBBLE_TYPE_BOMB)
	{
		bubbleBlast(row, col, tempFlag);
	}
	else
	{
		sameSum = 0;
		waitTime = 0.1f;
		findTheSameBubble(row, col, tempFlag, board[row][col]->getType());
		deleteTheSameBubble(row, col, tempFlag);
	}
	resetAllPass();
	checkDownBubble();
	findRandomBombPositions();
	downBubble();
	throwBallAction();
	changeWaitToReady();
	//ready = NULL;
}
bool LevelType1::getFirstRowFlag()  //得到第一行是否左缺 不缺为true
{
	for (int i = 0; i < MAX_COLS; ++i)
	{
		if (board[0][i]) return board[0][i]->getFlag();
	}
	return true;
}

bool LevelType1::isCircleCollision(Point pos1, float radius1, Point pos2, float radius2)
{
	if (pos1.getDistance(pos2) >(radius1 + radius2 - 10) || pos2.y > pos1.y)
	{
		return false;
	}
	return true;
}

void LevelType1::findTheSameBubble(int i, int j, bool flag, BubbleType type)
{	
	setDisable();
	if (i < 0 || i >= MAX_ROWS || j < 0 || j >= MAX_COLS) 	{
		return;
	}
	if (board[i][j] == nullptr) 	{
		return;
	}
	if (board[i][j]->getType() != type) 	{
		return;
	}
	if (board[i][j]->getIsSame()) 	{
		return;
	}
	/*
	首先将自己isSame设置为true，并且增加theSame
	从当前位置左上，右上，左下，右下，左，右六个方向进行寻找
	*/
	board[i][j]->setIsSame(true);
	++sameSum;

	if (flag && j > 0 && i > 0) 	{
		findTheSameBubble(i - 1, j - 1, !flag, type);
	}
	else if (!flag && i > 0) 	{
		findTheSameBubble(i - 1, j, !flag, type);
	}
	if (flag && i > 0) 	{
		findTheSameBubble(i - 1, j, !flag, type);
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1) 	{
		findTheSameBubble(i - 1, j + 1, !flag, type);
	}
	if (flag && j > 0 && i < MAX_ROWS) 	{
		findTheSameBubble(i + 1, j - 1, !flag, type);
	}
	else if (!flag && i < MAX_ROWS) 	{
		findTheSameBubble(i + 1, j, !flag, type);
	}
	if (flag && i < MAX_ROWS) 	{
		findTheSameBubble(i + 1, j, !flag, type);
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1) 	{
		findTheSameBubble(i + 1, j + 1, !flag, type);
	}
	if (j > 0)
	{
		findTheSameBubble(i, j - 1, flag, type);
	}
	if (j < MAX_COLS)
	{
		findTheSameBubble(i, j + 1, flag, type);
	}                                                                                                                                                                                                                                        
	if (sameSum >= 5)
	{
		waitTime += 0.05f;
		Bubble *obj = board[i][j];  
		SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("BubbleSpecial/baozha.ExportJson");
		Armature* armature = Armature::create("baozha");
		obj->addChild(armature);
		armature->setPosition(BUBBLE_RADIUS, BUBBLE_RADIUS);
		armature->getAnimation()->play("daojubaozha");
		obj->runAction(Sequence::create(FadeOut::create(waitTime), CallFunc::create([=](){obj->removeFromParent(); }), nullptr));
	}
}
void LevelType1::findAGroup(int n1, int n2, int n3, int n4, int n5, int n6, int n7)
{
	int nmin = 5;
	if (!(n1 == 0 && n2 == 0 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0))
	{
		int a = 1;
		//same group
		if (n1 >= nmin&& n2 == 0 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
		{
			sameGroup1++;
		}
		else if (n1 == 0 && n2 >= nmin && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
		{
			sameGroup2++;
		}
		else if (n1 == 0 && n2 == 0 && n3 >= nmin && n4 == 0 && n5 == 0 && n6 == 0 && n7 == 0)
		{
			sameGroup3++;
		}
		else if (n1 == 0 && n2 == 0 && n3 == 0 && n4 >= nmin && n5 == 0 && n6 == 0 && n7 == 0)
		{
			sameGroup4++;
		}
		else if (n1 == 0 && n2 == 0 && n3 == 0 && n4 == 0 && n5 >= nmin && n6 == 0 && n7 == 0)
		{
			sameGroup5++;
		}
		else if (n1 == 0 && n2 == 0 && n3 == 0 && n4 == 0 && n5 == 0 && n6 >= nmin && n7 == 0)
		{
			sameGroup6++;
		}
		else if (n1 == 0 && n2 == 0 && n3 == 0 && n4 == 0 && n5 == 0 && n6 == 0 && n7 >= nmin)
		{
			sameGroup7++;
		}
		//colorfulGroup
		else if (n1 == n2 && n2 == n3 && n3 == n4 && n4== n5 && n5 == n6 && n6 == n7 &&n1 > 0)
		{
			colorfulGroup++;
		}
		else
		{
			if (n1 + n2 + n3 + n4 + n5 + n6 + n7 >= 5)
			{
				commonGroup++;
			}
		}
	}


}
void LevelType1::bubbleBlast(int i, int j, bool flag)
{
	bubbleAction(board[i][j]);
	board[i][j] = nullptr;
	ready = nullptr;

	if (flag && j > 0 && i > 0 && board[i-1][j-1]) 	{
		bubbleAction(board[i - 1][j - 1]);
		board[i - 1][j - 1] = nullptr;
	}
	else if (!flag && i > 0 && board[i-1][j]) 	{
		bubbleAction(board[i - 1][j]);
		board[i - 1][j] = nullptr;
	}
	if (flag && i > 0 && board[i - 1][j]) 	{
		bubbleAction(board[i - 1][j]);
		board[i - 1][j] = nullptr;
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1 && board[i - 1][j + 1]) 	{
		bubbleAction(board[i - 1][j+1]);
		board[i - 1][j + 1] = nullptr;
	}
	if (flag && j > 0 && i < MAX_ROWS && board[i + 1][j - 1]) 	{
		bubbleAction(board[i + 1][j - 1]);
		board[i + 1][j - 1] = nullptr;
	}
	else if (!flag && i < MAX_ROWS && board[i + 1][j]) 	{
		bubbleAction(board[i + 1][j]);
		board[i + 1][j] = nullptr;
	}
	if (flag && i < MAX_ROWS && board[i + 1][j]) 	{
		bubbleAction(board[i + 1][j]);
		board[i + 1][j] = nullptr;
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1 && board[i + 1][j + 1]) 	{
		bubbleAction(board[i + 1][j+1]);
		board[i + 1][j + 1] = nullptr;
	}
	if (j > 0 && board[i][j - 1])
	{
		bubbleAction(board[i][j - 1]);
		board[i][j - 1] = nullptr;
	}
	if (j < MAX_COLS && board[i][j + 1])
	{
		bubbleAction(board[i][j + 1]);
		board[i][j + 1] = nullptr;
	}
}
void LevelType1::randomBombBlast(int i, int j, bool flag)
{
	randomBombBlastAction(board[i][j]);
	board[i][j] = nullptr;
	if (flag && j > 0 && i > 0 && board[i - 1][j - 1]) {
		randomBombBlastAction(board[i - 1][j - 1]);
		board[i - 1][j - 1] = nullptr;
	}
	else if (!flag && i > 0 && board[i - 1][j]) {
		randomBombBlastAction(board[i - 1][j]);
		board[i - 1][j] = nullptr;
	}
	if (flag && i > 0 && board[i - 1][j]) {
		randomBombBlastAction(board[i - 1][j]);
		board[i - 1][j] = nullptr;
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1 && board[i - 1][j + 1]) {
		randomBombBlastAction(board[i - 1][j + 1]);
		board[i - 1][j + 1] = nullptr;
	}
	if (flag && j > 0 && i < MAX_ROWS && board[i + 1][j - 1]) {
		randomBombBlastAction(board[i + 1][j - 1]);
		board[i + 1][j - 1] = nullptr;
	}
	else if (!flag && i < MAX_ROWS && board[i + 1][j]) {
		randomBombBlastAction(board[i + 1][j]);
		board[i + 1][j] = nullptr;
	}
	if (flag && i < MAX_ROWS && board[i + 1][j]) {
		randomBombBlastAction(board[i + 1][j]);
		board[i + 1][j] = nullptr;
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1 && board[i + 1][j + 1]) {
		randomBombBlastAction(board[i + 1][j + 1]);
		board[i + 1][j + 1] = nullptr;
	}
	if (j > 0 && board[i][j - 1])
	{
		randomBombBlastAction(board[i][j - 1]);
		board[i][j - 1] = nullptr;
	}
	if (j < MAX_COLS && board[i][j + 1])
	{
		randomBombBlastAction(board[i][j + 1]);
		board[i][j + 1] = nullptr;
	}
}
void LevelType1::moveTheBubble(int i, int j, bool flag, float distance)
{

	if (distance <= 1.0f)
	{
		return;
	}
	if (i < 0 || i >= MAX_ROWS || j < 0 || j >= MAX_COLS) 	{
		return;
	}
	if (board[i][j] == nullptr) 	{
		return;
	}
	if (board[i][j]->getIsMove()) 	{
		return;
	}

	board[i][j]->setIsMove(true);
	
	auto dir = (((Vec2)getPointByRowAndCol(i, j) - ready->getPosition()).getNormalized())*distance;
	auto moveBy = MoveBy::create(0.1f, dir);
	auto seq = Sequence::create(moveBy, moveBy->reverse(), nullptr);
	board[i][j]->runAction(seq);
	
	if (flag && j > 0 && i > 0) 	{
		moveTheBubble(i - 1, j - 1, !flag, distance - 5);
	}
	else if (!flag && i > 0) 	{
		moveTheBubble(i - 1, j, !flag, distance - 5);
	}
	if (flag && i > 0) 	{
		moveTheBubble(i - 1, j, !flag, distance - 5);
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1) 	{
		moveTheBubble(i - 1, j + 1, !flag, distance - 5);
	}
	if (flag && j > 0 && i < MAX_ROWS) 	{
		moveTheBubble(i + 1, j - 1, !flag, distance - 5);
	}
	else if (!flag && i < MAX_ROWS) 	{
		moveTheBubble(i + 1, j, !flag, distance - 5);
	}
	if (flag && i < MAX_ROWS) 	{
		moveTheBubble(i + 1, j, !flag, distance - 5);
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1) 	{
		moveTheBubble(i + 1, j + 1, !flag, distance - 5);
	}
	if (j > 0)
	{
		moveTheBubble(i, j - 1, flag, distance - 5);
	}
	if (j < MAX_COLS)
	{
		moveTheBubble(i, j + 1, flag, distance - 5);
	}
}

void LevelType1::deleteTheSameBubble(int i, int j, bool flag)
{
	if (sameSum < 3)
	{
		for (int i = 0; i < MAX_ROWS; ++i) 	{
			for (int j = 0; j < MAX_COLS; ++j)
			{
				if (board[i][j] != nullptr && board[i][j]->getIsSame())
				{
					board[i][j]->setIsSame(false);
					sameSum--;
				}
			}
		}
		setEnable();
	}
	else
	{
		for (int i = 0; i < MAX_ROWS; ++i) 	{  
			for (int j = 0; j < MAX_COLS; ++j)
			{
				if (board[i][j] != nullptr && board[i][j]->getIsSame())
				{
					Bubble *obj = board[i][j];
					waitTime += 0.05f;
					SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");

					ArmatureDataManager::getInstance()->addArmatureFileInfo("BubbleSpecial/baozha.ExportJson");
					Armature* armature = Armature::create("baozha");
					obj->addChild(armature);
					armature->setPosition(BUBBLE_RADIUS, BUBBLE_RADIUS);
					armature->getAnimation()->play("daojubaozha");
					obj->runAction(Sequence::create(FadeOut::create(waitTime), CallFunc::create([=](){obj->removeFromParent(); setEnable(); }), nullptr));
					board[i][j] = NULL;
				}
			}
		}

	}


}

void LevelType1::bubbleAction(Bubble *obj)
{
	//道具爆炸改为与普通爆炸效果相同
	waitTime += 0.05f;
	SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("BubbleSpecial/baozha.ExportJson");
	Armature* armature = Armature::create("baozha");
	obj->addChild(armature);
	armature->setPosition(BUBBLE_RADIUS, BUBBLE_RADIUS);
	armature->getAnimation()->play("daojubaozha");
	obj->runAction(Sequence::create(FadeOut::create(waitTime), CallFunc::create([=]() {obj->removeFromParent(); setEnable(); }), nullptr));
	/*
	//原普通消除式爆炸
	auto gameSceme = (GameScene*)this->getParent();
	gameSceme->_propLayer->AddScoreLabel(5);
	SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");

 	auto armature = (Armature*)(obj->getChildByTag(20));
 	//armature->getAnimation()->play("daojubaozha");
	//obj->runAction(Sequence::create(DelayTime::create(waitTime), CallFuncN::create(CC_CALLBACK_1(LevelType1::callbackRemoveBubble, this)),NULL));
	obj->runAction(Sequence::create(FadeOut::create(waitTime), CallFunc::create([=](){obj->removeFromParent(); setEnable(); }), NULL));
	*/
}
void LevelType1::randomBombBlastAction(Bubble *obj)
{
	//道具爆炸改为与普通爆炸效果相同
	float delay = 1.2f;
	waitTime += 0.05f;
	SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("BubbleSpecial/baozha.ExportJson");
	Armature* armature = Armature::create("baozha");
	obj->addChild(armature);
	armature->setPosition(BUBBLE_RADIUS, BUBBLE_RADIUS);
	runAction(Sequence::create(DelayTime::create(delay), CallFunc::create([=]() {armature->getAnimation()->play("daojubaozha"); }), nullptr));
	obj->runAction(Sequence::create(DelayTime::create(delay),FadeOut::create(0.1f),CallFunc::create([=]() {obj->removeFromParent(); setEnable(); }), nullptr));
	//原FadeOut::create(waitTime), 
	/*
	//原普通消除式爆炸
	auto gameSceme = (GameScene*)this->getParent();
	gameSceme->_propLayer->AddScoreLabel(5);
	SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");

	auto armature = (Armature*)(obj->getChildByTag(20));
	//armature->getAnimation()->play("daojubaozha");
	//obj->runAction(Sequence::create(DelayTime::create(waitTime), CallFuncN::create(CC_CALLBACK_1(LevelType1::callbackRemoveBubble, this)),NULL));
	obj->runAction(Sequence::create(FadeOut::create(waitTime), CallFunc::create([=](){obj->removeFromParent(); setEnable(); }), NULL));
	*/
}
void LevelType1::callbackRemoveBubble(cocos2d::Node *obj)
{
	auto bubble = dynamic_cast<Bubble*>(obj);
	if (bubble != nullptr)
	{
		Armature* armature = Armature::create("paopaolong");
		armature->getAnimation()->play("daojubaozha");
		obj->addChild(armature);
		armature->setPosition(obj->getContentSize() / 2);
		armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(LevelType1::moveParantCallBack, this));
	}

}
void LevelType1::movementPassCallBack(Armature * armature, MovementEventType type, const std::string &name)
{
	if (type == COMPLETE){
		if (name == "gongxiguoguan")
		{
			this->removeChild(armature);
			_level++;
			if (_level >= MAX_CUS)
			{
				return;
			}
			this->initTheBoard(_level);
			UserData::getInstance()->addLevel(1);
			_havePass = false;
		}

	}
}

void LevelType1::moveParantCallBack(Armature * armature, MovementEventType type, const std::string &name)
{
	if (type == COMPLETE){
		{
			auto bubble = (Bubble*)armature->getParent();
			bubble->removeFromParentAndCleanup(true);
			sameSum--;
			if (sameSum == 0)
			{
				setEnable();
			}
		}
	}
}
void LevelType1::jumpActionCallBack()
{
	for (int i = 0; i < MAX_WAIT_PAOPAO - 1; ++i)
	{
		wait[i] = wait[i + 1];
	}
	wait[MAX_WAIT_PAOPAO - 1] = randomPaoPao(3)
		;
	this->addChild(wait[MAX_WAIT_PAOPAO - 1], -1);
	for (int i = 0; i < MAX_WAIT_PAOPAO; ++i)
	{
		wait[i]->setPosition(Point(WAIT_PAOPAO_POS));
	}
}
void LevelType1::resetAllPass()
{
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (board[i][j])
			{
				board[i][j]->setIsPass(false);
				board[i][j]->setIsMove(false);
			}
		}
	}
}

void LevelType1::checkDownBubble()
{
	for (int i = 0; i < MAX_COLS; ++i) 	{
		if (board[0][i]) board[0][i]->setIsPass(true);
	}
		for (int i = 0; i < MAX_ROWS; ++i)
	{
				/*
		当第一次的时候横着只关心右边，第二次的时候横着只关心左边
		剩下关心与自己相关的下面两个
		*/
				for (int j = 0; j < MAX_COLS; ++j)
		{
			if (board[i][j] && board[i][j]->getIsPass())
			{
								if (j < MAX_COLS - 1 && board[i][j + 1]) board[i][j + 1]->setIsPass(true);
				if (i < MAX_ROWS - 1)
				{
										if (board[i][j]->getFlag() && j > 0 && board[i + 1][j - 1]) board[i + 1][j - 1]->setIsPass(true);
					else if (!(board[i][j]->getFlag()) && board[i + 1][j]) board[i + 1][j]->setIsPass(true);
										if (board[i][j]->getFlag() && board[i + 1][j]) board[i + 1][j]->setIsPass(true);
					else if (!(board[i][j]->getFlag()) && j < MAX_COLS - 1 && board[i + 1][j + 1]) board[i + 1][j + 1]->setIsPass(true);
				}
			}
		}
				for (int j = MAX_COLS - 1; j >= 0; --j)
		{
			if (board[i][j] && board[i][j]->getIsPass())
			{
								if (j > 0 && board[i][j - 1]) board[i][j - 1]->setIsPass(true);
				if (i < MAX_ROWS - 1)
				{
										if (board[i][j]->getFlag() && j > 0 && board[i + 1][j - 1]) board[i + 1][j - 1]->setIsPass(true);
					else if (!(board[i][j]->getFlag()) && board[i + 1][j]) board[i + 1][j]->setIsPass(true);
										if (board[i][j]->getFlag() && board[i + 1][j]) board[i + 1][j]->setIsPass(true);
					else if (!(board[i][j]->getFlag()) && j < MAX_COLS - 1 && board[i + 1][j + 1]) board[i + 1][j + 1]->setIsPass(true);
				}
			}
		}
	}
}

void LevelType1::downBubble()
{
	int n1=0, n2=0, n3=0, n4=0, n5=0, n6=0, n7=0;//下坠泡泡中各种类型泡泡的数量
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (board[i][j] && !(board[i][j]->getIsPass()))
			{
				Bubble *obj = board[i][j];
				//计算下落泡泡中各类型泡泡数
				switch (obj->getType())
				{
				case BUBBLE_TYPE1:
					n1++;
					break;
				case BUBBLE_TYPE2:
					n2++;
					break;
				case BUBBLE_TYPE3:
					n3++;
					break;
				case BUBBLE_TYPE4:
					n4++;
					break;
				case BUBBLE_TYPE5:
					n5++;
					break;
				case BUBBLE_TYPE6:
					n6++;
					break;
				case BUBBLE_TYPE7:
					n7++;
					break;
				}
				downBubbleAction(obj);
				board[i][j] = NULL;
			}
		}
		setEnable();
	}
	
	throwRandomBombs();
	findAGroup(n1,n2,n3,n4,n5,n6,n7);
	conditionsCheck();
}

void LevelType1::downBubbleAction(Bubble *obj)
{
	auto gameSceme = (GameScene*)this->getParent();
	gameSceme->_propLayer->AddScoreLabel(5);

	float offY = 200.0;
	Point pos = obj->getPosition();
	if (!_randomBombAnimationPositions.empty())
	{
		auto *s = Sprite::create();
		s->initWithFile("temp2.png");
		addChild(s,999);
		s->setPosition(Point(0,0));
		auto p =_randomBombAnimationPositions.back()->getPoint();
		s->runAction(Sequence::create(MoveTo::create(1.2f,p), CallFunc::create([=]() { s->removeFromParentAndCleanup(true); }), nullptr));
		_randomBombAnimationPositions.popBack();
	}
	obj->runAction(Sequence::create(MoveTo::create((pos.y - offY) / 600.0, Point(pos.x, offY)), CallFuncN::create(CC_CALLBACK_1(LevelType1::downBubbleActionCallBack,this)), NULL));
}

void LevelType1::downBubbleActionCallBack(Node *obj)
{
	auto bubble = dynamic_cast<Bubble*>(obj);
	auto particle = ParticleSystemQuad::create("Particle/luoxia_lizi.plist");
	particle->setPosition(bubble->getContentSize().width/2, 0);
	bubble->addChild(particle);
	bubble->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.1f), CallFunc::create([=]() { bubble->removeFromParentAndCleanup(true);  }), nullptr));
}
void LevelType1::findRandomBombPositions()
{
	if (levelSettings[_level][3][0] == 1)//如启用坠落泡泡爆炸加成
	{
		int x = levelSettings[_level][3][1];
		//findDownBubbleNumber
		int downNumber = 0;
		for (int i = 0; i < MAX_ROWS; ++i)
		{
			for (int j = 0; j < MAX_COLS; ++j)
			{
				if (board[i][j] && !(board[i][j]->getIsPass()))
				{
					downNumber++;
				}
			}
		}
		//
		for (int i = 0; i < downNumber/x; i++)
		{
			bool isDone = false;
			while (!isDone)
			{
				int notNullNumber = 0;
				for (int i = MAX_ROWS - 1; i >= 0; i--)
				{
					for (int j = 0; j < MAX_COLS; j++)
					{
						if (i % 2 && (j = MAX_COLS - 1) || board[i][j] == NULL)
						{
							continue;
						}
						if (board[i][j]->getType() != BUBBLE_TYPE_BOMB&&board[i][j]->getType() != BUBBLE_TYPE_COLOR && board[i][j]->getIsPass() == true)
						{
							notNullNumber++;
							if (rand() % 3 == 1 && isDone == false)
							{
								auto p = getPointByRowAndCol(i, j);
								auto *po = PointObject::create(p);
								_randomBombPositions.pushBack(po);
								_randomBombAnimationPositions.pushBack(po);
								isDone = true;
								break;
							}
						}
					}
				}
				if (notNullNumber == 0)
				{
					isDone = true;
				}
			}
		}
	}
}
void LevelType1::throwRandomBombs()
{
	for (int i = 0; i < _randomBombPositions.size();i++)//如启用坠落泡泡爆炸加成
	{
		if (!_randomBombPositions.empty())
		{
			auto p = _randomBombPositions.back()->getPoint();
			auto v = getRowAndColByPoint(p);
			bool flag = (int)v.x % 2 ? false : true;
			if (board[(int)v.x][(int)v.y] != NULL)
			{
				randomBombBlast((int)v.x, (int)v.y, flag);
			}
			_randomBombPositions.popBack();
			resetAllPass();
			checkDownBubble();
			findRandomBombPositions();
			downBubble();
		}
	}
}
void LevelType1::initBubbleAction(Bubble *obj, int i, int j)
{
	setDisable();
	auto point = getPointByRowAndCol(i, j);
	auto start = Point(point.x, 300.0f - i * BUBBLE_RADIUS * 2);
	obj->setPosition(start);
	auto moveTo = MoveTo::create(0.4f + j * 0.1f, point);
	obj->runAction(Sequence::create(moveTo, CallFunc::create([=](){setEnable(); }), nullptr));
}
void LevelType1::addTwoRowsOriginalBubbleAction(Bubble *obj, int i, int j)
{
	setDisable();
	auto point = getPointByRowAndCol(i, j);
	auto start = Point(point.x, point.y+4*BUBBLE_RADIUS);
	obj->setPosition(start);
	auto moveTo = MoveTo::create(1.0f, point);
	obj->runAction(Sequence::create(DelayTime::create(1.2f),moveTo, CallFunc::create([=]() {setEnable(); }), nullptr));
}
void LevelType1::addTwoRowsRandomBubbleAction(Bubble *obj, int i, int j)
{
	setDisable();
	auto point = getPointByRowAndCol(i, j);
	auto start = Point(point.x,point.y+4*BUBBLE_RADIUS);
	obj->setPosition(start);
	auto moveTo = MoveTo::create(1.0f, point);
	obj->runAction(Sequence::create(DelayTime::create(1.2f),moveTo, CallFunc::create([=]() {setEnable(); }), nullptr));
}

void LevelType1::gameOver(bool over)
{
	auto gameSceme = (GameScene*)this->getParent();
	gameSceme->gameOver(over);
}

void LevelType1::swapBubble()
{
	auto readyPoint = ready->getPosition();
	auto waitPoint = wait[0]->getPosition();

	ready->runAction(MoveTo::create(0.1f, waitPoint));
	wait[0]->runAction(MoveTo::create(0.1f, readyPoint));

	auto temp = ready;
	ready = wait[0];
	wait[0] = temp;

	throwBallAction();
}
void LevelType1::colorBubble()
{
	ready->setType(BUBBLE_TYPE_COLOR);
	ready->setSpriteFrame(BUBBLE_COLOR_NAME.c_str());
}
void LevelType1::bombBubble()
{
	ready->setType(BUBBLE_TYPE_BOMB);
	ready->initWithSpriteFrameName(BUBBLE_BOMB_NAME);
}
void LevelType1::auxiliaryLine(Point tagrat)
{	
	auto node = Node::create();
	addChild(node);
	node->setTag(100);
	
	
	auto speed = 30.0f;
	auto position = Point(READY_PAOPAO_POS.x + real.x * speed, READY_PAOPAO_POS.y + real.y * speed);
	CCLOG("%f,%f", real.x, real.y);

	while (position.y < TOUCH_TOP * Director::getInstance()->getVisibleSize().height - BUBBLE_RADIUS)
	{
		for (int i = MAX_ROWS - 1; i >= 0; i--)
		{
			for (int j = 0; j < MAX_COLS; j++)
			{
				if (board[i][j] != nullptr)
				{
					if ((board[i][j]->getPosition()).getDistance(position) <= BUBBLE_RADIUS*1.5)
					{
						return;
					}
				}
			}
		}
		if (position.x <= BUBBLE_RADIUS || position.x >= Director::getInstance()->getVisibleSize().width - BUBBLE_RADIUS)
		{
			real.x = -real.x;
		}
		position = Point(position.x + real.x * speed, position.y + real.y * speed);
		auto line = Sprite::createWithSpriteFrameName("line.png");
		node->addChild(line);
		line->setPosition(position);
	}
}
void LevelType1::setReadyAngle(Point target)
{
	auto angle = (target - READY_PAOPAO_POS).getAngle(Vec2(0, 1));
	ready->setRotation(CC_RADIANS_TO_DEGREES(angle));
}
void LevelType1::throwBallAction()
{
	auto gameScene = (GameScene*)this->getParent();

	auto bgNode = (Node*)gameScene->_bg;
	auto bgArmature = (Armature*)bgNode->getChildByTag(63);
	bgArmature->getAnimation()->play("huanqiu");

	auto propLayer = (PropLayer*)gameScene->_propLayer;
	auto propNode = (Node*)propLayer->_propLayer;
	auto propArmature = (Armature*)propNode->getChildByTag(68);
	propArmature->getAnimation()->play("huanqiushou");
}

bool LevelType1::isPass()
{
	if (commonGroup >= 10||colorfulGroup>=2&&sameGroup1>=2)
		return true;
	else
		return false;
}

bool LevelType1::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void LevelType1::onTouchMoved(Touch *touch, Event *unused_event)
{
	/*
	auto gameScene = (GameScene*)this->getParent();
	LevelType1* LevelType1 = gameScene->_LevelType1;
	LevelType1->removeChildByTag(100);

	if (touch->getLocation().y <= TOUCH_DOWN * Director::getInstance()->getVisibleSize().height || touch->getLocation().y >= TOUCH_TOP*Director::getInstance()->getVisibleSize().height)
	{
		return;
	}

	auto real = (touch->getLocation() - LevelType1->ready->getPosition()).getNormalized();
	LevelType1->setReal(real);
	if (real.x <= 0.9 && real.x >= -0.9 && real != Vec2::ZERO && real.y > 0)
	{
		LevelType1->auxiliaryLine(touch->getLocation());
		LevelType1->setReadyAngle(touch->getLocation());
		gameScene->setCannonAngle(touch->getLocation());
	}
	*/
	auto gameScene = (GameScene*)this->getParent();
	this->removeChildByTag(100);
	if (touch->getLocation().y <= TOUCH_DOWN * Director::getInstance()->getVisibleSize().height || touch->getLocation().y >= TOUCH_TOP*Director::getInstance()->getVisibleSize().height)
	{
		return;
	}
	real = (touch->getLocation() - READY_PAOPAO_POS).getNormalized();
	if (real.x <= 0.9 && real.x >= -0.9 && real != Vec2::ZERO && real.y > 0)
	{
		this->auxiliaryLine(touch->getLocation());
		this->setReadyAngle(touch->getLocation());
		gameScene->setCannonAngle(touch->getLocation());
	}

}

void LevelType1::onTouchEnded(Touch *touch, Event *unused_event)
{
	/*
	auto gameScene = (GameScene*)this->getParent();
	LevelType1* LevelType1 = gameScene->_LevelType1;
	LevelType1->removeChildByTag(100);

	if (touch->getLocation().y <= TOUCH_DOWN * Director::getInstance()->getVisibleSize().height && touch->getLocation().x <= 200)
	{
		gameScene->swapBubble();
	}

	if (touch->getLocation().y <= TOUCH_DOWN * Director::getInstance()->getVisibleSize().height || touch->getLocation().y >= TOUCH_TOP * Director::getInstance()->getVisibleSize().height)
	{
		setTouchEnable(true);
		return;
	}
	auto real = (touch->getLocation() - LevelType1->ready->getPosition()).getNormalized();

	if (real.x <= 0.9 && real.x >= -0.9 && real != Vec2::ZERO && real.y > 0)
	{
		gameScene->setCannonAngle(touch->getLocation());
		gameScene->onTouch(touch->getLocation());
		gameScene->_propLayer->setCannonAction(touch->getLocation());

	}
	*/
	auto gameScene = (GameScene*)this->getParent();
	this->removeChildByTag(100);
	if (touch->getLocation().y <= TOUCH_DOWN * Director::getInstance()->getVisibleSize().height && touch->getLocation().x <= 200)
	{
		this->swapBubble();
	}
	if (touch->getLocation().y <= TOUCH_DOWN * Director::getInstance()->getVisibleSize().height || touch->getLocation().y >= TOUCH_TOP * Director::getInstance()->getVisibleSize().height)
	{
		_listener->setEnabled(true);
		return;
	}
	real = (touch->getLocation() - READY_PAOPAO_POS).getNormalized();
	if (real.x <= 0.9 && real.x >= -0.9 && real != Vec2::ZERO && real.y > 0)
	{
		gameScene->setCannonAngle(touch->getLocation());
		this->onTouch(touch->getLocation());
		gameScene->_propLayer->setCannonAction(touch->getLocation());

	}

}