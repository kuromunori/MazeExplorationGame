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
* ロボットや盤面の状態を更新する用のコード群
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
bool if_go[N][N] = {0};		//各マスが踏破済みかを記録
bool wall[2][N][N] = {		//縦横の壁の情報
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
int mission[5][2]={	{1,1},	//ミッションである踏むべき星マスの位置座標
					{2,7},
					{4,3},
					{7,2},
					{7,6}};
int FlowChartHandle;		//フローチャートの画像ハンドル
char player[5]="^>v<";
int orient = 2;				//ロボットの向き0:上,1:右,2:下,3:左
int x=0,y=4;				//ロボットの座標
//int turn = 0;
bool marker[N][N] = {0};	//各マスにマーカーが置いてあるか記録

void turn_left(){			//左回転
	orient--;
	orient+=4;
	orient%=4;
}
void turn_right(){			//右回転
	orient++;
	orient%=4;
}
bool if_wall(int orient_ = 0){	//壁が存在するか判定　基本ロボットの前方の壁を見るが、引数orient_で前後左右の判定も可能
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
void go_straight(){		//前に移動　壁がある場合はその場にとどまる
	if_go[x][y] = true;	//移動前のマスを踏破済みに設定
	if(orient == 0 && !wall[0][x][y])
		x = (x-1+N)%N;
	if(orient == 1 && !wall[1][x][(y+1)%N])
		y = (y+1)%N;
	if(orient == 2 && !wall[0][(x+1)%N][y])
		x = (x+1)%N;
	if(orient == 3 && !wall[1][x][y%N])
		y = (y-1+N)%N;
	if_go[x][y] = true;	//移動後のマスを踏破済みに設定
}
void put_marker(){		//現在のマスにマーカーを置く
	marker[x][y] = true;
}
void remove_marker(){	//現在のマスからマーカーを取り除く
	marker[x][y] = false;
}
bool read_marker(int location = 0){	//マーカーの有無の判定　location=0:その場,1:前,2:右,3:後,4:左をチェック
	if(location == 0)
		return marker[x][y];
	if(if_wall(location-1))			//目の前が壁の場合、マーカーが見えないため、実際に置いてあるか無関係にFalseが返る
		return 0;
	location = (orient+location+3)%4;
	if(location == 0){				//画面外を参照しないような処理
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
bool if_star() {					//今いるマスが星マスかどうか判定
	for (int i = 0; i < 5; i++) {
		if (mission[i][0] == x && mission[i][1] == y)
			return true;
	}
	return false;
}
void break_wall() {	//迷路の壁を1度だけ壊す　空振りしても使用済みとなる
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
bool if_north() {	//北を向いているか判定
	return orient % 4 == 0;
}
bool if_east() {	//東を向いているか判定
	return orient % 4 == 1;
}
bool if_south() {	//南を向いているか判定
	return orient % 4 == 2;
}
bool if_west() {	//西を向いているか判定
	return orient % 4 == 3;
}

void InitializeMap() {	//マップの初期化　壁、マーカー、踏破済みか、ロボットの位置、向き　を初期化
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
void DrawPlayer(int _x=y*SIZE, int _y=x*SIZE,int theta=orient*90){	//ロボットを⇒記号で描画 三角形と四角形を組み合わせて⇒を描画
	_x+=SIZE/2+OFSET_X;
	_y+=SIZE/2+OFSET_Y;
	Point tri[3];	//三角形(矢印の先端)
	tri[0] = Point(-SIZE/3,0);
	tri[1] = Point(SIZE/3,0);
	tri[2] = Point(0,-SIZE/3);
	for(int i = 0; i < 3; i++){
		tri[i].Rotate(theta);
		tri[i].x += _x;
		tri[i].y += _y;
	}
	Point box[4];	//四角形(矢印の根本)
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




//ロボットの操作アルゴリズムを実行　およびフローチャート上で現在の実行箇所を描画
// speed:アニメーションの実行スピード
// if_draw:描画の有無の設定
//アルゴリズムは以下
//今いるマスにマーカーがあるなら取り除い前進
//						ないなら置いて右回転し前進
void policy(double speed = 1,bool if_draw = true){	
	static int pre_mode = 0;	//1個前に実行していたコマンド情報(フローチャートの図示に使う)
	static int mode = 0;		//現在実行しているコマンド情報
	static double time = 0;

	if(mode == 0)
		goto Mode0;
	if(mode == 1)
		goto Mode1;
	if(mode == 2)
		goto Mode2;
	if(mode == 3)
		goto Mode3;
Mode0://今いるマスにマーカーがあるかないか判定
	if(read_marker()){
Mode1://マーカーがある場合、取り除く
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
Mode2://マーカーがない場合、おいて右に回転
		time += speed;
		if(speed  < 40){	//スピードアップ用にspeed<40でのみアニメーションを実行
			if( time > 80){			//80<time で次の状態へ遷移
				put_marker();
				turn_right();
				mode = 3;
				pre_mode = 2;
				time = 0;
			}else if( time > 40){	//40<time<80の間で右へ回転
				if(if_draw){
					DrawLine(727,320,727,330,GetColor(0,0,255),2);
					DrawCircle(726,363,33,GetColor(0,0,255) ,0,2);
					DrawPlayer(y*50,x*50,orient*90+(time-40)*90/40);
				}
				put_marker();
				return;
			}else{					//time<40の間は停止
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
		}else{	//speed>40ではアニメーションを全て飛ばす
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
Mode3://前進
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


void LoadFlowChartHandle(){		//フローチャートの画像を読み込む
	FlowChartHandle = LoadGraph("FlowChart.bmp");
}

void PrintBoard(bool explanation,bool if_41 = false){	//盤面および周辺情報の描画
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(wall[0][j][i]){	//壁の描画
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+(i+1)*SIZE,OFSET_Y+j*SIZE,GetColor(0,0,0),5);
			}else{
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+(i+1)*SIZE,OFSET_Y+j*SIZE,GetColor(0,0,0),1);
			}
			if(wall[1][j][i]){
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+i*SIZE,OFSET_Y+(j+1)*SIZE,GetColor(0,0,0),5);
			}else{
				DrawLine(OFSET_X+i*SIZE,OFSET_Y+j*SIZE,OFSET_X+i*SIZE,OFSET_Y+(j+1)*SIZE,GetColor(0,0,0),1);
			}
			if(if_go[j][i])	//踏破済みのマスの描画
				DrawBox(OFSET_X+i*SIZE+2,OFSET_Y+j*SIZE+2,OFSET_X+(i+1)*SIZE-2,OFSET_Y+(j+1)*SIZE-2,GetColor(180,180,255),true);
		}
	}
	DrawLine(OFSET_X,OFSET_Y+N*SIZE,OFSET_X+N*SIZE,OFSET_Y+N*SIZE,GetColor(0,0,0),5);
	DrawLine(OFSET_X+N*SIZE,OFSET_Y,OFSET_X+N*SIZE,OFSET_Y+N*SIZE,GetColor(0,0,0),5);

	for(int i = 0; i < N; i++){	//マーカーの描画
		for(int j = 0; j < N; j++){
			if(marker[i][j])
				DrawCircle(OFSET_X+j*SIZE+SIZE/2,OFSET_Y+i*SIZE+SIZE/2,SIZE/5,GetColor(255,0,0),0,3);
		}
	}
	SetFontSize(40);	//星マスの描画
	for(int i = 0; i < 5;i++){
		DrawFormatString(OFSET_X+3+mission[i][0]*SIZE,OFSET_Y+3+mission[i][1]*SIZE,GetColor(255,192,0),"☆");

	}
	if(explanation)		//説明モードの場合のみ、フローチャートの描画
		DrawGraph(610,0,FlowChartHandle,FALSE);
	if (if_41) {		//特別賞(410コマンドで何マス踏破できたか)用の描画
		int count = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (if_go[i][j] == true)
					count++;
			}
		}
		DrawFormatString(100, 600, GetColor(0, 0, 0), "踏んだマスの数:%d", count);
	}
}



#endif