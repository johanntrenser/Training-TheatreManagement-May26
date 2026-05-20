/*
 * File: Validator.cpp
 * Description: Implements validation helper functions for user input fields
 *              including password, email, and phone number. Ensures that
 *              user provided data meets defined format.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <iostream>
#include <regex>
#include "Validator.h"
#include "InputHelper.h"

 /*
  * Function: isPasswordValid
  * Description: Validates and enforces password rules. A valid password must:
  *                - Be at least 8 characters long
  *                - Contain at least one uppercase letter
  *                - Contain at least one lowercase letter
  *                - Contain at least one digit
  *                - Contain at least one special character
  * Parameters:
  *    value - Reference to the password string entered by the user
  * Returns:
  *    None
  */
void util::isPasswordValid(std::string& value) 
{
    bool isPasswordValid = false;
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char ch : value) {
        if (std::isupper(static_cast<unsigned char>(ch))) {
            hasUpper = true;
        }
        else if (std::islower(static_cast<unsigned char>(ch))) {
            hasLower = true;
        }
        else if (std::isdigit(static_cast<unsigned char>(ch))) {
            hasDigit = true;
        }
        else {
            hasSpecial = true;
        }
    }
    if (value.size() >= 8 && hasUpper && hasLower && hasDigit && hasSpecial) {
        std::cout << "Password set!\n";
        return;
    }
    while (!isPasswordValid)
    {
        std::cout << "Invalid password: must be at least 8 characters, contain upper, lower, digit, and special character.\n";
        util::readValue(value);
        hasUpper = hasLower = hasDigit = hasSpecial = false;
        for (char ch : value) {
            if (std::isupper(static_cast<unsigned char>(ch))) {
                hasUpper = true;
            }
            else if (std::islower(static_cast<unsigned char>(ch))) {
                hasLower = true;
            }
            else if (std::isdigit(static_cast<unsigned char>(ch))) {
                hasDigit = true;
            }
            else {
                hasSpecial = true;
            }
        }
        if (value.size() >= 8 && hasUpper && hasLower && hasDigit && hasSpecial) {
            std::cout << "Password set!\n";
            isPasswordValid = true;
        }
    }
}

/*
 * Function: isEmailValid
 * Description: Validates an email address against a regex pattern.
 * Parameters:
 *    value - Reference to the email string entered by the user
 * Returns:
 *    None
 */
void util::isEmailValid(std::string& value)
{
    bool isValidEmail = false;
    std::regex emailPattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    if (std::regex_match(value, emailPattern)) {
        std::cout << "Email address accepted.\n";
        return;
    }
    while (!isValidEmail) {
        std::cout << "Invalid email address. Please enter a valid format such as example@domain.com.\n";
        util::readValue(value);
        if (std::regex_match(value, emailPattern)) {
            std::cout << "Email address accepted.\n";
            isValidEmail = true;
        }
    }
}

/*
 * Function: isPhoneNumberValid
 * Description: Validates a phone number against a regex pattern.
 *              A valid phone number must contain exactly 10 digits.
 * Parameters:
 *    value - Reference to the phone number string entered by the user
 * Returns:
 *    None
 */
void util::isPhoneNumberValid(std::string& value)
{
    bool isValidPhoneNumber = false;
    std::regex phoneNumberPattern(R"(^[0-9]{10}$)");
    if (std::regex_match(value, phoneNumberPattern)) {
        std::cout << "Phone Number accepted.\n";
        return;
    }
    while (!isValidPhoneNumber)
    {
        std::cout << "Invalid phone number. Phone number should be of 10 digits.\n";
        util::readValue(value);
        if (std::regex_match(value, phoneNumberPattern)) {
            std::cout << "Phone Number accepted.\n";
            isValidPhoneNumber = true;
        }
    }
}