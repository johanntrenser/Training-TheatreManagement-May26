/*
 * File: AuthenticationManagementService.h
 * Description: Declares the AuthenticationManagementService class, which provides
 *              functionality for user authentication including login, logout,
 *              and registration. The class interacts with the shared DataStore
 *              instance to manage authentication data.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include "DataStore.h"
#include "LogManagementService.h"
#include "NamedMutex.h"
#include "ScopedLock.h"
#include "NotificationEvent.h"

class AuthenticationManagementService
{
private:
    DataStore& m_dataStore;
    LogManagementService logManagementService;
    NamedMutex m_mutex;
    NotificationEvent m_event;
public:
    AuthenticationManagementService();
    std::pair<Enums::LoginStatus, Enums::UserType> login(const std::string& email, const std::string& password);
    void logout();
    Enums::ProcessStatus registerUser(const std::string& userName, const std::string& email, const std::string& password, const std::string phoneNumber, Enums::UserType userType);
    const std::string generateUserId();
    bool isPhoneNumberUnique(const std::string&);
    bool isEmailIdUnique(const std::string&);
    bool checkAndHandleForcedLogout();
    ~AuthenticationManagementService() = default;
};


