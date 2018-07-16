#include "create_dataset.h"

using namespace std;

void function()
{
	ifstream input("D:\\out.txt");
	ofstream out;
	out.open("D:\\moves.txt");
	if (input.is_open() && out.is_open()) {
		string line;
		while (getline(input, line)) {
			if (line != "" && line.at(0) == '1') {
				out << line << endl;
			}
		}
	}
}
