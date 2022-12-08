#include "DxLib.h"
#include "HanchoRobotCommand.h"
#include "CommandGraph.h"
#include <iostream>
#include "Input.h"
#define explanation_mode 1
#define load_script_mode 0
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	int mode = explanation_mode;	//explanation_mode�F����p�A���S���Y���̃A�j���[�V���������s���郂�[�h
									//load_script_mode�F�O���e�L�X�g�t�@�C���ɏ����ꂽ����X�N���v�g�����ǂ݁A
														//�ǂݍ��񂾃A���S���Y�������s���郂�[�h
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
	DxLib_Init(); // DX���C�u��������������
	SetAlwaysRunFlag(TRUE);
	SetDrawScreen( DX_SCREEN_BACK ); //�`���𗠉�ʂɐݒ�
	double speed = 0.25;		//�v���O���������s����X�s�[�h
	LoadFlowChartHandle();
	Command_Graph com_graph;
	bool if_41 = false;		//���ʏ�(410�R�}���h�܂Ŏ��s�����ۂɓ��j�ł����}�X���ő��̃`�[��)�̔���p�t���O
	if (mode==load_script_mode)
		if (com_graph.LoadScript() == -1)	//�O���̃X�N���v�g�t�@�C���̓ǂݍ���
			return 0;
		1;
	while(!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()){
		gpUpdateKey();
		if(GetKey(KEY_INPUT_UP) %10 == 1)	//����L�[���͂ŃX�s�[�h�A�b�v
			speed*=2;
		if(GetKey(KEY_INPUT_DOWN) %10 == 1)	//��󉺃L�[���͂ŃX�s�[�h�_�E��
			speed/=2.0;
		if(speed < 0.25)
			speed = 0.25;
		if(GetKey(KEY_INPUT_SPACE) == 1)	//�X�y�[�X�L�[���͂ŃX�s�[�h������
			speed = 1;
		if(speed > 201)
			speed = 201;
		if (GetKey(KEY_INPUT_RETURN) == 1) {//�G���^�[�L�[�œ��ʏ܂̌��ʕ\���Ɛ؂�ւ�(load_script_mode�ł̂ݍ쓮)
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