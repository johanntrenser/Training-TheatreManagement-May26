#pragma once
#include <string>
#include <vector>
#include "User.h"

class UserManagementService
{
public:
    UserManagementService();
    bool createUser(const std::string& name,
        const std::string& username,
        const std::string& email,
        long int phone,
        const std::string& password);

    bool deactivateUser(const std::string& userId);
    bool reactivateUser(const std::string& userId);

    std::vector<User*> viewAllUsers() const;

    bool updateUserDetails(const std::string& userId,
        const std::string& name,
        const std::string& email,
        long int phone);

    User* viewProfile(const std::string& userId) const;
    int viewUserStatus(const std::string& userId) const; // enum placeholder

    void changePassword(const std::string& userId,
        const std::string& newPassword);
};

