#ifndef DEF_HANCHOROBOT_H
#define DEF_HANCHOROBOT_H
#define N 10
#define OFSET_X 100
#define OFSET_Y 10
#define SIZE 50
#define PI 3.1415926536
#include "DxLib.h"
#include <cmath>

/*************************
* ���{�b�g��Ֆʂ̏�Ԃ��X�V����p�̃R�[�h�Q
**************************/


class Point{
public:
	double x,y;
	Point(double _x, double _y){
		x = _x;
		y = _y;
	}
	Point(){};
	void Rotate(int theta,double _x = 0, double _y = 0){
		double nx,ny;
		nx = (x-_x)*cos(theta*PI/180)-(y-_y)*sin(theta*PI/180);
		ny = (x-_x)*sin(theta*PI/180)+(y-_y)*cos(theta*PI/180);
		x = nx+_x;
		y = ny+_y;
	}
};
bool if_go[N][N] = {0};		//�e�}�X�����j�ς݂����L�^
bool wall[2][N][N] = {		//�c���̕ǂ̏��
					{{1,1,1,1,1,1,1,1,1,1},
					{0,0,0,0,0,0,1,1,0,1},
					{0,1,0,0,0,0,1,0,1,1},
					{0,0,0,1,1,0,0,1,1,1},
					{0,0,0,0,1,0,1,1,0,1},
					{1,1,0,0,0,0,0,0,0,0},
					{0,1,1,0,0,0,0,1,1,0},
					{1,0,0,0,0,0,0,0,0,0},
					{0,1,1,0,0,0,0,0,0,0},
					{1,1,0,0,0,0,0,0,0,0}},

					{{1,0,0,0,1,0,0,0,0,0},
					{1,0,1,0,0,0,1,0,0,0},
					{1,0,0,0,0,0,1,0,0,0},
					{1,0,0,0,0,0,1,0,0,0},
					{1,0,0,0,0,0,1,0,0,0},
					{1,0,0,0,0,0,0,0,0,0},
					{1,0,0,1,0,0,0,1,0,0},
					{1,0,0,1,0,0,0,1,0,0},
					{1,0,0,0,0,0,0,0,0,0},
					{1,0,0,0,0,0,0,0,0,0}}

					};
int mission[5][2]={	{1,1},	//�~�b�V�����ł��铥�ނׂ����}�X�̈ʒu���W
					{2,7},
					{4,3},
					{7,2},
					{7,6}};
int FlowChartHandle;		//�t���[�`���[�g�̉摜�n���h��
char player[5]="^>v<";
int orient = 2;				//���{�b�g�̌���0:��,1:�E,2:��,3:��
int x=0,y=4;				//���{�b�g�̍��W
//int turn = 0;
bool marker[N][N] = {0};	//�e�}�X�Ƀ}�[�J�[���u���Ă��邩�L�^

void turn_left(){			//����]
	orient--;
	orient+=4;
	orient%=4;
}
void turn_right(){			//�E��]
	orient++;
	orient%=4;
}
bool if_wall(int orient_ = 0){	//�ǂ����݂��邩����@��{���{�b�g�̑O���̕ǂ����邪�A����orient_�őO�㍶�E�̔�����\
	orient_ = (orient+orient_)%4;
	if(orient_ == 0 && !wall[0][x][y])
		return false;
	if(orient_ == 1 && !wall[1][x][(y+1)%N])
		return false;
	if(orient_ == 2 && !wall[0][(x+1)%N][y])
		return false;
	if(orient_ == 3 && !wall[1][x][y%N])
		return false;	
	return true;
}
void go_straight(){		//�O�Ɉړ��@�ǂ�����ꍇ�͂��̏�ɂƂǂ܂�
	if_go[x][y] = true;	//�ړ��O�̃}�X�𓥔j�ς݂ɐݒ�
	if(orient == 0 && !wall[0][x][y])
		x = (x-1+N)%N;
	if(orient == 1 && !wall[1][x][(y+1)%N])
		y = (y+1)%N;
	if(orient == 2 && !wall[0][(x+1)%N][y])
		x = (x+1)%N;
	if(orient == 3 && !wall[1][x][y%N])
		y = (y-1+N)%N;
	if_go[x][y] = true;	//�ړ���̃}�X�𓥔j�ς݂ɐݒ�
}
void put_marker(){		//���݂̃}�X�Ƀ}�[�J�[��u��
	marker[x][y] = true;
}
void remove_marker(){	//���݂̃}�X����}�[�J�[����菜��
	marker[x][y] = false;
}
bool read_marker(int location = 0){	//�}�[�J�[�̗L���̔���@location=0:���̏�,1:�O,2:�E,3:��,4:�����`�F�b�N
	if(location == 0)
		return marker[x][y];
	if(if_wall(location-1))			//�ڂ̑O���ǂ̏ꍇ�A�}�[�J�[�������Ȃ����߁A���ۂɒu���Ă��邩���֌W��False���Ԃ�
		return 0;
	location = (orient+location+3)%4;
	if(location == 0){				//��ʊO���Q�Ƃ��Ȃ��悤�ȏ���
		if(x == 0)
			return 0;
		else 
			return marker[x-1][y];
	}else if(location == 1){
		if(y == N-1)
			return 0;
		else
			return marker[x][y+1];
	}else if(location == 2){
		if(x == N-1)
			return 0;
		else
			return marker[x+1][y];
	}else if(location == 3){
		if(y == 0)
			return 0;
		else
			return marker[x][y-1];
	}
	return -1;
}
bool if_star() {					//������}�X�����}�X���ǂ�������
	for (int i = 0; i < 5; i++) {
		if (mission[i][0] == x && mission[i][1] == y)
			return true;
	}
	return false;
}
void break_wall() {	//���H�̕ǂ�1�x�����󂷁@��U�肵�Ă��g�p�ς݂ƂȂ�
	static bool if_use = false;
	if (if_use)
		return;
	if_use = true;
	if (orient == 0)
		wall[0][x][y] = 0;
	if (orient == 1)
		wall[1][x][(y + 1) % N] = 0;
	if (orient == 2)
		wall[0][(x + 1) % N][y] = 0;
	if (orient == 3)
		wall[1][x][y%N] = 0;
}
bool if_north() {	//�k�������Ă��邩����
	return orient % 4 == 0;
}
bool if_east() {	//���������Ă��邩����
	return orient % 4 == 1;
}
bool if_south() {	//��������Ă��邩����
	return orient % 4 == 2;
}
bool if_west() {	//���������Ă��邩����
	return orient % 4 == 3;
}

