#include "DxLib.h"
#include "HanchoRobotCommand.h"
#include "CommandGraph.h"
#include <iostream>
#include "Input.h"
#define explanation_mode 1
#define load_script_mode 0
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	int mode = explanation_mode;	//explanation_mode：解説用アルゴリズムのアニメーションを実行するモード
									//load_script_mode：外部テキストファイルに書かれた自作スクリプト言語を読み、
														//読み込んだアルゴリズムを実行するモード
	SetBackgroundColor(255,255,255);
	if (mode == explanation_mode) {
		SetGraphMode(1000, 600, 32);
		SetWindowSize(1000, 600);
	}
	else if(mode==load_script_mode){
		SetGraphMode(700, 700, 32);
		SetWindowSize(700, 700);
	}
	ChangeWindowMode(true);
	DxLib_Init(); // DXライブラリ初期化処理
	SetAlwaysRunFlag(TRUE);
	SetDrawScreen( DX_SCREEN_BACK ); //描画先を裏画面に設定
	double speed = 0.25;		//プログラムを実行するスピード
	LoadFlowChartHandle();
	Command_Graph com_graph;
	bool if_41 = false;		//特別賞(410コマンドまで実行した際に踏破できたマスが最多のチーム)の判定用フラグ
	if (mode==load_script_mode)
		if (com_graph.LoadScript() == -1)	//外部のスクリプトファイルの読み込み
			return 0;
		1;
	while(!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()){
		gpUpdateKey();
		if(GetKey(KEY_INPUT_UP) %10 == 1)	//矢印上キー入力でスピードアップ
			speed*=2;
		if(GetKey(KEY_INPUT_DOWN) %10 == 1)	//矢印下キー入力でスピードダウン
			speed/=2.0;
		if(speed < 0.25)
			speed = 0.25;
		if(GetKey(KEY_INPUT_SPACE) == 1)	//スペースキー入力でスピード初期化
			speed = 1;
		if(speed > 201)
			speed = 201;
		if (GetKey(KEY_INPUT_RETURN) == 1) {//エンターキーで特別賞の結果表示と切り替え(load_script_modeでのみ作動)
			if_41 = !(if_41);
			InitializeMap();
			com_graph.Init();
			if (if_41) {
				com_graph.Check41();
			}
		}
		if (mode==explanation_mode) {
			PrintBoard(mode);
			if (speed > 50)
				policy(speed, false);
			policy(speed);
		}
		else {
			PrintBoard(mode,if_41);
			if (if_41) {
				DrawPlayer();
			}
			else {
				com_graph.DoOneStep(speed);
				com_graph.DrawNowCommand();
				DrawPlayer();
			}
		}
		
	}
	
	DxLib_End();
	return 0;
}