/*
 * File: util.h
 * Description: Declares utility functions for console input handling.
 *              Provides templated and specialized methods to safely
 *              read values from the console, handle invalid input,
 *              and manage user interaction prompts.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace util
{
	/*
	 * Function: readValue
	 * Description: Reads a value of type T from the console input stream.
	 *              Clears the input buffer and throws an exception if
	 *              invalid input is detected.
	 * Template Parameters:
	 *   - T: The type of value to be read (e.g., int, double, string).
	 * Parameters:
	 *   - value: Reference to the variable where the input will be stored.
	 * Returns: None
	 * Throws: std::runtime_error if the input is invalid.
	 */
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

	/*
	* Function Name : readValueWithRetry (template)
	* Description   : Reads a value of type T from user input with retry logic.
	*                 Keeps prompting until a valid value is entered.
	* Parameters    :
	*                  value  - Reference to the variable where input will be stored
	*                  prompt - The message displayed to the user before input
	* Return Type   : void
	*/
	template <typename T>
	void readValueWithRetry(T& value, const std::string& prompt)
	{
		while (true)
		{
			try
			{
				std::cout << prompt;
				readValue(value);
				break;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << " Please try again.\n";
			}
		}
	}

	/*
	* Function Name : readValueWithRetry (string overload)
	* Description   : Reads a string value from user input with retry logic.
	*                 Keeps prompting until a non-empty string is entered.
	* Parameters    :
	*                  value  - Reference to the string where input will be stored
	*                  prompt - The message displayed to the user before input
	* Return Type   : void
	*/
	inline void readValueWithRetry(std::string& value, const std::string& prompt)
	{
		while (true)
		{
			std::cout << prompt;
			readValue(value);
			if (!value.empty())
				break;
			std::cout << "Input cannot be empty. Please try again.\n";
		}
	}
	void readValue(std::string& value);
	void pressEnter();
}

namespace utils {

	/**
	 * @brief Converts a copy of a std::string to lowercase.
	 * @param str The input string (passed by value to create a copy).
	 * @return A new lowercase std::string.
	 */
	[[nodiscard]] inline std::string to_lower(std::string str) noexcept {
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return str;
	}

}