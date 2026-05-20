#include "Validator.h"

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