void InitializeMap() {	//�}�b�v�̏������@�ǁA�}�[�J�[�A���j�ς݂��A���{�b�g�̈ʒu�A�����@��������
	bool origin_wall[2][N][N] = {
		{ { 1,1,1,1,1,1,1,1,1,1 },
		{ 0,0,0,0,0,0,1,1,0,1 },
		{ 0,1,0,0,0,0,1,0,1,1 },
		{ 0,0,0,1,1,0,0,1,1,1 },
		{ 0,0,0,0,1,0,1,1,0,1 },
		{ 1,1,0,0,0,0,0,0,0,0 },
		{ 0,1,1,0,0,0,0,1,1,0 },
		{ 1,0,0,0,0,0,0,0,0,0 },
		{ 0,1,1,0,0,0,0,0,0,0 },
		{ 1,1,0,0,0,0,0,0,0,0 } },

		{ { 1,0,0,0,1,0,0,0,0,0 },
		{ 1,0,1,0,0,0,1,0,0,0 },
		{ 1,0,0,0,0,0,1,0,0,0 },
		{ 1,0,0,0,0,0,1,0,0,0 },
		{ 1,0,0,0,0,0,1,0,0,0 },
		{ 1,0,0,0,0,0,0,0,0,0 },
		{ 1,0,0,1,0,0,0,1,0,0 },
		{ 1,0,0,1,0,0,0,1,0,0 },
		{ 1,0,0,0,0,0,0,0,0,0 },
		{ 1,0,0,0,0,0,0,0,0,0 } }

	};

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			marker[i][j] = 0;
			if_go[i][j] = 0;
			wall[0][i][j] = origin_wall[0][i][j];
			wall[1][i][j] = origin_wall[1][i][j];
		}
	}
	//turn = 0;
	x = 0;
	y = 4;
	orient = 2;

}
void DrawPlayer(int _x=y*SIZE, int _y=x*SIZE,int theta=orient*90){	//���{�b�g���ˋL���ŕ`�� �O�p�`�Ǝl�p�`��g�ݍ��킹�ā˂�`��
	_x+=SIZE/2+OFSET_X;
	_y+=SIZE/2+OFSET_Y;
	Point tri[3];	//�O�p�`(���̐�[)
	tri[0] = Point(-SIZE/3,0);
	tri[1] = Point(SIZE/3,0);
	tri[2] = Point(0,-SIZE/3);
	for(int i = 0; i < 3; i++){
		tri[i].Rotate(theta);
		tri[i].x += _x;
		tri[i].y += _y;
	}
	Point box[4];	//�l�p�`(���̍��{)
	box[0] = Point(-SIZE/6,0);
	box[1] = Point( SIZE/6,0);
	box[2] = Point( SIZE/6,SIZE/3);
	box[3] = Point(-SIZE/6,SIZE/3);
	for(int i = 0; i < 4; i++){
		box[i].Rotate(theta);
		box[i].x += _x;
		box[i].y += _y;
	}
	DrawTriangle(tri[0].x,tri[0].y,tri[1].x,tri[1].y,tri[2].x,tri[2].y,GetColor(0,255,0),true);
	DrawTriangle(box[0].x,box[0].y,box[1].x,box[1].y,box[2].x,box[2].y,GetColor(0,255,0),true);
	DrawTriangle(box[0].x,box[0].y,box[3].x,box[3].y,box[2].x,box[2].y,GetColor(0,255,0),true);
	DrawLine(box[0].x,box[0].y,box[1].x,box[1].y,GetColor(0,0,0),2);
	DrawLine(box[2].x,box[2].y,box[1].x,box[1].y,GetColor(0,0,0),2);
	DrawLine(box[2].x,box[2].y,box[3].x,box[3].y,GetColor(0,0,0),2);
	DrawLine(box[0].x,box[0].y,box[3].x,box[3].y,GetColor(0,0,0),2);
	DrawLine(tri[0].x,tri[0].y,tri[1].x,tri[1].y,GetColor(0,0,0),2);
	DrawLine(tri[2].x,tri[2].y,tri[1].x,tri[1].y,GetColor(0,0,0),2);
	DrawLine(tri[0].x,tri[0].y,tri[2].x,tri[2].y,GetColor(0,0,0),2);
}




