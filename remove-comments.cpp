#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <xutility>
#include <regex>
#include <filesystem>

void usage()
{
	std::cout << "Usage: ./remove-comments <line comments file> <block comments file> <file1 | regex> [file2 | regex] [...]\n" <<
		"Help page: ./remove-comments --help\n";
	exit(EXIT_FAILURE);
}

void help()
{
	std::cout << "Removes comments from file based on 'comment files'.\n" <<
		"'Comment files' contain information about different languages in a specific format\n" <<
		"Example 'line comments file'(without'\\t')\n" <<
		"\t<FILE_EXTENSION_1>:<LINE_COMMENT_DEFINITION_1>\n" <<
		"\t<FILE_EXTENSION_2>:<LINE_COMMENT_DEFINITION_2>\n" <<
		"Example 'block comments file':\n" <<
		"\t<FILE_EXTENSION_1>:<BLOCK_COMMENT_BEGINNING_DEFINITION_1>QQQ<BLOCK_COMMENT_ENDING_DEFINITION_1>\n" <<
		"\t<FILE_EXTENSION_2>:<BLOCK_COMMENT_BEGINNING_DEFINITION_2>QQQ<BLOCK_COMMENT_ENDING_DEFINITION_2>\n" <<
		"Example for C++: Line file: \"cpp://\", Block file: cpp:/*QQQ*/\n" << 
		"Note: QQQ should be included, hence this puts a certain limitation on flexibility of this tool, " <<
		"but no language should use QQQ as part of defining a comment\n" <<
		"Also note that support for most used known languages is built in\n" <<
		"Also note that instead of specifying filenames you can use REGEX expressions. \n" <<
		"A good example would be './.*.c' -> all C files in current directory.\n";

	exit(EXIT_SUCCESS);
}

using line_comments_map = std::unordered_map<std::string, std::vector<std::string>>;
using block_comments_map = std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>;

void remove_line_comments(std::vector<std::string>& file, const std::string& comment_symbol);

void remove_block_comments(std::vector<std::string>& file,
	const std::string& comment_symbol_start,
	const std::string& comment_symbol_end);

void remove_all_comments(const std::string& filename, const line_comments_map& line_map, const block_comments_map& block_map);

void fill_comments_maps(line_comments_map& line_map, block_comments_map& block_map,
	const std::string& line_file, const std::string& block_file);

std::vector<std::string> globPattern(const std::string& pattern, const std::string& directory);

