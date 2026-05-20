/*
 * File: util_validation.h
 * Description: Declares utility functions for validating user input data.
 *              Provides methods to check the validity of phone numbers
 *              and email addresses using basic string operations.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include<string>
#include<algorithm>
#include<cctype>

namespace util
{
	bool isPhoneNumberValid(const std::string&);
	bool isEmailValid(const std::string&);
}