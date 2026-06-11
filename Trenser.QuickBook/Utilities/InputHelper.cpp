#include "InputHelper.h"

/*
     * Function: pressEnterToContinue
     * Description: Waits for the user to press the Enter key before continuing.
     *              Useful for pausing console applications until user input.
     * Parameters: None
     * Returns: None
     */
void util::pressEnterToContinue()
{
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();
}