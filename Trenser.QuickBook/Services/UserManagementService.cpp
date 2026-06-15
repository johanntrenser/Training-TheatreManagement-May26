#include <map>
#include <iomanip>
#include <sstream>
#include "UserManagementService.h"
#include "Admin.h"
#include "TheatreOwner.h"
#include "Customer.h"
#include "Factory.h"
#include "AuthenticationManagementService.h"

/*
     * Function: UserManagementService
     * Description: Default constructor that initializes the user management
     *              service. Prepares the service for handling user-related
     *              operations.
     * Parameters: None
     * Returns: None
     */
UserManagementService::UserManagementService()
    : m_dataStore(DataStore::getInstance()), m_mutex(config::MutexMappings::USER_MUTEX_NAME)
{
}

/*
 * Function: generateUserId
 * Description: Generates a unique user ID based on the current number of users
 *              in the DataStore. IDs are formatted as "US" followed by a
 *              zero-padded number (e.g., US001, US002).
 * Parameters: None
 * Returns:
 *    std::string - The generated unique user ID
 */
const std::string UserManagementService::generateUserId()
{
    const int usersCount = m_dataStore.getUsersCount();
    int idNumber = usersCount + 1;
    std::ostringstream buffer;
    buffer << "US" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: createUser
 * Description: Passes the Admin driven user registration process to the
 *              UserManagementService.
 * Parameters:
 *    userName   - The name of the user to be registered
 *    email      - The email address of the user
 *    password   - The password for the user account
 *    phoneNumber- The phone number of the user
 *    userType(enum)   - The role of the user (Customer, Theatre Owner, or Admin)
 * Returns:
 *    ProcessStatus::SUCCESS if registration is successful,
 *    ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus UserManagementService::createUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType)
{
    ScopedLock lock(m_mutex);
    AuthenticationManagementService authenticationService;
    if (!authenticationService.isPhoneNumberUnique(phoneNumber))
    {
        return Enums::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS;
    }
    if (!authenticationService.isEmailIdUnique(email))
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
    * Function: getActiveUsers
    * Description: Retrieves a list of all active users in the system.
    * Parameters: None
    * Returns: Vector of User pointers representing all active users.
    */
const std::vector<const User*> UserManagementService::getActiveUsers()
{
    ScopedLock lock(m_mutex);
    std::vector<const User*> constUsers;
    if (m_dataStore.getAuthenticatedUserType() == Enums::UserType::ADMIN)
    {
        const std::map<std::string, User*>& users = m_dataStore.getUsers();
        for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
        {
            if (iterator->second->getStatus() == Enums::UserStatus::ACTIVE)
            {
                constUsers.push_back(iterator->second);
            }
        }
        return constUsers;
    }
    return constUsers;
}

/*
    * Function: getInactiveUsers
    * Description: Retrieves a list of all inactive users in the system.
    * Parameters: None
    * Returns: Vector of User pointers representing all inactiveUsers users.
    */
const std::vector<const User*> UserManagementService::getInactiveUsers()
{
    ScopedLock lock(m_mutex);
    std::vector<const User*> constUsers;
    if (m_dataStore.getAuthenticatedUserType() == Enums::UserType::ADMIN)
    {
        const std::map<std::string, User*> users = m_dataStore.getUsers();
        for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
        {
            if (iterator->second->getStatus() == Enums::UserStatus::INACTIVE)
            {
                constUsers.push_back(iterator->second);
            }
        }
        return constUsers;
    }
    return constUsers;
}

/*
 * Function: setAuthenticatedUserUserName
 * Description: Updates the username of the currently authenticated user in the data store.
 * Parameters:
 *    username - The new username to be set
 * Returns:
 *    enum - ProcessStatus if the update succeeds,
 *    enum - ProcessStatus otherwise
 */
Enums::ProcessStatus UserManagementService::setAuthenticatedUserUserName(const std::string& username)
{
    ScopedLock lock(m_mutex);
    m_dataStore.setAuthenticatedUserName(username);
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: setAuthenticatedUserEmail
 * Description: Updates the email of the currently authenticated user in the data store.
 * Parameters:
 *    email - The new email to be set
 * Returns:
 *    enum - ProcessStatus if the update succeeds,
 *    enum - ProcessStatus otherwise
 */
Enums::ProcessStatus UserManagementService::setAuthenticatedUserEmail(const std::string& email)
{
    ScopedLock lock(m_mutex);
    m_dataStore.setAuthenticatedUserEmail(email);
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: setAuthenticatedUserPhoneNumber
 * Description: Updates the phone number of the currently authenticated user in the data store.
 * Parameters:
 *    phoneNumber - The new phone number to be set
 * Returns:
 *    enum - ProcessStatus if the update succeeds,
 *    enum - ProcessStatus otherwise
 */
Enums::ProcessStatus UserManagementService::setAuthenticatedUserPhoneNumber(const std::string& phoneNumber)
{
    ScopedLock lock(m_mutex);
    m_dataStore.setAuthenticatedUserPhoneNumber(phoneNumber);
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: deactivateUser
 * Description: Iterates through the user records in the data store to locate
 *              the user with the specified ID and returns the user status (ACTIVE/INACTIVE).
 * Parameters:
 *    userId - The unique identifier of the user to be deactivated
 * Returns:
 *    enum - SUCCESS if the user was found and deactivated,
 *    enum - FAILED if the user was not found or already inactive
 */
Enums::ProcessStatus UserManagementService::deactivateUser(const std::string& userId)
{
    ScopedLock lock(m_mutex);
    std::map<std::string, User*> users = m_dataStore.getUsers();
    User* currentUser = m_dataStore.getAuthenticatedUser();
    for (std::map<std::string, User*>::iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if (iterator->second->getUserId() == userId && userId != currentUser->getUserId())
        {
            if (iterator->second->getStatus() == Enums::UserStatus::ACTIVE)
            {
                iterator->second->setStatus(Enums::UserStatus::INACTIVE);
                std::string message = "User with ID : " + iterator->second->getUserId() + " deactivated.";
                Enums::ProcessStatus status =  m_dataStore.updateUserStatus(iterator->second->getUserId(), iterator->second->getStatus());
                if (status == Enums::ProcessStatus::FAILED)
                {
                    return Enums::ProcessStatus::FAILED;
                }
                logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
                return Enums::ProcessStatus::SUCCESS;
            }
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
     * Function: reactivateUser
     * Description: Reactivates a previously deactivated user.
     * Parameters:
     *   - userId: Unique identifier of the user.
     * Returns: enum SUCCESS if the user is successfully reactivated, FAILURE otherwise.
     */
Enums::ProcessStatus UserManagementService::reactivateUser(const std::string& userId)
{
    ScopedLock lock(m_mutex);
    std::map<std::string, User*> users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if (iterator->second->getUserId() == userId)
        {
            if (iterator->second->getStatus() == Enums::UserStatus::INACTIVE)
            {
                iterator->second->setStatus(Enums::UserStatus::ACTIVE);
                std::string message = "User with ID : " + iterator->second->getUserId() + " reeactivated.";
                Enums::ProcessStatus status = m_dataStore.updateUserStatus(iterator->second->getUserId(), iterator->second->getStatus());
                if (status == Enums::ProcessStatus::FAILED)
                {
                    return Enums::ProcessStatus::FAILED;
                }
                logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
                return Enums::ProcessStatus::SUCCESS;
            }
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: getAuthenticatedUser
 * Description: Retrieves the currently authenticated user from the data store.
 * Parameters:
 *    None
 * Returns:
 *    A pointer to the authenticated User object if one exists,
 */
const User* const UserManagementService::getAuthenticatedUser()
{
    return m_dataStore.getAuthenticatedUser();
}

/*
 * Function: changePassword
 * Description: Validates the current password of the authenticated user and,
 *              if it matches, updates the password to the new value.
 * Parameters:
 *    currentPassword (string) - The current password entered by the user
 *    newPassword (string)    - The new password to be set
 * Returns:
 *    enum - SUCCESS if the password change succeeds,
 *    enum - FAILED if the current password does not match
 */
Enums::ProcessStatus UserManagementService::changePassword(const std::string& currentPassword, const std::string& newPassword)
{
    ScopedLock lock(m_mutex);
    User* authenticatedUser = m_dataStore.getAuthenticatedUser();
    std::string authenticatedUserPassword = authenticatedUser->getPassword();
    if (currentPassword == authenticatedUserPassword && newPassword != authenticatedUserPassword)
    {
        m_dataStore.setAuthenticatedUserPassword(newPassword);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: UserManagementService::isAdminPresent
 * Description: Checks if any admin user exists in the system.
 * Parameters:
 *    None
 * Returns:
 *    true if an admin is present, false otherwise
 */
bool UserManagementService::isAdminPresent()
{
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getUserType() == Enums::UserType::ADMIN)
        {
            return true;
        }
    }
    return false;
}

/*
 * Function: UserManagementService::createDefaultAdmin
 * Description: Creates a default admin user with preset credentials and adds it to DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserManagementService::createDefaultAdmin()
{
    ScopedLock lock(m_mutex);
    if (!isAdminPresent())
    {
        User* user = new User(generateUserId(), "admin", "admin@gmail.com", "Admin@123", "9999999999", Enums::UserType::ADMIN);
        m_dataStore.addUser(user);
    }
}