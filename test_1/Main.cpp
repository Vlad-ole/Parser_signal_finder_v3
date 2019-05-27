//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <omp.h> 


using namespace std;

int main(int argc, char **argv)
{

	ostringstream file_name_raw_out_osst;
	cout << "test" << std::setfill('0') << std::setw(6) << 123 << "__" << std::setfill('0') << std::setw(6) << 456 << ".dat" << endl;

	cout << "all is ok" << endl;
	system("pause");
}