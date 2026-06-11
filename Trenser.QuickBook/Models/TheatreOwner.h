/*
 * File: TheatreOwner.h
 * Description: Defines the TheatreOwner class, which represents a theatre
 *              owner in the system. Inherits from User and can be extended
 *              with theatre management functions such as adding theatres,
 *              managing screens, scheduling shows, and handling movies.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include "User.h"

class TheatreOwner : public User
{
public:
    TheatreOwner() = default;
    TheatreOwner(const std::string& userId,
        const std::string& userName,
        const std::string& email,
        const std::string& password,
        const std::string& phoneNumber,
        Enums::UserType type);
};