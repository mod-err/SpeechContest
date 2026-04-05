#include "speechManager.h"

//构造函数
SpeechManager::SpeechManager() {
	initSpeech();

	createSpeaker();
}

//显示菜单
void SpeechManager::showMenu() {
	cout << "****************************************" << endl;
	cout << "************欢迎参加演讲比赛************" << endl;
	cout << "************1、开始演讲比赛*************" << endl;
	cout << "************2、查看往届记录*************" << endl;
	cout << "************3、清空比赛记录*************" << endl;
	cout << "************0、退出比赛程序*************" << endl;
	cout << "****************************************" << endl;
	cout << endl;
}

//退出系统
void SpeechManager::exitSystem() {
	cout << "欢迎下次使用!" << endl;
	system("pause");
	exit(0);
}

//初始化成员
void SpeechManager::initSpeech() {
	this->v1.clear();
	this->v2.clear();
	this->victory.clear();
	this->m.clear();
	this->mRecord.clear();
	this->index = 1;
}

//创建选手
void SpeechManager::createSpeaker() {
	string nameindex = "ABCDEFGHIJKL";

	for (int i = 0; i < nameindex.size(); i++) {

		string name = "选手";
		name += nameindex[i];//name.operator+=(name.operator[](i));

		Speaker s;
		s.name = name;

		s.score[0] = 0;
		s.score[1] = 0;
		
		//12名选手编号
		v1.push_back(i + 10001);

		m.insert(make_pair(i + 10001, s));
	}
}

//比赛流程
void SpeechManager::startSpeech() {
	//第一轮比赛
	
	//1、抽签
	speechDraw();

	//2、比赛
	speechContest();

	//3、显示晋级结果
	showScore();

	//第二轮比赛
	this->index++;

	//1、抽签
	speechDraw();

	//2、比赛
	speechContest();

	//3、显示最终结果
	showScore();

	//4、保存分数
	saveRecord();

	initSpeech();
	createSpeaker();

	system("pause");
	system("cls");
}

//抽签
void SpeechManager::speechDraw() {
	cout << "第 << " << this->index << " >> 轮比赛选手正在抽签" << endl;
	cout << "---------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;

	random_device rd;
	mt19937 g(rd());

	if (index == 1) {
		shuffle(v1.begin(), v1.end(), g);//打乱顺序

		for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
	else {
		shuffle(v2.begin(), v2.end(), g);

		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++) {
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << "---------------------" << endl;
	system("pause");
	cout << endl;
}

//比赛
void SpeechManager::speechContest() {
	cout << "-----第 << " << this->index << " >> 轮比赛正式开始-----" << endl;

	multimap<double, int, greater<double>> groupScore;//分数，编号

	int num = 0; //记录人员数，6个为1组

	vector<int> vSrc;
	if (index == 1) {
		vSrc = v1;
	}
	else {
		vSrc = v2;
	}

	//遍历所有参赛选手
	for (vector<int>::iterator it = vSrc.begin(); it != vSrc.end(); it++) {
		
		num++;

		deque<double> d;
		for (int i = 0; i < 10; i++) {
			double score = (rand() % 4001 + 6000) / 100.f; //直接取余是整数，取余后再缩小便有小数，[60, 100]
			d.push_back(score);
			//cout << d[i] << " ";
		}
		//cout << endl;

		//降序排序
		sort(d.begin(), d.end(), greater<double>());
		d.pop_front();	//去掉最高分
		d.pop_back();	//去掉最低分

		double sum = accumulate(d.begin(), d.end(), 0);
		double avg = sum / (double)d.size();
		//cout << avg << endl;

		//将得分放入map<int, double>容器
		m[*it].score[index - 1] = avg;

		groupScore.insert(make_pair(avg, *it));

		if (num % 6 == 0) {
			cout << "第 " << num / 6 << " 组比赛名次：" << endl;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end(); it++) {
				cout << "编号：" << it->second << " 姓名：" << m[it->second].name << " 得分：" << m[it->second].score[index - 1] << endl;
			}

			int count = 0;

			//取出前三名
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end() && count < 3; it++, count++) {
				if (index == 1) {
					v2.push_back(it->second);
				}
				else {
					victory.push_back(it->second);
				}
			}

			groupScore.clear();
		}
	}
	cout << "-----第" << this->index << "轮比赛完毕!-----" << endl;
	system("pause");
	cout << endl;
}

