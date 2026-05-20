#include "Validator.h"

/*
	 * Function: isPhoneNumberValid
	 * Description: Validates whether the given string represents a valid phone number.
	 * Parameters:
	 *   - phone: String containing the phone number to validate.
	 * Returns: True if the phone number is valid, false otherwise.
	 */
bool util::isPhoneNumberValid(const std::string& phoneNumber) {
	if (phoneNumber.size() != 10)
	{
		return false;
	}
	return std::all_of(phoneNumber.begin(), phoneNumber.end(),
		[](char character)
		{
			return std::isdigit(character);
		}
	);
}

/*
	 * Function: isEmailValid
	 * Description: Validates whether the given string represents a valid email address.
	 * Parameters:
	 *   - email: String containing the email address to validate.
	 * Returns: True if the email address is valid, false otherwise.
	 */
bool util::isEmailValid(const std::string& email) {
	size_t index = email.find('@');
	if (index == std::string::npos) return false;
	if (email.find('@', index + 1) != std::string::npos)
	{
		return false;
	}
	if (index == 0 || index == email.size() - 1)
	{
		return false;
	}
	return true;
}