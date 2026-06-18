#include <sstream>
#include <iomanip>
#include "AuthenticationManagementService.h"
#include "Factory.h"
#include "DataStore.h"
#include "Admin.h"
#include "TheatreOwner.h"
#include "Customer.h"
#include "Log.h"

/*
 * Function: AuthenticationManagementService
 * Description: Default constructor that initializes the authentication service
 *              with a reference to the shared DataStore instance and named mutex.
 * Parameters: None
 * Returns: None
 */
AuthenticationManagementService::AuthenticationManagementService() :
    m_dataStore(DataStore::getInstance()), m_mutex(config::MutexMappings::USER_MUTEX_NAME)
{
}

/*
 * Function: AuthenticationManagementService::login
 * Description: To authenticate a user based on provided email and password.
 * Parameters:
 *    email - The email address of the user attempting to log in
 *    password - The password associated with the given email
 * Returns:
 *    A pair containing:
 *     enum - LoginStatus indicating the result of the login attempt
 *     enum - UserType of the authenticated user if login is successful,
 *        otherwise USER_NOT_FOUND
 */
std::pair<Enums::LoginStatus, Enums::UserType> AuthenticationManagementService::login(const std::string& email, const std::string& password)
{
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if (iterator->second->getEmail() == email)
        {
            if (iterator->second->getPassword() == password)
            {
                if (iterator->second->getStatus() == Enums::UserStatus::ACTIVE)
                {
                    if (m_dataStore.isUserLoggedIn(iterator->second->getUserId()))
                    {
                        return std::make_pair(Enums::LoginStatus::USER_ALREADY_LOGGED_IN, Enums::UserType::USER_NOT_FOUND);
                    }
                    m_event.init(iterator->second->getUserId());
                    m_event.startListener(Enums::getUserTypeString(iterator->second->getUserType()), iterator->second->getUserId(), iterator->second->getUserName());
                    User* user = Factory::getObject<User>(
                        iterator->second->getUserId(),
                        iterator->second->getUserName(),
                        iterator->second->getEmail(),
                        iterator->second->getPassword(),
                        iterator->second->getPhoneNumber(),
                        iterator->second->getUserType());
                    if (user != nullptr)
                    {
                        user->setStatus(iterator->second->getStatus());
                        m_dataStore.addLoggedInUser(user->getUserId());
                        m_dataStore.setAuthenticatedUser(user);
                        std::string message = "User with ID : " + user->getUserId() + " has logged in.";
                        logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
                        return std::make_pair(Enums::LoginStatus::USER_FOUND, user->getUserType());
                    }
                }
                else
                {
                    std::string message = "User with ID : " + iterator->second->getUserId() + " could not log in.";
                    logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
                    return std::make_pair(Enums::LoginStatus::USER_NOT_FOUND, Enums::UserType::USER_NOT_FOUND);
                }
            }
            return std::make_pair(Enums::LoginStatus::INVALID_PASSWORD, Enums::UserType::USER_NOT_FOUND);
        }
    }
    return std::make_pair(Enums::LoginStatus::USER_NOT_FOUND, Enums::UserType::USER_NOT_FOUND);
}

/*
 * Function: logout
 * Description: Logs out the currently authenticated user, clearing session data.
 * Parameters: None
 * Returns: None
 */
void AuthenticationManagementService::logout()
{
    const User* authenticatedUser = m_dataStore.getAuthenticatedUser();
    std::string message = "User with ID : " + authenticatedUser->getUserId() + " has logged out.";
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    m_dataStore.removeLoggedInUser(authenticatedUser->getUserId());
    m_dataStore.setAuthenticatedUser(nullptr);
}

/*
 * Function: AuthenticationManagementService::registerUser
 * Description: Registers a new user by creating a User object through the Factory
 *              and adding it to the DataStore.
 * Parameters:
 *    userName (const std::string&) - The name of the user
 *    email (const std::string&) - The email address of the user
 *    password (const std::string&) - The password for the user
 *    phoneNumber (const std::string&) - The phone number of the user
 *    userType (Enums::UserType) - The type of user (Customer, Theatre Owner, Admin)
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the user is registered, FAILED otherwise
 */
Enums::ProcessStatus AuthenticationManagementService::registerUser(const std::string& userName, const std::string& email, const std::string& password, const std::string phoneNumber, Enums::UserType userType)
{
    ScopedLock lock(m_mutex);
    if (!isPhoneNumberUnique(phoneNumber))
    {
        return Enums::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS;
    }
    if (!isEmailIdUnique(email))
    {
        return Enums::ProcessStatus::EMAIL_ALREADY_EXISTS;
    }
    User* user = nullptr;
    if (userType == Enums::UserType::ADMIN)
    {
        user = Factory::getObject<Admin>(generateUserId(), userName, email, password, phoneNumber, userType);
        std::string message = "New User Type - Admin with ID : " + user->getUserId() + " has registered.";
        logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    }
    else if (userType == Enums::UserType::THEATRE_OWNER)
    {
        user = Factory::getObject<TheatreOwner>(generateUserId(), userName, email, password, phoneNumber, userType);
        std::string message = "New User Type - Theatre Owner with ID : " + user->getUserId() + " has registered.";
        logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    }
    else
    {
        user = Factory::getObject<Customer>(generateUserId(), userName, email, password, phoneNumber, userType);
        std::string message = "New User Type - Customer with ID : " + user->getUserId() + " has registered.";
        logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    }
    if (user != nullptr)
    {
        m_dataStore.addUser(user);
        return Enums::ProcessStatus::SUCCESS;
    }
    std::string message = "New user registration failed.";
    logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: AuthenticationManagementService::generateUserId
 * Description: Generates a unique user ID based on the current number of users
 *              in the DataStore. IDs are formatted as "US" followed by a
 *              zero-padded number (e.g., US001, US002).
 * Parameters: None
 * Returns:
 *    std::string - The generated unique user ID
 */
const std::string AuthenticationManagementService::generateUserId()
{
    const int usersCount = m_dataStore.getUsersCount();
    int idNumber = usersCount + 1;
    std::ostringstream buffer;
    buffer << "US" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: DataStore::isPhoneNumberUnique
 * Description: Checks whether the given phone number is unique among all users.
 * Parameters:
 *    phoneNumber - The phone number string to validate.
 * Returns:
 *    true if the phone number does not exist in the DataStore, false otherwise.
 */
bool AuthenticationManagementService::isPhoneNumberUnique(const std::string& phoneNumber)
{
    ScopedLock lock(m_mutex);
    const std::map<std::string, User*> users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if ((iterator->second)->getPhoneNumber() == phoneNumber)
        {
            return false;
        }
    }
    return true;
}


/*
 * Function: DataStore::isEmailIdUnique
 * Description: Checks whether the given email address is unique among all users.
 * Parameters:
 *    email - The email string to validate.
 * Returns:
 *    true if the email does not exist in the DataStore, false otherwise.
 */
bool AuthenticationManagementService::isEmailIdUnique(const std::string& email)
{
    ScopedLock lock(m_mutex);
    const std::map<std::string, User*> users = m_dataStore.getUsers();
    if (!users.empty())
    {
        for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
        {
            if ((iterator->second)->getEmail() == email)
            {
                return false;
            }
        }
    }
    return true;
}
