#include <iostream>
#include "speechManager.h"

using namespace std;

int main() {

	srand((unsigned int)time(NULL));

	SpeechManager sm;

	//for (map<int, Speaker>::iterator it = sm.m.begin(); it != sm.m.end(); it++) {
	//	cout << "选手编号：" << it->first << " 姓名：" << it->second.name << " 得分：" << it->second.score[0] << endl;
	//}

	int choice;

	while (1) {
		sm.showMenu();

		cout << "请输入您的选择：" << endl;
		cin >> choice;

		switch (choice)
		{
		case 1:
			//开始比赛
			sm.startSpeech();
			break;
		case 2:  
			//查看记录
			sm.showRecord();
			break;
		case 3:  
			//清空记录
			sm.clearRecord();
			break;
		case 0:  
			//退出系统
			sm.exitSystem();
			break;
		default:
			cout << "输入有误请重新输入！" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
	
	system("pause");

	return 0;
}