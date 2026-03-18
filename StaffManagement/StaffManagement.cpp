#include <iostream>
#include "workerManager.h"
#include "worker.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"

using namespace std;

int main() {

	WorkerManager wm;
	int choice = 0;
	while (1) {
		wm.ShowMenu();

		cout << "请输入您的选择：" << endl;
		cin >> choice;

		switch (choice) {
		case 0://退出系统
			wm.exitSystem();
			break;
		case 1://增加信息
			wm.addEmploy();
			break;
		case 2://显示信息
			wm.showEmp();
			break;
		case 3://删除信息
			wm.delEmp();
			break;
		case 4://修改信息
			wm.modEmp();
			break;
		case 5://查找信息
			wm.findEmp();
			break;
		case 6://排序信息
			wm.sortEmp();
			break;
		case 7://清空文件
			wm.cleanEmp();
			break;
		default:
			system("cls");
			break;
		}
	}
	system("pause");
	return 0;
}
