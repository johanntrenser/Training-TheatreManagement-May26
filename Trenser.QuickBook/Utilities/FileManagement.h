#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
class FileManagement
{
public:
	static std::vector<std::string> readlines(const std::string& path);
	static void writeLines(const std::string& path, const std::vector<std::string>& line);
};