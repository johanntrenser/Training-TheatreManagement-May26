#include "InputHelper.h"

/*
     * Function: readString
     * Description: Reads a string value from the console input stream.
     * Parameters:
     *   - str: Reference to the string variable where the input will be stored.
     * Returns: None
     */
void util::readString(std::string& value)
{
    getline(std::cin >> std::ws, value);
}

/*
     * Function: pressEnter
     * Description: Waits for the user to press the Enter key before continuing.
     *              Useful for pausing console applications until user input.
     * Parameters: None
     * Returns: None
     */
void util::pressEnter()
{
    system("pause");
}