//显示结果
void SpeechManager::showScore() {

	cout << "----------第" << this->index << "轮晋级选手信息如下：-----------" << endl;

	vector<int> v;

	if (index == 1) {
		v = v2;
	}
	else {
		v = victory;
	}

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "选手编号：" << *it << " 姓名：" << m[*it].name << "  得分：" << m[*it].score[index-1] << endl;
	}
	cout << endl;

	system("pause");
	cout << endl;
}

//保存结果
void SpeechManager::saveRecord() {
	ofstream ofs;

	ofs.open("speech.csv", ios::out | ios::app);

	for (vector<int>::iterator it = victory.begin(); it != victory.end(); it++) {
		ofs << *it << "," << m[*it].score[1] << ",";
	}
	ofs << endl;

	ofs.close();
	cout << "记录已经保存" << endl;
}

//读取记录
void SpeechManager::loadRecord() {
	ifstream ifs;
	ifs.open("speech.csv", ios::in);//输入流对象，读取文件

	if (!ifs.is_open()) {
		this->fileIsEmpty = true;
		cout << "文件不存在！" << endl;
		ifs.close();
		return;
	}

	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		cout << "文件为空!" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	this->fileIsEmpty = false;
	ifs.putback(ch); //放回读取到的1字节

	string data = "";
	int index = 1;//届数   

	while (ifs >> data) {
		//cout << data << endl;
		
		//每一届都用一个vector容器存储
		vector<string> v;
		
		int pos = -1;
		int start = 0;

		while (1) {
			pos = data.find(",", start);

			if (pos == -1) {
				break;
			}

			string temp = data.substr(start, pos - start);
			v.push_back(temp);

			start = pos + 1;
			//cout << temp << endl;
		}

		mRecord.insert(make_pair(index, v));
		index++;
	}

	ifs.close();
}

//显示往届得分
void SpeechManager::showRecord() {
	loadRecord();

	for (map<int, vector<string>>::iterator it = mRecord.begin(); it != mRecord.end(); it++) {
		cout << "第 " << it->first << " 届" 
			 << " 冠军编号：" << it->second[0] << " 得分：" << (*it).second[1] 
			 <<	" 亚军编号：" << it->second[2] << " 得分：" << (*it).second[3]
			 << " 季军编号：" << it->second[4] << " 得分：" << (*it).second[5]
		<< endl;
	}

	//for (int i = 1; i < mRecord.size() + 1; i++) {
	//	cout << "第 " << i << " 届"
	//		 << " 冠军编号：" << mRecord[i][0] << " 得分：" << mRecord[i][1]//mRecord.operator[](i).operator[](0)
	//		 << " 亚军编号：" << mRecord[i][2] << " 得分：" << mRecord[i][3]
	//		 << " 季军编号：" << mRecord[i][4] << " 得分：" << mRecord[i][5]
	//		<< endl;
	//}

	system("pause");
	system("cls");
}

//清空记录
void SpeechManager::clearRecord() {
	int select;

	cout << "您确定要清空记录吗？" << endl;
	cout << "1、确定" << endl;
	cout << "2、返回" << endl;

	cin >> select;

	if (select == 1) {
		ofstream ofs("speech.csv", ios::trunc);
		ofs.close();

		//初始化属性
		this->initSpeech();
		//创建选手
		this->createSpeaker();	

		cout << "清空成功！" << endl;
	}
	system("pause");
	system("cls");
}

//析构函数
SpeechManager::~SpeechManager() {

}