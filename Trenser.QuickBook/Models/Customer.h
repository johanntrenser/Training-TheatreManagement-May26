/*
 * File: Customer.h
 * Description: Defines the Customer class, which represents a customer
 *              in the system. Inherits from User and can be extended
 *              with booking and payment functions such as ticket
 *              reservations, cancellations, and viewing bookings.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#pragma once
#include "User.h"

class Customer : public User
{
public:
    Customer() = default;
    Customer(const std::string& userId,
        const std::string& userName,
        const std::string& email,
        const std::string& password,
        const std::string& phoneNumber,
        Enums::UserType type);
};

