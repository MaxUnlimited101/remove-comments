#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>

void usage()
{
	std::cout << "Usage: ./remove-comments <file1> [file2] [...]\n" <<
		"Help page: ./remove-comments --help\n";
	exit(EXIT_FAILURE);
}

void help()
{
	std::cout << "Removes comments from file based on its extension.\n" <<
		"For example *.c files are treated as C code, hence '/* */' and '//' are treated as comments\n" <<
		"All supported languages: https://en.wikipedia.org/wiki/Comparison_of_programming_languages_(syntax)#Comments\n";
	exit(EXIT_SUCCESS);
}

void setup_ext_to_line_comm_map(std::unordered_map<std::string, std::string>& ext_to_line_comment_symbols)
{
	// Since this program was intented as a CLI tool, 
	// this should probably be embedded in the executable

	ext_to_line_comment_symbols["adb"] = "--";
	ext_to_line_comment_symbols["ads"] = "--";
	ext_to_line_comment_symbols["e"] = "--";
	ext_to_line_comment_symbols["exw"] = "--";
	ext_to_line_comment_symbols["edb"] = "--";
	ext_to_line_comment_symbols["vhd"] = "--";
	ext_to_line_comment_symbols["scpt"] = "--";
	ext_to_line_comment_symbols["scptd"] = "--";
	ext_to_line_comment_symbols["applescript"] = "--";
	ext_to_line_comment_symbols["applescript"] = "--";
	ext_to_line_comment_symbols["asm"] = ";";
	ext_to_line_comment_symbols["s"] = ";";
	ext_to_line_comment_symbols["inc"] = ";";
	ext_to_line_comment_symbols["wla"] = ";";
	ext_to_line_comment_symbols["ps1"] = "#";
	ext_to_line_comment_symbols["sh"] = "#";
	ext_to_line_comment_symbols["bas"] = "REM";
	ext_to_line_comment_symbols["c"] = "//"; // NOTE: only works from C99
	ext_to_line_comment_symbols["h"] = "//"; // NOTE: only works from C99
	ext_to_line_comment_symbols["cpp"] = "//";
	ext_to_line_comment_symbols["hpp"] = "//";
	ext_to_line_comment_symbols["C"] = "//";
	ext_to_line_comment_symbols["cc"] = "//";
	ext_to_line_comment_symbols["cxx"] = "//";
	ext_to_line_comment_symbols["c++"] = "//";
	ext_to_line_comment_symbols["hh"] = "//";
	ext_to_line_comment_symbols["hpp"] = "//";
	ext_to_line_comment_symbols["hxx"] = "//";
	ext_to_line_comment_symbols["h++"] = "//";
	ext_to_line_comment_symbols["cppm"] = "//";
	ext_to_line_comment_symbols["ixx"] = "//";
	ext_to_line_comment_symbols["go"] = "//";
	ext_to_line_comment_symbols["swift"] = "//";
	ext_to_line_comment_symbols["js"] = "//";
	ext_to_line_comment_symbols["cjs"] = "//";
	ext_to_line_comment_symbols["mjs"] = "//";
	ext_to_line_comment_symbols["ts"] = "//";
	ext_to_line_comment_symbols["v"] = "//";
	ext_to_line_comment_symbols["vsh"] = "//";
	ext_to_line_comment_symbols["vsh"] = "//";
	ext_to_line_comment_symbols["cs"] = "//";
	ext_to_line_comment_symbols["csx"] = "//";
	ext_to_line_comment_symbols["cbl"] = "*>"; // NOTE: only for COBOL 2002
	ext_to_line_comment_symbols["cob"] = "*>"; // NOTE: only for COBOL 2002
	ext_to_line_comment_symbols["cpy"] = "*>"; // NOTE: only for COBOL 2002
	ext_to_line_comment_symbols["cobra"] = "#";
	ext_to_line_comment_symbols["d"] = "//";
	ext_to_line_comment_symbols["ex"] = "//"; // Elixir
	ext_to_line_comment_symbols["exs"] = "//";
	ext_to_line_comment_symbols["fs"] = "\\";
	ext_to_line_comment_symbols["fs"] = "\\";
	ext_to_line_comment_symbols["fth"] = "\\";
	ext_to_line_comment_symbols["4th"] = "\\";
	ext_to_line_comment_symbols["f90"] = "!";
	ext_to_line_comment_symbols["hs"] = "--";
	ext_to_line_comment_symbols["lhs"] = "--";
	ext_to_line_comment_symbols["java"] = "//";
	ext_to_line_comment_symbols["jl"] = "#";
	ext_to_line_comment_symbols["lisp"] = ";";
	ext_to_line_comment_symbols["lua"] = "--";
	ext_to_line_comment_symbols["m"] = "%"; // MATLAB
	ext_to_line_comment_symbols["mat"] = "%";
	ext_to_line_comment_symbols["nim"] = "#";
	ext_to_line_comment_symbols["pas"] = "//"; // Object Pascal
	ext_to_line_comment_symbols["plx"] = "#";
	ext_to_line_comment_symbols["pl"] = "#";
	ext_to_line_comment_symbols["rb"] = "#";
	ext_to_line_comment_symbols["rb"] = "#";
	ext_to_line_comment_symbols["sql"] = "--";
	ext_to_line_comment_symbols["py"] = "#";
	ext_to_line_comment_symbols["pyw"] = "#";
	ext_to_line_comment_symbols["r"] = "#";
	ext_to_line_comment_symbols["rs"] = "//";
	ext_to_line_comment_symbols["rlib"] = "//";
	ext_to_line_comment_symbols["erl"] = "%";
	ext_to_line_comment_symbols["zig"] = "//";

	// PHP and VB .NET are special, they are covered seperately
	// since they have multiple forms of line comments
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

void remove_line_comments(std::istream& in, const std::string& comment_symbol);

void remove_block_comments(std::istream& in,
	const std::string& comment_symbol_start,
	const std::string& comment_symbol_end);

void remove_all_comments(const std::string& filename);

int main(int argc, const char** argv)
{
	if (argc == 1)
		usage();

	if (strcmp(argv[1], "--help"))
		help();
	
	std::unordered_map<std::string, std::string>& ext_to_line_comment_symbols;
	setup_ext_to_line_comm_map(ext_to_line_comment_symbols);

	// TODO: add wildcard support

	// TODO: proper testing

	// TODO: update README on 'remove support for '\' char to break line' ???

	// TODO: update README on file extensions

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
				std::cout << line.substr(0, offset);
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

void remove_line_comments(std::istream& in, const std::string& comment_symbol)
{
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);

		size_t offset;

		if ((offset = line.find(comment_symbol)) != line.npos)
		{
			line = line.substr(0, offset);
		}

		std::cout << line << std::endl;
	}
}

