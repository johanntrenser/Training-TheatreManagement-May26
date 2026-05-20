#include "AuthenticationManagementService.h"

AuthenticationManagementService::AuthenticationManagementService() :
    m_dataStore(DataStore::getInstance())
{}

void AuthenticationManagementService::login(const std::string& email, const std::string& password)
{
}

void AuthenticationManagementService::logout()
{
}

void AuthenticationManagementService::registerUser()
{
}
