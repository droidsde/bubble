#ifndef GAME_CONST_H_
#define GAME_CONST_H_
#include "GameEnum.h"
using namespace std;

#define MAX_COLS 10
#define MAX_ROWS 12
#define MAX_CUS 5

const float BUBBLE_RADIUS = 26.0f;
const int HOURGLASSTIME = 30;

const float TOUCH_TOP = 0.78f;//0.88
const float TOUCH_DOWN = 0.23f;//0.33

const float MOVE_DISTANCE = 10.0f;

const int BUBBLE_COUNT = 7;

#define MAX_WAIT_PAOPAO 1

#define READY_PAOPAO_POS Point(270, 93)//270 193
#define WAIT_PAOPAO_POS Point(175, 70)//175 170

#define PAOPAO_SPEED 30.0f

const int RMB[5] = { 1, 4, 6, 10, 15 };

const string BUBBLE_NAME = "bubble_%d.png";
const string BUBBLE_COLOR_NAME = "caihong1.png";
const string BUBBLE_BOMB_NAME = "zhadan1.png";
const int customs[MAX_CUS][MAX_ROWS][MAX_COLS] = {   //Lev 1
													{
														{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
														{ 0, 0, 0, 2, 2, 2, 0, 0, 0 },
														{ 0, 0, 0, 0, 3, 3, 0, 0, 0, 0 },
														{ 0, 0, 0, 0, 4, 0, 0, 0, 0}
													}
													,
													{
														{ -1, -1, -1, -1, 1, 1, 1, 1, 1, 1 },
														{ -1, -1, -1, -1, 3, 2, 1, 1, 1 },
														{ -1, -1, -1, -1, -1, 1, 1, 1, 1, 1 },
														{ -1, -1, -1, -1, 3, 2, 1, 1, 1 }
													}
													,
													{
														{ 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
														{ 3, 3, 3, 2, 3, 2, 1, 2, 2 }
													}

};

const int attachments[MAX_CUS][MAX_ROWS][MAX_COLS] = {
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, ATTACHMENT_GHOST, -1, -1, -1, -1}
	}
	,
	{
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1 }
	}
	,
	{
		{ 1, 1, 1, -1, -1, -1, -1, -1, -1, -1 },
		{ -1, -1, -1, -1, -1, -1, -1, -1, -1}
	}
};
const int levelSettings[MAX_CUS][4][3] = { 
									{
										{0},//��ʱ��{�Ƿ�ʱ��}
										{1,100},//������������{�Ƿ�����}
										{3,10,4},//�����½���{�Ƿ񣬲���}{2�����ڵ��ڶ������½�,3}{3.,����������}
										{0,1}//����׹�����ݱ�ը�ӱ�
									},
									{
										{ 0 },//��ʱ��{�Ƿ�ʱ��}
										{ 1,100 },//������������{�Ƿ�����}
										{ 1,10 },//�����½���{�Ƿ񣬲���}
										{1,2}//����׹�����ݱ�ը�ӱ�
									},
									{
										{ 0 },//��ʱ��{�Ƿ�ʱ��}
										{ 1,100 },//������������{�Ƿ�����}
										{ 1,10 },//�����½���{�Ƿ񣬲���}
										{ 1,2 }//����׹�����ݱ�ը�ӱ�
									},
									{
										{ 0 },//��ʱ��{�Ƿ�ʱ��}
										{ 1,100 },//������������{�Ƿ�����}
										{ 1,10 }//�����½���{�Ƿ񣬲���}
									},
									{
										{ 1,60 },//��ʱ��{�Ƿ�ʱ��}
										{0},//������������{�Ƿ�����}
										{0}//�����½���{�Ƿ񣬲���}
									}
};

#endif