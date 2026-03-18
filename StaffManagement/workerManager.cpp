#include "workerManager.h"
//构造函数
WorkerManager::WorkerManager() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	//1.文件不存在
	if (!ifs.is_open()) {
		cout << "文件不存在" << endl;
		this->empNum = 0;//初始化人数
		this->empArray = NULL;//初始化数组指针
		this->fileIsEmpty = true;//文件空
		ifs.close();
		return;
	}
	//2.文件存在但为空
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		cout << "文件为空" << endl;
		this->empNum = 0;
		this->empArray = NULL;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}
	//3.文件存在且保存数据
	// 第一步：先统计出员工数量
	int num = this->getEmpNum();
	//cout << "职工个数：" << num << endl;
	this->empNum = num;
	// 第二步：根据数量，创建刚好能装下这些员工的数组。empArray是指针，不创建数组empArray就没有指向，就是野指针
	this->empArray = new Worker * [this->empNum];
	// 第三步：把每个员工的指针放进这个数组里
	//初始化职工
	this->initEmp();
	//测试代码
	//for (int i = 0; i < this->empNum; i++) {
	//	cout << this->empArray[i]->ID << " ";
	//	cout << this->empArray[i]->name << " ";
	//	cout << this->empArray[i]->DeptID << endl;
	//}

}
//析构函数
WorkerManager::~WorkerManager() {
	if (this->empArray != NULL) {
		for (int i = 0; i < this->empNum; i++) {
			if (this->empArray[i] != NULL) {
				delete this->empArray[i];
			}
		}
		delete[] this->empArray;
		this->empArray = NULL;
	}
}
//菜单
void WorkerManager::ShowMenu() {
	cout << "********************************************" << endl;
	cout << "*********  欢迎使用职工管理系统！ **********" << endl;
	cout << "*************  0.退出管理程序  *************" << endl;
	cout << "*************  1.增加职工信息  *************" << endl;
	cout << "*************  2.显示职工信息  *************" << endl;
	cout << "*************  3.删除离职职工  *************" << endl;
	cout << "*************  4.修改职工信息  *************" << endl;
	cout << "*************  5.查找职工信息  *************" << endl;
	cout << "*************  6.按照编号排序  *************" << endl;
	cout << "*************  7.清空所有文档  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

//添加职工
void WorkerManager::addEmploy() {
	cout << "请输入增加职工的数量：" << endl;

	int addNum = 0;
	cin >> addNum;

	if (addNum > 0) {
		//计算新空间的大小
		int newSize = this->empNum + addNum;
		//开辟新空间
		Worker** newSpace = new Worker*[newSize];
		/*
		①在堆区创建建一个普通员工对象，地址是0x100
		Worker* p = new Employee(1001, "张三", 1); p里存的是0x100
		②如果有多个员工对象，那就需要很多个（文件）Worker*，这时候就需要指针数组（文件夹）Worker*[newSize]
		③该指针数组Worker*[newSize]有很多格子[][][].....，每一个格子都可以存放一个Worker*，[Worker*][Worker*].....
		④newSpace二级指针，存放（文件夹的地址）数组的首地址，拿着newSpace，就能找到文件夹；打开文件夹的第i个格子（newSpace[i]），就能拿到第i个员工的门牌号worker（Worker*）；
		⑤一级指针（Worker*）→ 指向「Worker 对象」；二级指针（Worker**）→ 指向「Worker 指针」
		*/
		//如果原空间非空
		if (empArray != NULL) {
			//将原空间的内容拷贝到新空间
			for (int i = 0; i < this->empNum; i++) {
				newSpace[i] = this->empArray[i];//指针偏移：newSpace[i]等价于*(newSpace + i)
			}
		}
		//输入新数据
		for (int i = 0; i < addNum; i++) {
			int id;//编号
			string name;//姓名
			int dSelect;//职务

			//防止职工编号重复
			while (1) {
				cout << "请输入第" << i + 1 << "个新职工编号：" << endl;
				cin >> id;

				int ret = empIsExist(id);
				if (ret != -1) {
					cout << "职工编号重复，请重新输入" << endl;
				}
				else {
					break;
				}
			}

			cout << "请输入第" << i + 1 << "个新职工姓名：" << endl;
			cin >> name;
			while (1) {
				cout << "请输入该职工的岗位：" << endl;
				cout << "1：普通职工" << endl;
				cout << "2：经理" << endl;
				cout << "3：老板" << endl;
				cin >> dSelect;
				if (dSelect == 1 || dSelect == 2 || dSelect == 3) {
					//父类指针
					Worker* worker = NULL;//worker存放的是能找到员工的地址

					switch (dSelect) {
					case 1:
						worker = new Employee(id, name, 1);//父类指针指向子类对象
						break;
					case 2:
						worker = new Manager(id, name, 2);
						break;
					case 3:
						worker = new Boss(id, name, 3);
						break;
					default:
						break;
					}
					newSpace[i + this->empNum] = worker;
					break;//跳出while循环
				}
				else {
					cout << "输入有误，请重新输入" << endl;
				}
			}
		}
		//释放原对象
		delete[] this->empArray;
		//更改新的空间指向
		this->empArray = newSpace;
		//更新员工个数
		this->empNum = newSize;
		//提示信息
		cout << "成功添加" << addNum << "名员工信息" << endl;
		//标注文件非空
		this->fileIsEmpty = false;
		//保存数据
		this->save();
	}
	else {
		cout << "输入有误，请重新输入" << endl;
	}
	system("pause");//停住：请按任意键继续
	system("cls");//清屏
}

//保存职工数据
void WorkerManager::save() {
	ofstream ofs;
	ofs.open(FILENAME, ios::out);
	for (int i = 0; i < this->empNum; i++) {
		ofs << this->empArray[i]->ID << " "
			<< this->empArray[i]->name << " "
			<< this->empArray[i]->DeptID << endl;
	}
	ofs.close();
}

//获取文件中人数
int WorkerManager::getEmpNum() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;

	int num = 0;

	//只有三个数据都返回真，读取成功，才继续读下一个人
	while (ifs >> id && ifs >> name && ifs >> dId) {
		num++;
	}
	ifs.close();
	return num;
}

