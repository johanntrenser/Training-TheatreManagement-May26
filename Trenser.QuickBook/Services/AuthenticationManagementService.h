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

class AuthenticationManagementService
{
private:
    DataStore& m_dataStore;
public:
    AuthenticationManagementService();
    void login(const std::string& email, const std::string& password);
    void logout();
    void registerUser();
    ~AuthenticationManagementService() = default;
};