//���{�b�g�̑���A���S���Y�������s�@����уt���[�`���[�g��Ō��݂̎��s�ӏ���`��
// speed:�A�j���[�V�����̎��s�X�s�[�h
// if_draw:�`��̗L���̐ݒ�
//�A���S���Y���͈ȉ�
//������}�X�Ƀ}�[�J�[������Ȃ��菜���O�i
//						�Ȃ��Ȃ�u���ĉE��]���O�i
void policy(double speed = 1,bool if_draw = true){	
	static int pre_mode = 0;	//1�O�Ɏ��s���Ă����R�}���h���(�t���[�`���[�g�̐}���Ɏg��)
	static int mode = 0;		//���ݎ��s���Ă���R�}���h���
	static double time = 0;

	if(mode == 0)
		goto Mode0;
	if(mode == 1)
		goto Mode1;
	if(mode == 2)
		goto Mode2;
	if(mode == 3)
		goto Mode3;
Mode0://������}�X�Ƀ}�[�J�[�����邩�Ȃ�������
	if(read_marker()){
Mode1://�}�[�J�[������ꍇ�A��菜��
		time += speed;
		if(if_draw){
			DrawLine(890,175,890,250,GetColor(0,0,255),2);
			DrawLine(727,175,890,175,GetColor(0,0,255),2);
			DrawLine(727,103,727,175,GetColor(0,0,255),2);
			DrawLine(726,103,626,103,GetColor(0,0,255),2);
			DrawLine(626,103,626,524,GetColor(0,0,255),2);
			DrawLine(803,524,626,524,GetColor(0,0,255),2);
			DrawLine(803,524,803,486,GetColor(0,0,255),2);	
			DrawCircle(893,285,33,GetColor(0,0,255) ,0,2);

			DrawPlayer();
		}
		if( time > 40){
			remove_marker();
			mode = 3;
			time = 0;
			pre_mode = 1;
		}else{
			return;
		}
	}else{
		mode = 2;
Mode2://�}�[�J�[���Ȃ��ꍇ�A�����ĉE�ɉ�]
		time += speed;
		if(speed  < 40){	//�X�s�[�h�A�b�v�p��speed<40�ł̂݃A�j���[�V���������s
			if( time > 80){			//80<time �Ŏ��̏�Ԃ֑J��
				put_marker();
				turn_right();
				mode = 3;
				pre_mode = 2;
				time = 0;
			}else if( time > 40){	//40<time<80�̊ԂŉE�։�]
				if(if_draw){
					DrawLine(727,320,727,330,GetColor(0,0,255),2);
					DrawCircle(726,363,33,GetColor(0,0,255) ,0,2);
					DrawPlayer(y*50,x*50,orient*90+(time-40)*90/40);
				}
				put_marker();
				return;
			}else{					//time<40�̊Ԃ͒�~
				if(if_draw){
					DrawLine(727,103,727,250,GetColor(0,0,255),2);
					if(pre_mode != 0){
						DrawLine(726,103,626,103,GetColor(0,0,255),2);
						DrawLine(626,103,626,524,GetColor(0,0,255),2);
						DrawLine(800,524,626,524,GetColor(0,0,255),2);
					}
					DrawCircle(726,285,33,GetColor(0,0,255) ,0,2);
					DrawPlayer();
				}
				return;
			}
		}else{	//speed>40�ł̓A�j���[�V������S�Ĕ�΂�
				put_marker();
				turn_right();
				mode = 3;
				pre_mode = 2;
				time = 0;
				if(if_draw){
					DrawLine(727,320,727,330,GetColor(0,0,255),2);
					DrawCircle(726,363,33,GetColor(0,0,255) ,0,2);
					DrawLine(727,103,727,250,GetColor(0,0,255),2);
					DrawLine(726,103,626,103,GetColor(0,0,255),2);
					DrawLine(626,103,626,524,GetColor(0,0,255),2);
					DrawLine(800,524,626,524,GetColor(0,0,255),2);
					DrawCircle(726,285,33,GetColor(0,0,255) ,0,2);
					DrawPlayer();
				}
		}
	}
Mode3://�O�i
	time += speed;
	if(if_draw){
		if(pre_mode == 1){
			DrawLine(893,322,893,405,GetColor(0,0,255),2);
			DrawLine(803,405,890,405,GetColor(0,0,255),2);
			DrawLine(803,405,803,415,GetColor(0,0,255),2);
			DrawCircle(802,453,33,GetColor(0,0,255) ,0,2);
		}
		if(pre_mode == 2){
			DrawLine(727,392,727,405,GetColor(0,0,255),2);
			DrawLine(803,405,727,405,GetColor(0,0,255),2);
			DrawLine(803,405,803,415,GetColor(0,0,255),2);
			DrawCircle(802,453,33,GetColor(0,0,255) ,0,2);

		}
	}

	if(time > 50){
		go_straight();
		if(if_draw)
			DrawPlayer();
		mode = 0;
		time = 0;
	}else{
		if(!if_draw)
			return;
		if(if_wall()){
			DrawPlayer();
			return;
		}
		if(orient == 0)
			DrawPlayer(y*50,x*50-time);
		else if(orient == 1)
			DrawPlayer(y*50+time,x*50);
		else if(orient == 2)
			DrawPlayer(y*50,x*50+time);
		else if(orient == 3)
			DrawPlayer(y*50-time,x*50);	
	}
	return;

}


