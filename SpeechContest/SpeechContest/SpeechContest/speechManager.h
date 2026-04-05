#pragma once
#include<iostream>
#include<string>
#include<random>
#include<vector>
#include<deque>
#include<map>
#include<algorithm>
#include<numeric>
#include<fstream>
#include<ctime>
#include"speaker.h"

using namespace std;

class SpeechManager {
public:
	//构造函数
	SpeechManager();
	//显示菜单
	void showMenu();
	//退出系统
	void exitSystem();
	//初始化成员
	void initSpeech();
	//创建选手
	void createSpeaker();
	//比赛流程
	void startSpeech();
	//抽签
	void speechDraw();
	//比赛
	void speechContest();
	//显示结果
	void showScore();
	//保存结果
	void saveRecord();
	//读取记录
	void loadRecord();
	//显示往届得分
	void showRecord();
	//清空记录
	void clearRecord();
	//析构函数
	~SpeechManager();

	//成员属性
	vector<int> v1;//第一轮12人
	vector<int> v2;//第二轮6人
	vector<int> victory;//胜利3人
	map<int, Speaker> m;//选手和编号
	int index;//轮数

	bool fileIsEmpty;//文件是否空
	map<int, vector<string>> mRecord;//往届记录
};