//初始化员工
void WorkerManager::initEmp() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);

	int id;
	string name;
	int dId;

	int index = 0;
	while (ifs >> id && ifs >> name && ifs >> dId) {
		Worker* worker = NULL;
		switch (dId) {
			case 1:
				worker = new Employee(id, name, 1);
				break;
			case 2:
				worker = new Manager(id, name, 2);
				break;
			case 3:
				worker = new Boss(id, name, 3);
				break;
			default:
				break;
		}
		this->empArray[index] = worker;
		index++;
	}
}

//显示职工
void WorkerManager::showEmp() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或文件为空" << endl;
	}
	else {
		for (int i = 0; i < this->empNum; i++) {
			this->empArray[i]->showInfo();//「先通过二级指针的数组访问拿到 Worker* 指针，再通过这个指针调用虚函数（多态）」
		}
	}
	system("pause");
	system("cls");
}

//删除职工
void WorkerManager::delEmp() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或文件为空" << endl;
	}
	else {
		cout << "请输入要删除的职工编号：" << endl;
		int id;
		cin >> id;
		int index = this->empIsExist(id);

		if (index != -1) {
			//删除的本质是数组数据前移覆盖
			for (int i = index; i < this->empNum - 1; i++) {
				//empNum：0 1 2 3 4 5
				//id：1 2 3 4 5 6
				//删除id = 2，index = 1，数组2号下标数据给1,3号给2，依次类推，从index起总共进行empNum-1次
				this->empArray[i] = this->empArray[i + 1];
			}
			this->empNum--;
			this->save();//同步到文件
			cout << "删除成功" << endl;
		}
		else {
			cout << "删除失败，未找到该职工" << endl;
		}
	}
	system("pause");
	system("cls");
}

//查找职工是否存在
int WorkerManager::empIsExist(int id) {
	int index = -1;//先假设不存在，在数组中的位置是-1

	for (int i = 0; i < this->empNum; i++) {
		if ( this->empArray[i]->ID == id) {
			index = i;
			break;
		}
	}
	return index;
}
int WorkerManager::empIsExist(string name, int* resultArr) {
	int count = 0;

	for (int i = 0; i < this->empNum; i++) {
		if (this->empArray[i]->name == name) {
			resultArr[count] = i;
			count++;
		}
	}
	return count;
}

