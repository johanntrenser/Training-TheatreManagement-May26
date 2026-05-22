/*
 * File: UserManagementService.h
 * Description: Declares the UserManagementService class, which provides
 *              functionality for managing users within the system.
 *              This service handles operations such as creating, updating,
 *              deactivating/reactivating users, viewing user profiles,
 *              changing passwords, and retrieving user lists.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "DataStore.h"

class UserManagementService
{
private:
    DataStore& m_dataStore;
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
    const std::string generateUserId();
    Enums::ProcessStatus createUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType);
    const std::vector<const User*>& getActiveUsers() const;

};

