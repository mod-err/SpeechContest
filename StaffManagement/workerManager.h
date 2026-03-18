#pragma once
#include <iostream>
#include <fstream>
#include "worker.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"
using namespace std;

#define FILENAME "empFile.txt"

class WorkerManager {
public:
	WorkerManager();
	~WorkerManager();
	//菜单
	void ShowMenu();
	//退出系统
	void exitSystem();
	//添加职工
	void addEmploy();
	//保存职工数据
	void save();
	//获取文件中人数
	int getEmpNum();
	//初始化员工
	void initEmp();
	//标注文件是否为空
	bool fileIsEmpty;
	//显示职工
	void showEmp();
	//删除职工
	void delEmp();
	//查找职工是否存在
	int empIsExist(int id);
	int empIsExist(string name, int* resultArr);
	//修改职工
	void modEmp();
	//查找职工信息
	void findEmp();
	//排序职工
	void sortEmp();
	//清空数据
	void cleanEmp();
	//记录文件中人数
	int empNum;
	//员工数组指针
	Worker** empArray;
};