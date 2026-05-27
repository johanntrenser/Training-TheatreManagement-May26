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