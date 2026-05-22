#include <map>
#include <iomanip>
#include <sstream>
#include "UserManagementService.h"
#include "Factory.h"

/*
     * Function: UserManagementService
     * Description: Default constructor that initializes the user management
     *              service. Prepares the service for handling user-related
     *              operations.
     * Parameters: None
     * Returns: None
     */
UserManagementService::UserManagementService()
    : m_dataStore(DataStore::getInstance())
{
}


/*
     * Function: viewAllUsers
     * Description: Retrieves a list of all users in the system.
     * Parameters: None
     * Returns: Vector of User pointers representing all users.
     */
std::vector<User*> UserManagementService::viewAllUsers() const
{
    return {};
}

/*
     * Function: updateUserDetails
     * Description: Updates the details of an existing user.
     * Parameters:
     *   - userId: Unique identifier of the user.
     *   - name: Updated full name of the user.
     *   - email: Updated email address.
     *   - phone: Updated contact phone number.
     * Returns: True if the details are successfully updated, false otherwise.
     */
bool UserManagementService::updateUserDetails(const std::string& userId,
    const std::string& name,
    const std::string& email,
    long int phone)
{
    return true;
}

/*
     * Function: viewProfile
     * Description: Retrieves the profile details of a specific user.
     * Parameters:
     *   - userId: Unique identifier of the user.
     * Returns: Pointer to the User object containing profile details, or nullptr if not found.
     */
User* UserManagementService::viewProfile(const std::string& userId) const
{
    return nullptr;
}

/*
     * Function: viewUserStatus
     * Description: Retrieves the current status of a user (active/inactive).
     * Parameters:
     *   - userId: Unique identifier of the user.
     * Returns: Integer representing the user status (enum placeholder).
     */
int UserManagementService::viewUserStatus(const std::string& userId) const
{
    return 0; // placeholder enum value
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
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
    int idNumber = static_cast<int>(users.size()) + 1;
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
    User* user = Factory::getObject<User>(generateUserId(), userName, email, password, phoneNumber, userType);
    if (user != nullptr)
    {
        m_dataStore.addUser(user);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
    * Function: getActiveUsers
    * Description: Retrieves a list of all active users in the system.
    * Parameters: None
    * Returns: Vector of User pointers representing all active users.
    */
const std::vector<const User*>& UserManagementService::getActiveUsers() const
{
    std::vector<const User*> constUsers;
    if (m_dataStore.getAuthenticatedUserType() == Enums::UserType::ADMIN)
    {
        const std::map<std::string, User*> users = m_dataStore.getUsers();
        for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
        {
            constUsers.push_back(iterator->second);
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
    std::map<std::string, User*> users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if (iterator->second->getUserId() == userId)
        {
            if (iterator->second->getStatus() == Enums::UserStatus::ACTIVE)
            {
                iterator->second->setStatus(Enums::UserStatus::INACTIVE);
                return Enums::ProcessStatus::SUCCESS;
            }
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
     * Function: rectivateUser
     * Description: Reactivates a previously deactivated user.
     * Parameters:
     *   - userId: Unique identifier of the user.
     * Returns: enum SUCCESS if the user is successfully reactivated, FAILURE otherwise.
     */
Enums::ProcessStatus UserManagementService::reactivateUser(const std::string& userId)
{
    std::map<std::string, User*> users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if (iterator->second->getUserId() == userId)
        {
            if (iterator->second->getStatus() == Enums::UserStatus::INACTIVE)
            {
                iterator->second->setStatus(Enums::UserStatus::ACTIVE);
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
    User* const authenticatedUser = m_dataStore.getAuthenticatedUser();
    std::string authenticatedUserPassword = authenticatedUser->getPassword();
    if (currentPassword == authenticatedUserPassword)
    {
        authenticatedUser->setPassword(newPassword);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}