/*
 * File: Admin.cpp
 * Description: Implements the Admin class, which represents an administrative 
 *              user in the system. Admins can manage theatres, movies, shows, 
 *              and users, ensuring smooth operation of the booking platform.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#include "Admin.h"

/*
 * Function: Admin::Admin
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
Admin::Admin(const std::string& userId,
    const std::string& userName,
    const std::string& email,
    const std::string& password,
    const std::string& phoneNumber,
    Enums::UserType userType)
    : User(userId, userName, email, password, phoneNumber, userType)
{
}