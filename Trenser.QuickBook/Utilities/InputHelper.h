#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

namespace util
{
	template <typename T>
	void readValue(T& value)
	{
		std::cin >> value;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			throw std::runtime_error("Invalid Console Input");
		}
	}

	void readString(std::string&);
	void pressEnter();
}