void LoadFlowChartHandle(){		//�t���[�`���[�g�̉摜��ǂݍ���
	FlowChartHandle = LoadGraph("FlowChart.bmp");
}

void PrintBoard(bool explanation,bool if_41 = false){	//�Ֆʂ���ю��ӏ��̕`��
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(wall[0][j][i]){	//�ǂ̕`��
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+(i+1)*SIZE,OFSET_Y+j*SIZE,GetColor(0,0,0),5);
			}else{
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+(i+1)*SIZE,OFSET_Y+j*SIZE,GetColor(0,0,0),1);
			}
			if(wall[1][j][i]){
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+i*SIZE,OFSET_Y+(j+1)*SIZE,GetColor(0,0,0),5);
			}else{
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+i*SIZE,OFSET_Y+(j+1)*SIZE,GetColor(0,0,0),1);
			}
			if(if_go[j][i])	//���j�ς݂̃}�X�̕`��
				DrawBox(OFSET_X+i*SIZE+2,OFSET_Y+j*SIZE+2,OFSET_X+(i+1)*SIZE-2,OFSET_Y+(j+1)*SIZE-2,GetColor(180,180,255),true);
		}
	}
	DrawLine(OFSET_X,OFSET_Y+N*SIZE,OFSET_X+N*SIZE,OFSET_Y+N*SIZE,GetColor(0,0,0),5);
	DrawLine(OFSET_X+N*SIZE,OFSET_Y,OFSET_X+N*SIZE,OFSET_Y+N*SIZE,GetColor(0,0,0),5);

	for(int i = 0; i < N; i++){	//�}�[�J�[�̕`��
		for(int j = 0; j < N; j++){
			if(marker[i][j])
				DrawCircle(OFSET_X+j*SIZE+SIZE/2,OFSET_Y+i*SIZE+SIZE/2,SIZE/5,GetColor(255,0,0),0,3);
		}
	}
	SetFontSize(40);	//���}�X�̕`��
	for(int i = 0; i < 5;i++){
		DrawFormatString(OFSET_X+3+mission[i][0]*SIZE,OFSET_Y+3+mission[i][1]*SIZE,GetColor(255,192,0),"��");

	}
	if(explanation)		//�������[�h�̏ꍇ�̂݁A�t���[�`���[�g�̕`��
		DrawGraph(610,0,FlowChartHandle,FALSE);
	if (if_41) {		//���ʏ�(410�R�}���h�ŉ��}�X���j�ł�����)�p�̕`��
		int count = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (if_go[i][j] == true)
					count++;
			}
		}
		DrawFormatString(100, 600, GetColor(0, 0, 0), "���񂾃}�X�̐�:%d", count);
	}
}



#endif