#include <iostream>
#include <fstream>

void usage()
{
	std::cout << "Usage: ./remove-comments <mode> <file1> [file2] [...]\n";
	std::cout << "<mode>:\n";
	std::cout << "\t--c : Comments like C/C++/JS code\n";
	std::cout << "\t--python : Comments like python code\n";
	exit(EXIT_FAILURE);
}

int main(int argc, const char** argv)
{
	if (argc < 3)
		usage();


	return 0;
}