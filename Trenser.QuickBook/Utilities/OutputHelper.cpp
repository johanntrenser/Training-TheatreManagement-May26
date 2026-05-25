#include "OutputHelper.h"

/*
     * Function: clear
     * Description: Clears the console screen. Useful for refreshing the display
     *              during interactive console applications.
     * Parameters: None
     * Returns: None
     */
void util::clear()
{
    std::cout << "\x1B[2J\x1B[H" << std::flush;
}