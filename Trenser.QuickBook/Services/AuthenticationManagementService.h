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


