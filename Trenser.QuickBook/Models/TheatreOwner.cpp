/*
 * File: TheatreOwner.cpp
 * Description: Implements the TheatreOwner class, which represents a theatre
 *              owner in the system. Theatre owners can manage theatres,
 *              screens, movies, and shows, ensuring smooth operation of
 *              their cinema business.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#include "TheatreOwner.h"

 /*
  * Function: TheatreOwner::TheatreOwner
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
TheatreOwner::TheatreOwner(const std::string& userId,
    const std::string& userName,
    const std::string& email,
    const std::string& password,
    const std::string& phoneNumber,
    Enums::UserType userType)
    : User(userId, userName, email, password, phoneNumber, userType)
{
}