void remove_block_comments(std::istream& in,
	const std::string& comment_symbol_start,
	const std::string& comment_symbol_end)
{
	std::string line;
	bool multiline_line_break = false;
	while (!in.eof())
	{
		std::getline(in, line);

		if (multiline_line_break)
		{
			size_t of = line.find(comment_symbol_end);
			if (of != line.npos)
			{
				line = line.substr(of + comment_symbol_end.size());
				multiline_line_break = false;
			}
			else
			{
				continue;
			}
		}

		size_t offset;
		offset = line.find(comment_symbol_start);
		size_t offset2 = line.find(comment_symbol_end);

		while (offset != line.npos)
		{
			if (offset2 != line.npos)
			{
				line = line.substr(0, offset) + line.substr(offset2 + comment_symbol_end.size());
				multiline_line_break = false;
			}
			else
			{
				multiline_line_break = true;
				std::cout << line.substr(0, offset);
				break;
			}
			offset = line.find(comment_symbol_start);
			offset2 = line.find(comment_symbol_end);
		}
		if (multiline_line_break)
		{
			continue;
		}
		std::cout << line << std::endl;
	}
}

void remove_all_comments(const std::string& filename)
{
	size_t ext_off = filename.find_last_of('.');
	if (ext_off == filename.npos)
	{
		std::cerr << '[' << filename << "] has no extension, trying to process next files...\n";
		return;
	}

	std::ifstream in(filename);
	if (!in.good())
	{
		std::cerr << "Could not open [" << filename << "], trying to process next files...\n";
		in.close();
		return;
	}

	// https://en.wikipedia.org/wiki/Comparison_of_programming_languages_(syntax)#Comments
	std::string ext = filename.substr(0, ext_off);

	for (size_t i = 0; i < ext.size(); i++)
	{
		ext[i] = std::tolower(ext[i]);
	}

	std::ifstream in(filename);
	if (!in.good())
	{
		std::cerr << "Could not open " << filename << ", trying to process next files...\n";
		in.close();
		return;
	}

	if (ext == "php")
	{
		remove_line_comments(in, "#");
		remove_line_comments(in, "//");
		remove_block_comments(in, "/*", "*/");
		return;
	}
	if (ext == "vb")
	{
		remove_line_comments(in, "'");
		remove_line_comments(in, "Rem");
		remove_block_comments(in, "#If COMMENT Then", "#End If");
		return;
	}



}