#include <iostream>

using namespace std;

int main()
{
	int *a;
	int b = 10;
	a = &b;

	cout << a << "\t" << *a << endl;
	
	cout << "it's ok" << endl;
	system("pause");
}