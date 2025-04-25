#include "ini_parser/ini_parser.h"
#include <iostream>

int main()
{
	IniParser parser;

	try
	{
		auto ini_data = parser.parse("config.ini");

		// 특정 섹션 출력
		auto section = parser.getSection("Settings");
		std::cout << "[Settings]" << std::endl;
		for (const auto &[key, value] : section)
		{
			std::cout << key << " = " << value << std::endl;
		}

		// Network 섹션 출력
		auto network_section = parser.getSection("Network");
		std::cout << "[Network]" << std::endl;
		for (const auto &[key, value] : network_section)
		{
			std::cout << key << " = " << value << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}