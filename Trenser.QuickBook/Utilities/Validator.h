/*
 * File: util_validation.h
 * Description: Declares utility functions for validating user input data.
 *              Provides methods to check the validity of phone numbers
 *              and email addresses using basic string operations.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include "User.h"
#include "DataStore.h"

namespace util
{
	void isPhoneNumberValid(std::string&);
	void isEmailValid(std::string&);
	void isPasswordValid(std::string&);
	bool validateCard(const std::string& cardNumber, const std::string& expiry, const std::string& cvv);
	inline bool validateUPI(const std::string& upiId);
}