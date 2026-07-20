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
#include "LogManagementService.h"
#include "NamedMutex.h"
#include "ScopedLock.h"
#include "NotificationEvent.h"

class UserManagementService
{
private:
    DataStore& m_dataStore;
    LogManagementService logManagementService;
    const std::string& PATH = config::File::USER_FILEPATH;
    NamedMutex m_mutex;
    NotificationEvent m_event;
public:
    UserManagementService();
    const std::string generateUserId();
    Enums::ProcessStatus createUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType);
    const std::vector<const User*> getActiveUsers();
    const std::vector<const User*> getInactiveUsers();
    Enums::ProcessStatus setAuthenticatedUserEmail(const std::string& email);
    Enums::ProcessStatus setAuthenticatedUserPhoneNumber(const std::string& phoneNumber);
    Enums::ProcessStatus setAuthenticatedUserUserName(const std::string& userName);
    Enums::ProcessStatus deactivateUser(const std::string& userId);
    Enums::ProcessStatus reactivateUser(const std::string& userId);
    Enums::ProcessStatus changePassword(const std::string& currentPassword, const std::string& newPassword);
    const User* const getAuthenticatedUser();
    bool isAdminPresent();
    void createDefaultAdmin();
};