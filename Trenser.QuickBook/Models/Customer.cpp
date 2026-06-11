/*
 * File: Customer.cpp
 * Description: Implements the Customer class, which represents a customer
 *              in the system. Customers can browse movies, book tickets,
 *              make payments, and manage their reservations.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#include "Customer.h"

 /*
  * Function: Customer::Customer
  * Description: Parameterized constructor. Initializes user attributes with
  *              provided values.
  * Parameters:
  *    const std::string& userId - Unique identifier for the user
  *    const std::string& userName - Name of the user
  *    const std::string& email - Email address of the user
  *    const std::string& password - Password for authentication
  *    long int phoneNumber - Phone number of the user
  *    enum userType - Type of user (role)
  *    enum status - Current status of the user
  * Returns: None
  */
Customer::Customer(const std::string& userId,
    const std::string& userName,
    const std::string& email,
    const std::string& password,
    const std::string& phoneNumber,
    Enums::UserType userType)
    : User(userId, userName, email, password, phoneNumber, userType)
{
}