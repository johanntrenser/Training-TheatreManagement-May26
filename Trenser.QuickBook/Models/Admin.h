/*
 * File: Admin.h
 * Description: Defines the Admin class, which represents an administrative
 *              user in the system. Inherits from User and can be extended
 *              with system management functions such as managing theatres,
 *              movies, shows, and users.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include "User.h"

class Admin : public User
{
public:
    Admin() = default;
    Admin(const std::string& userId,
        const std::string& userName,
        const std::string& email,
        const std::string& password,
        const std::string& phoneNumber,
        Enums::UserType type);
};
