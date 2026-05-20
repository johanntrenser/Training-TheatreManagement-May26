#include "UserManagementService.h"

UserManagementService::UserManagementService()
{
}

bool UserManagementService::createUser(const std::string& name,
    const std::string& username,
    const std::string& email,
    long int phone,
    const std::string& password)
{
    return true;
}

bool UserManagementService::deactivateUser(const std::string& userId)
{
    return true;
}

bool UserManagementService::reactivateUser(const std::string& userId)
{
    return true;
}

std::vector<User*> UserManagementService::viewAllUsers() const
{
    return {};
}

bool UserManagementService::updateUserDetails(const std::string& userId,
    const std::string& name,
    const std::string& email,
    long int phone)
{
    return true;
}

User* UserManagementService::viewProfile(const std::string& userId) const
{
    return nullptr;
}

int UserManagementService::viewUserStatus(const std::string& userId) const
{
    return 0; // placeholder enum value
}

void UserManagementService::changePassword(const std::string& userId,
    const std::string& newPassword)
{
}
