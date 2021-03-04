#define _CRT_SECURE_NO_WARNINGS
#include "LoremIpsum.h"
int main(int argc, char* argv[]) {
	ifstream input;
	input.open(argv[1]);
 	UserInterface ui1(argv[1]);
}
