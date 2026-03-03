#include <iostream>
using namespace std;

int g_a = 10;
int g_b = 10;
const int c_g_a = 10;
const int c_g_b = 10;

int main() {

	//全局区：全局变量，静态变量，字符串常量和const修饰的全局变量
	
	//全局变量
	cout << "全局变量a地址：" << (int)&g_a << endl;
	cout << "全局变量b地址：" << (int)&g_b << endl;
	//静态变量
	static int s_a = 10;
	static int s_b = 10;
	cout << "静态变量a地址：" << (int)&s_a << endl;
	cout << "静态变量b地址：" << (int)&s_b << endl;
	//字符串常量
	cout << "字符串常量地址" << (int)&"HELLO" << endl;
	//const修饰的全局变量
	cout << "const修饰的全局变量a地址：" << (int)&c_g_a << endl;
	cout << "const修饰的全局变量b地址：" << (int)&c_g_b << endl;

	//非全局区：局部变量

	//局部变量
	int l_a = 10;
	int l_b = 10;
	cout << "局部变量a地址：" << (int)&l_a << endl;
	cout << "局部变量b地址：" << (int)&l_b << endl;
	//const修饰的局部变量
	const int c_l_a = 10;
	const int c_l_b = 10;
	cout << "const修饰的局部变量a地址：" << (int)&c_l_a << endl;
	cout << "const修饰的局部变量b地址：" << (int)&c_l_b << endl;

	system("pause");
	return 0;
}