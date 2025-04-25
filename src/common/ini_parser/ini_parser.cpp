#include "ini_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

IniParser::IniData IniParser::parse(const std::string &file_path)
{
	ini_data_.clear();
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open INI file: " + file_path);
	}

	std::string line;
	std::string current_section;
	while (std::getline(file, line))
	{
		// Trim whitespace
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		// Skip comments and empty lines
		if (line.empty() || line[0] == ';' || line[0] == '#')
		{
			continue;
		}

		// Section header
		if (line[0] == '[' && line.back() == ']')
		{
			current_section = line.substr(1, line.size() - 2);
			ini_data_[current_section] = Section();
		}
		// Key-value pair
		else
		{
			auto delimiter_pos = line.find('=');
			if (delimiter_pos == std::string::npos)
			{
				throw std::runtime_error("Invalid INI format: " + line);
			}

			std::string key = line.substr(0, delimiter_pos);
			std::string value = line.substr(delimiter_pos + 1);

			// Trim whitespace
			key.erase(0, key.find_first_not_of(" \t"));
			key.erase(key.find_last_not_of(" \t") + 1);
			value.erase(0, value.find_first_not_of(" \t"));
			value.erase(value.find_last_not_of(" \t") + 1);

			ini_data_[current_section][key] = value;
		}
	}

	return ini_data_;
}

IniParser::Section IniParser::getSection(const std::string &section_name) const
{
	auto it = ini_data_.find(section_name);
	if (it == ini_data_.end())
	{
		throw std::runtime_error("Section not found: " + section_name);
	}
	return it->second;
}