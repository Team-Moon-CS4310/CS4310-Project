#include "colors.hpp"
#include <iostream>

using namespace std;

void printColor(Color c, char* msg) {
	switch (c) {
	case YELLOW:
		cout << "\033[1;33m" << msg << "\033[0m" << endl;
		break;
	case BLUE:
		cout << "\033[1;34m" << msg << "\033[0m" << endl;
		break;
	case MAGENTA:
		cout << "\033[1;35m" << msg << "\033[0m" << endl;
		break;
	case RED:
		cout << "\033[1;31m" << msg << "\033[0m" << endl;
		break;
	case GREEN:
		cout << "\033[1;32m" << msg << "\033[0m" << endl;
		break;
	default:
		break;
	}
}

void printColor(Color c, const char* msg) {
	switch (c) {
	case YELLOW:
		cout << "\033[1;33m" << msg << "\033[0m" << endl;
		break;
	case BLUE:
		cout << "\033[1;34m" << msg << "\033[0m" << endl;
		break;
	case MAGENTA:
		cout << "\033[1;35m" << msg << "\033[0m" << endl;
		break;
	case RED:
		cout << "\033[1;31m" << msg << "\033[0m" << endl;
		break;
	case GREEN:
		cout << "\033[1;32m" << msg << "\033[0m" << endl;
		break;
	default:
		break;
	}
}