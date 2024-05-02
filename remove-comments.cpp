#include <iostream>
#include <fstream>
#include <string>

void usage()
{
	std::cout << "Usage: ./remove-comments <mode> <file1> [file2] [...]\n";
	std::cout << "<mode>:\n";
	std::cout << "\t--c : Comments like C/C++/JS code\n";
	std::cout << "\t--python : Comments like python code\n";
	exit(EXIT_FAILURE);
}

#pragma region test_comments
// ----------------------

// comment 

/* comment */

/*
	comment \
	comment
	// comment
*/

// /* \
*/ comment\
comment */


// comment \
	comment \
	comment	

/*
comment
// comment
\
comment
*/

// /* /* comment */ /* comment */ */ <--does not work

// ----------------------
#pragma endregion

void remove_c_comments(const std::string& filename);

int main(int argc, const char** argv)
{
	if (argc < 3)
		usage();

	// TODO: choose mode by extension
	// maybe add it as a flag

	// TODO: add wildcard support

	// TODO: proper testing

	// TODO: support for UNIX pipes? maybe it's by default ???

	remove_c_comments(argv[2]);

	return 0;
}

void remove_c_comments(const std::string& filename)
{
	std::ifstream in(filename);
	if (!in.good())
	{
		std::cerr << "Could not open " << filename << ", trying to process next files...\n";
		in.close();
		return;
	}

	std::string line;
	std::cout << "---Starting printing the document\n";
	bool line_break = false;
	bool multiline_line_break = false;
	while (!in.eof())
	{
		std::getline(in, line);

		if (multiline_line_break)
		{
			size_t of = line.find("*/");
			if (of != line.npos)
			{
				line = line.substr(of + 2);
				multiline_line_break = false;
			}
			else
			{
				continue;
			}
		}

		size_t offset;
		if (line_break)
		{
			if (line.find('\\') != line.size() - 1)
			{
				line_break = false;
			}
			else
			{
				line_break = true;
			}
			continue;
		}

		if ((offset = line.find("//")) != line.npos)
		{
			if (line.find('\\', offset) == line.size() - 1)
			{
				line_break = true;
			}
			line = line.substr(0, offset);
		}

		offset = line.find("/*");
		size_t offset2 = line.find("*/");

		while (offset != line.npos)
		{
			if (offset2 != line.npos)
			{
				line = line.substr(0, offset) + line.substr(offset2 + 2);
				multiline_line_break = false;
			}
			else
			{
				multiline_line_break = true;
				break;
			}
			offset = line.find("/*");
			offset2 = line.find("*/");
		}
		if (multiline_line_break)
		{
			continue;
		}

		std::cout << line << std::endl;
	}
	std::cout << "---Ended printing the document\n";
}