//修改职工
void WorkerManager::modEmp() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或文件为空" << endl;
	}
	else {
		cout << "请输入要修改的职工编号：" << endl;
		int id;
		cin >> id;
		int index = this->empIsExist(id);

		if (index != -1) {
			delete this->empArray[index];//释放指针所指向的堆区对象，并不释放数组格子

			int newID = 0;
			string newName = "";
			int newDeptID = 0;

			cout << "查询到" << id << "号员工" << endl;
			cout << "请输入新职工号：" << endl;
			cin >> newID;

			cout << "请输入修改后的姓名：" << endl;
			cin >> newName;

			while (1) {
				cout << "请输入修改后的岗位：" << endl;
				cout << "1.普通职工" << endl;
				cout << "2.经理" << endl;
				cout << "3.老板" << endl;
				cin >> newDeptID;

				if (newDeptID == 1 || newDeptID == 2 || newDeptID == 3) {
					Worker* worker = NULL;//Worker中有纯虚函数，是虚类，不允许实例化对象
					switch (newDeptID) {
						case 1:
							worker = new Employee(newID, newName, 1);
							break;
						case 2:
							worker = new Manager(newID, newName, 2);
							break;
						case 3:
							worker = new Boss(newID, newName, 3);
							break;
						default:
							break;
					}
					this->empArray[index] = worker;//传递地址
					cout << "修改成功！" << endl;
					this->save();//保存到文件中
					break;
				}
				else {
					cout << "输入有误，请重新输入" << endl;
				}
			}
		}
		else {
			cout << "修改失败，未找到该职工" << endl;
		}
	}
	system("pause");
	system("cls");
}

//查找职工信息
void WorkerManager::findEmp() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或文件为空" << endl;
	}
	else {
		cout << "请输入查找的方式：" << endl;
		cout << "1.按照职工编号查找" << endl;
		cout << "2.按照职工姓名查找" << endl;
		int select = 0;
		cin >> select;

		if (select == 1) {
			cout << "请输入要查找的职工编号：" << endl;
			int id = 0;
			cin >> id;

			int ret = this->empIsExist(id);
			if (ret != -1) {
				cout << "查找成功！" << endl;
				cout << "职工的信息如下：" << endl;
				this->empArray[ret]->showInfo();
			}
			else {
				cout << "查无此人" << endl;
			}
		}
		else if (select == 2) {
			cout << "请输入要查找的职工姓名：" << endl;
			string name;
			cin >> name;

			int indArr[10];//预留10个重名空间
			int	count = this->empIsExist(name, indArr);//重名人数
			if (count != 0) {
				cout << "查找成功！" << endl;
				cout << "职工的信息如下：" << endl;
				for (int i = 0; i < count; i++) {
					this->empArray[indArr[i]]->showInfo();
				}
			}
			else {
				cout << "查无此人" << endl;
			}
		}
		else {
			cout << "输入的选项有误" << endl;
		}
	}
	system("pause");
	system("cls");
}

//排序职工
void WorkerManager::sortEmp() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或文件为空" << endl;
		system("pause");
		system("cls");
	}
	else {
		
	L00P:
		cout << "请输入排序方法：" << endl;
		cout << "1.按照职工号升序排序" << endl;
		cout << "2.按照职工号降序排序" << endl;

		int select = 0;
		cin >> select;

		for (int i = 0; i < this->empNum; i++) {
			int minOrmax = i;//先假设最小或最大
			for (int j = i + 1; j < this->empNum; j++) {
				if (select == 1) {
					//升序：由小到大
					if (this->empArray[minOrmax]->ID > this->empArray[j]->ID) {
						minOrmax = j;
					}
				}
				else if (select == 2) {
					//降序：由大到小
					if (this->empArray[minOrmax]->ID < this->empArray[j]->ID) {
						minOrmax = j;
					}
				}
				else {
					cout << "输入有误请重新输入" << endl;
					goto L00P;
				}
			}
			if (i != minOrmax) {
				Worker* worker = this->empArray[i];
				this->empArray[i] = this->empArray[minOrmax];
				this->empArray[minOrmax] = worker;
			}
		}
		cout << "排序成功！排序结果为：" << endl;
		this->save();
		this->showEmp();
	}
}

//清空数据
void WorkerManager::cleanEmp() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或文件为空" << endl;
	}
	else {
		cout << "确定清空？" << endl;
		cout << "1.确定" << endl;
		cout << "2.取消" << endl;

		int select = 0;
		cin >> select;

		if (select == 1) {
			ofstream(FILENAME, ios::trunc);//如果文件存在，先删除再创建
			if (this->empArray != NULL) {//大柜子的地址不为空
				for (int i = 0; i < this->empNum; i++) {
					if (this->empArray[i] != NULL) {//当前小柜子里的“员工门牌号”是否非空
						delete this->empArray[i];//释放该员工门牌号指向的堆区员工对象
						this->empArray[i] = NULL;//将小柜子里的门牌号置空
					}
				}
				this->empNum = 0;
				delete[] this->empArray;
				this->empArray = NULL;
				this->fileIsEmpty = true;
			}
			cout << "清空成功！" << endl;
		} 
	}
	system("pause");
	system("cls");
}

//退出系统
void WorkerManager::exitSystem() {
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}