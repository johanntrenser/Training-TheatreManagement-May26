#include "OutputHelper.h"

void util::clear()
{
	std::cout << "\x1B[2J\x1B[H" << std::flush;
}