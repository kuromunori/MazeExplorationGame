#include "DxLib.h"
#include "ControlRobot.h"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
/****************************
アルゴリズムを外部から読み込み、実行するためのコード群
****************************/



/****************************

Node:アルゴリズムの1つのノードを指す

****************************/
class Node {
public:
	int id;			//このノードのID
	int com_type;	//このノードが指すコマンド
	int go_id_yes;		//このノードの判定がYesである時に移動するノードのID　(条件分岐コマンドで無いケースもこちらが選ばれる)
	int go_id_no;		//このノードの判定がNoであるときに移動するノードのID
	Node(int id_, int com_type_, int go_id_yes_, int go_id_no_) {
		id = id_;
		com_type = com_type_;
		go_id_yes = go_id_yes_;
		go_id_no = go_id_no_;
	}
};

/****************************

Command_Graph:Nodeの集合
	1つのアルゴリズムを指す

****************************/
class Command_Graph {
public:
	int now_id;					//現在いるノードのID
	int start_id;				//最初に実行されるノードのID
	std::vector<Node> commands;	//ノードの集合(アルゴリズムの本体)
	int count_com;				//実行したコマンド数の記録
	int LoadScript() {	//外部のtxtファイルから自作スクリプト言語を読み込む関数(初期化も兼ねる)
		count_com = 0;
		string str_buf;
		string str_conma_buf;
		string input_csv_file_path = "code.txt";

		ifstream ifs_csv_file(input_csv_file_path);


		getline(ifs_csv_file, str_buf);
		while (getline(ifs_csv_file, str_buf)) {//各行からNodeの情報を読み取りcommandsに追加
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

			if (com_name == "start") {		//startコマンドだけ、Nodeではなくstart_idとして記録
				now_id = go_id_yes;
				start_id = now_id;
			}
			else if (com_name == "go") {	//その他のコマンドは対応するコマンドタイプを記録し、Nodeへと変換
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
			if (com_name != "start" && com_type != -1) {	//正常なコマンドであればcommandsに追加
				commands.push_back(Node(id, com_type, go_id_yes, go_id_no));
			}
		}
		return 0;
	}
	int DoOneStep(double speed) {		//1コマンド進める関数　変数speedは時間のステップ幅(アニメーションのスピード)
		static double time = 0;
		time += speed;
		if (time < 50) {			//50フレームごとに1コマンド実行
			return 0;
		}
		if (speed > 100) {			//スピードが50×Nの時はN回実行(Nは切り捨て)
			DoOneStep(speed - 50);
		}
		count_com++;
		time = 0;

		for (auto ite = commands.begin(); ite != commands.end(); ite++) {
			if ((*ite).id == now_id) {		//現在のコマンドを探し、対応するアクションを実行
				now_id = (*ite).go_id_yes;	//現在のコマンドIDのアップデート　条件分岐でNoになった場合は後で上書き
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
	void DrawNowCommand() {			//現在の実行コマンドを描画する関数
		DrawPlayer();
		char com_types[14][10] = {"前","右","左","+","-","星","東","西","北","南","壁","@マ","前マ","壊"};
		int com_id;
		for (auto ite = commands.begin(); ite != commands.end(); ite++) {
			if ((*ite).id == now_id) {
				com_id = (*ite).com_type;
			}
		}
		DrawFormatString(20, 650, GetColor(0, 0, 0), "%dコマンド", commands.size());
		DrawFormatString(340, 650, GetColor(0, 0, 0), "ステップ数%d回", count_com);
		DrawFormatString(200,530,GetColor(0,0,0),"コマンドID:%d %s",now_id,com_types[com_id]);
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
				DrawFormatString(250, 580, GetColor(0, 0, 0), "Yesへ分岐");
			else
				DrawFormatString(250, 580, GetColor(0, 0, 0), "Noへ分岐");

		}
	};
	void Init() {			//初期化関数
		now_id = start_id;
		count_com = 0;
	}
	void Check41() {		//特別賞の判定用(第41回開催にちなみ、410コマンド目まででより多くのマスを踏んだチームを表彰した)
		while (count_com != 410) {
			DoOneStep(100);
		}
	}
};

