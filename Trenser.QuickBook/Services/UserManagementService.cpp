#include "UserManagementService.h"
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
     * Function: createUser
     * Description: Creates a new user with the provided details.
     * Parameters:
     *   - name: Full name of the user.
     *   - username: Unique username for login.
     *   - email: Email address of the user.
     *   - phone: Contact phone number.
     *   - password: Password for authentication.
     * Returns: True if the user is successfully created, false otherwise.
     */
bool UserManagementService::createUser(const std::string& name,
    const std::string& username,
    const std::string& email,
    long int phone,
    const std::string& password)
{
    return true;
}

/*
     * Function: deactivateUser
     * Description: Deactivates an active user, making them unable to log in.
     * Parameters:
     *   - userId: Unique identifier of the user.
     * Returns: True if the user is successfully deactivated, false otherwise.
     */
bool UserManagementService::deactivateUser(const std::string& userId)
{
    return true;
}

/*
     * Function: reactivateUser
     * Description: Reactivates a previously deactivated user.
     * Parameters:
     *   - userId: Unique identifier of the user.
     * Returns: True if the user is successfully reactivated, false otherwise.
     */
bool UserManagementService::reactivateUser(const std::string& userId)
{
    return true;
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
     * Function: changePassword
     * Description: Changes the password of a specific user.
     * Parameters:
     *   - userId: Unique identifier of the user.
     *   - newPassword: New password to be set.
     * Returns: None
     */
void UserManagementService::changePassword(const std::string& userId,
    const std::string& newPassword)
{
}

/*
 * Function: UserManagementService::saveData
 * Description: Saves all user data from the DataStore into a CSV file.
 *              Encrypts passwords before writing and overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void UserManagementService::saveUserData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::USER_HEADER);
    const std::map<std::string, User*> users = m_dataStore.getUsers();
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        lines.push_back((iterator->second)->serialize());
    }
    FileManagement::writeLines(std::string(config::File::USER_FILEPATH), lines);
}

/*
 * Function: UserManagementService::loadUserData
 * Description: Loads all user data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a User object via User::deserialize,
 *              and adds the User to the DataStore.
 *              This restores user information such as ID, name, email, password,
 *              phone number, type, and status into the system.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void UserManagementService::loadUserData()
{
    std::vector<std::string> lines = FileManagement::readlines(PATH);
    for (int index = 0; index < lines.size(); index++)
    {
        User* user = User::deserialize(lines[index]);
        m_dataStore.addUser(user);
    }
}