int main(int argc, const char** argv)
{
	if (argc == 2 && std::strcmp(argv[1], "--help") == 0)
		help();

	if (argc < 3)
		usage();

	line_comments_map ext_to_line_comment_symbols;
	block_comments_map ext_to_block_comment_symbols;

	// add basic built-in support for most know languages
	ext_to_line_comment_symbols["c"] = { "//" };
	ext_to_line_comment_symbols["cpp"] = { "//" };
	ext_to_line_comment_symbols["cxx"] = { "//" };
	ext_to_line_comment_symbols["h"] = { "//" };
	ext_to_line_comment_symbols["hpp"] = { "//" };
	ext_to_line_comment_symbols["js"] = { "//" };
	ext_to_line_comment_symbols["ts"] = { "//" };
	ext_to_line_comment_symbols["java"] = { "//" };
	ext_to_line_comment_symbols["cs"] = { "//" };
	ext_to_line_comment_symbols["py"] = { "#" };
	ext_to_line_comment_symbols["php"] = { "#", "//"};
	ext_to_line_comment_symbols["go"] = { "//" };
	ext_to_line_comment_symbols["rs"] = { "//" };
	ext_to_line_comment_symbols["rlib"] = { "//" };
	ext_to_line_comment_symbols["sql"] = { "--" };

	ext_to_block_comment_symbols["c"] = { {"/*", "*/"}};
	ext_to_block_comment_symbols["cpp"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["cxx"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["h"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["hpp"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["js"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["ts"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["java"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["cs"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["py"] = { {"'''", "'''"}, {"\"\"\"", "\"\"\""}};
	ext_to_block_comment_symbols["php"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["go"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["rs"] = { {"/*", "*/"} };
	ext_to_block_comment_symbols["rlib"] = { {"/*", "*/"} };

	fill_comments_maps(ext_to_line_comment_symbols, ext_to_block_comment_symbols, argv[1], argv[2]);

	for (int i = 3; i < argc; i++)
	{
		std::filesystem::path p(argv[i]);
		std::vector<std::string> files = globPattern(p.filename().string(), p.parent_path().string());
		for (auto& fil : files)
			remove_all_comments(fil, ext_to_line_comment_symbols, ext_to_block_comment_symbols);
	}
	return EXIT_SUCCESS;
}

void remove_line_comments(std::vector<std::string>& file, const std::string& comment_symbol)
{
	std::string line;
	for (std::string& line : file)
	{
		size_t offset;

		if ((offset = line.find(comment_symbol)) != line.npos)
		{
			line = line.substr(0, offset);
		}
	}
}

void remove_block_comments(std::vector<std::string>& file,
	const std::string& comment_symbol_start,
	const std::string& comment_symbol_end)
{
	std::string line;
	bool multiline_line_break = false;
	for (std::string& line : file)
	{
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
				line.clear();
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
				line = line.substr(0, offset);
				break;
			}
			offset = line.find(comment_symbol_start);
			offset2 = line.find(comment_symbol_end);
		}
		if (multiline_line_break)
		{
			continue;
		}
	}
}

void remove_all_comments(const std::string& filename, const line_comments_map& line_map, const block_comments_map& block_map)
{
	size_t ext_off = filename.find_last_of('.') + 1;
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

	std::string extension = filename.substr(ext_off);

	std::vector<std::string> file_read;
	while (!in.eof())
	{
		std::string t;
		std::getline(in, t);
		file_read.push_back(t);
	}
	in.close();

	for (const auto& pair : block_map.at(extension))
	{
		remove_block_comments(file_read, pair.first, pair.second);
	}

	for (const auto& line_com : line_map.at(extension))
	{
		remove_line_comments(file_read, line_com);
	}

	for (auto& str : file_read)
	{
		if (!str.empty())
			std::cout << str << std::endl;
	}
}

void fill_comments_maps(line_comments_map& line_map, block_comments_map& block_map,
	const std::string& line_file, const std::string& block_file)
{
	std::ifstream line_in(line_file);
	if (!line_in.good())
	{
		std::cerr << "Failed to open line_comments_file [" << line_file << "] Quitting!\n";
		exit(EXIT_FAILURE);
	}

	std::ifstream block_in(block_file);
	if (!block_in.good())
	{
		std::cerr << "Failed to open line_comments_file [" << block_file << "] Quitting!\n";
		exit(EXIT_FAILURE);
	}

	while (!line_in.eof())
	{
		std::string line;
		std::getline(line_in, line);
		if (line.empty())
			continue;
		std::string extension = line.substr(0, line.find(':'));
		line_map[extension].push_back(line.substr(line.find(':') + 1));
	}
	line_in.close();

	while (!block_in.eof())
	{
		std::string line;
		std::getline(block_in, line);
		if (line.empty())
			continue;
		std::string extension = line.substr(0, line.find(':'));
		std::string beg = line.substr(line.find(':') + 1, line.find("QQQ") - line.find(':') - 1);
		std::string end = line.substr(line.find("QQQ") + 3);
		block_map[extension].push_back({ beg, end });
	}
	block_in.close();
}

std::vector<std::string> globPattern(const std::string& pattern, const std::string& directory) {
	std::vector<std::string> files;
	std::regex regexPattern(pattern);

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.is_regular_file()) {
			std::string filename = entry.path().filename().string();
			if (std::regex_match(filename, regexPattern)) {
				files.emplace_back(entry.path().string());
			}
		}
	}

	return files;
}