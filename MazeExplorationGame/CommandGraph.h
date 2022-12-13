#include "DxLib.h"
#include "ControlRobot.h"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
/****************************
�A���S���Y�����O������ǂݍ��݁A���s���邽�߂̃R�[�h�Q
****************************/



/****************************

Node:�A���S���Y����1�̃m�[�h���w��

****************************/
class Node {
public:
	int id;			//���̃m�[�h��ID
	int com_type;	//���̃m�[�h���w���R�}���h
	int go_id_yes;		//���̃m�[�h�̔��肪Yes�ł��鎞�Ɉړ�����m�[�h��ID�@(��������R�}���h�Ŗ����P�[�X�������炪�I�΂��)
	int go_id_no;		//���̃m�[�h�̔��肪No�ł���Ƃ��Ɉړ�����m�[�h��ID
	Node(int id_, int com_type_, int go_id_yes_, int go_id_no_) {
		id = id_;
		com_type = com_type_;
		go_id_yes = go_id_yes_;
		go_id_no = go_id_no_;
	}
};

/****************************

Command_Graph:Node�̏W��
	1�̃A���S���Y�����w��

****************************/
class Command_Graph {
public:
	int now_id;					//���݂���m�[�h��ID
	int start_id;				//�ŏ��Ɏ��s�����m�[�h��ID
	std::vector<Node> commands;	//�m�[�h�̏W��(�A���S���Y���̖{��)
	int count_com;				//���s�����R�}���h���̋L�^
	int LoadScript() {	//�O����txt�t�@�C�����玩��X�N���v�g�����ǂݍ��ފ֐�(�����������˂�)
		count_com = 0;
		string str_buf;
		string str_conma_buf;
		string input_csv_file_path = "code.txt";

		ifstream ifs_csv_file(input_csv_file_path);


		getline(ifs_csv_file, str_buf);
		while (getline(ifs_csv_file, str_buf)) {//�e�s����Node�̏���ǂݎ��commands�ɒǉ�
			istringstream i_stream(str_buf);
			int id, go_id_yes, go_id_no;
			string com_name;
			getline(i_stream, str_conma_buf, ',');
			id = atoi(str_conma_buf.c_str());
			getline(i_stream, str_conma_buf, ',');
			com_name = str_conma_buf;
			getline(i_stream, str_conma_buf, ',');
			go_id_yes = atoi(str_conma_buf.c_str());
			getline(i_stream, str_conma_buf, ',');
			go_id_no = atoi(str_conma_buf.c_str());
			printf("%d\speed%s\t%d\t%d\n", id, com_name.c_str(), go_id_yes, go_id_no);
			int com_type=-1;

			if (com_name == "start") {		//start�R�}���h�����ANode�ł͂Ȃ�start_id�Ƃ��ċL�^
				now_id = go_id_yes;
				start_id = now_id;
			}
			else if (com_name == "go") {	//���̑��̃R�}���h�͑Ή�����R�}���h�^�C�v���L�^���ANode�ւƕϊ�
				com_type = 0;
			}
			else if (com_name == "right") {
				com_type = 1;
			}
			else if (com_name == "left") {
				com_type = 2;
			}
			else if (com_name == "put") {
				com_type = 3;
			}
			else if (com_name == "pop") {
				com_type = 4;
			}
			else if (com_name == "if_star") {
				com_type = 5;
			}
			else if (com_name == "if_east") {
				com_type = 6;
			}
			else if (com_name == "if_west") {
				com_type = 7;
			}
			else if (com_name == "if_north") {
				com_type = 8;
			}
			else if (com_name == "if_south") {
				com_type = 9;
			}
			else if (com_name == "if_wall") {
				com_type = 10;
			}
			else if (com_name == "if_marker") {
				com_type = 11;
			}
			else if (com_name == "if_front_marker") {
				com_type = 12;
			}
			else if (com_name == "break_wall") {
				com_type = 13;
			}
			if (com_name != "start" && com_type != -1) {	//����ȃR�}���h�ł����commands�ɒǉ�
				commands.push_back(Node(id, com_type, go_id_yes, go_id_no));
			}
		}
		return 0;
	}
	int DoOneStep(double speed) {		//1�R�}���h�i�߂�֐��@�ϐ�speed�͎��Ԃ̃X�e�b�v��(�A�j���[�V�����̃X�s�[�h)
		static double time = 0;
		time += speed;
		if (time < 50) {			//50�t���[�����Ƃ�1�R�}���h���s
			return 0;
		}
		if (speed > 100) {			//�X�s�[�h��50�~N�̎���N����s(N�͐؂�̂�)
			DoOneStep(speed - 50);
		}
		count_com++;
		time = 0;

		for (auto ite = commands.begin(); ite != commands.end(); ite++) {
			if ((*ite).id == now_id) {		//���݂̃R�}���h��T���A�Ή�����A�N�V���������s
				now_id = (*ite).go_id_yes;	//���݂̃R�}���hID�̃A�b�v�f�[�g�@���������No�ɂȂ����ꍇ�͌�ŏ㏑��
				switch ((*ite).com_type) {
				case 0:
					go_straight();
					break;
				case 1:
					turn_right();
					break;
				case 2:
					turn_left();
					break;
				case 3:
					put_marker();
					break;
				case 4:
					remove_marker();
					break;
				case 5:
					if (!if_star())
						now_id = (*ite).go_id_no;
					break;
				case 6:
					if (!if_east())
						now_id = (*ite).go_id_no;
					break;
				case 7:
					if (!if_west())
						now_id = (*ite).go_id_no;
					break;
				case 8:
					if (!if_north())
						now_id = (*ite).go_id_no;
					break;
				case 9:
					if (!if_south())
						now_id = (*ite).go_id_no;
					break;
				case 10:
					if (!if_wall())
						now_id = (*ite).go_id_no;
					break;
				case 11:
					if (!read_marker(0))
						now_id = (*ite).go_id_no;
					break;
				case 12:
					if (!read_marker(1))
						now_id = (*ite).go_id_no;
						break;
				case 13:
					break_wall();
					break;

				}
				break;
			}
		}
	}
	void DrawNowCommand() {			//���݂̎��s�R�}���h��`�悷��֐�
		DrawPlayer();
		char com_types[14][10] = {"�O","�E","��","+","-","��","��","��","�k","��","��","@�}","�O�}","��"};
		int com_id;
		for (auto ite = commands.begin(); ite != commands.end(); ite++) {
			if ((*ite).id == now_id) {
				com_id = (*ite).com_type;
			}
		}
		DrawFormatString(20, 650, GetColor(0, 0, 0), "%d�R�}���h", commands.size());
		DrawFormatString(340, 650, GetColor(0, 0, 0), "�X�e�b�v��%d��", count_com);
		DrawFormatString(200,530,GetColor(0,0,0),"�R�}���hID:%d %s",now_id,com_types[com_id]);
		if (com_id >= 5 && com_id <= 12) {
			bool check;
			if (com_id == 5)
				check = if_star();
			if (com_id == 6)
				check = if_east();
			if (com_id == 7)
				check = if_west();
			if (com_id == 8)
				check = if_north();
			if (com_id == 9)
				check = if_south();
			if (com_id == 10)
				check = if_wall();
			if (com_id == 11)
				check = read_marker();
			if (com_id == 12)
				check = read_marker(1);
			if (check)
				DrawFormatString(250, 580, GetColor(0, 0, 0), "Yes�֕���");
			else
				DrawFormatString(250, 580, GetColor(0, 0, 0), "No�֕���");

		}
	};
	void Init() {			//�������֐�
		now_id = start_id;
		count_com = 0;
	}
	void Check41() {		//���ʏ܂̔���p(��41��J�Âɂ��Ȃ݁A410�R�}���h�ڂ܂łł�葽���̃}�X�𓥂񂾃`�[����\������)
		while (count_com != 410) {
			DoOneStep(100);
		}
	}
};

