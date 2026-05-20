#include "InputHelper.h"

void util::readString(std::string& value)
{
	getline(std::cin >> std::ws, value);
}

void util::pressEnter()
{
	system("pause");
}