#ifndef GAME_CONST_H_
#define GAME_CONST_H_

using namespace std;

#define MAX_COLS 10
#define MAX_ROWS 12
#define MAX_CUS 11

const float BUBBLE_RADIUS = 26.0f;
const int HOURGLASSTIME = 30;

const float TOUCH_TOP = 0.88f;
const float TOUCH_DOWN = 0.33f;

const float MOVE_DISTANCE = 20.0f;

const int BUBBLE_COUNT = 7;

#define MAX_WAIT_PAOPAO 1

#define READY_PAOPAO_POS Point(270, 193)
#define WAIT_PAOPAO_POS Point(175,170)

#define PAOPAO_SPEED 30.0f

const int RMB[5] = { 1, 4, 6, 10, 15 };

const string BUBBLE_NAME = "bubble_%d.png";
const string BUBBLE_COLOR_NAME = "caihong1.png";
const string BUBBLE_BOMB_NAME = "zhadan1.png";

const int customs[MAX_CUS][MAX_ROWS][MAX_COLS] = { 
													{ { 1, 2, 3, 4, 5, 6, 7, 1, 2, 3 },
													{ 7, 6, 5, 4, 3, 2, 1, 7, 6 },
													{ 6, 1, 7, 3, 7, 5, 4, 3, 1, 2 },
													{ 5, 2, 6, 1, 2, 6, 7, 6, 5 },
													{ 4, 7, 3, 5, 4, 3, 1, 4, 2, 1 },
													{ 0, 5, 6, 7, 1, 7, 2, 3, 0 },
													{ 0, 0, 0, 2, 3, 6, 5, 0, 0, 0 },
													{ 0, 0, 0, 0, 1, 4, 0, 0, 0 },
													{ 0, 0, 0, 0, 7, 0, 0, 0, 0, 0 },
													{ 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	}
};


#endif