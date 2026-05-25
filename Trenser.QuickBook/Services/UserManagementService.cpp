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
    const std::map<std::string, User*> users = m_dataStore.getUsers();
    std::ofstream userFile(PATH, std::ios::trunc);
    if (!userFile.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    userFile << "USER ID,USER NAME,EMAIL,PASSWORD,PHONE NUMBER,USER TYPE,STATUS\n";
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {

        std::string pasword = (iterator->second)->getPassword();
        encryption(pasword);
        userFile << (iterator->second)->getUserId() << ","
            << (iterator->second)->getUserName() << ","
            << (iterator->second)->getEmail() << ","
            << pasword << ","
            << (iterator->second)->getPhoneNumber() << ","
            << userTypeToString((iterator->second)->getUserType()) << ","
            << userStatusToString((iterator->second)->getStatus()) << "\n";
    }
    userFile.close();
}

/*
 * Function: UserManagementService::userTypeToString
 * Description: Converts a UserType enum value into its corresponding string representation.
 * Parameters:
 *    type - UserType enum value (ADMIN, CUSTOMER, THEATRE_OWNER)
 * Returns:
 *    String representation of the user type
 */
std::string UserManagementService::userTypeToString(Enums::UserType type)
{
    switch (type)
    {
    case Enums::UserType::ADMIN:
        return "ADMIN";
        break;
    case Enums::UserType::CUSTOMER:
        return "CUSTOMER";
        break;
    case Enums::UserType::THEATRE_OWNER:
        return "THEATRE OWNER";
        break;
    }
}

/*
 * Function: UserManagementService::userStatusToString
 * Description: Converts a UserStatus enum value into its corresponding string representation.
 * Parameters:
 *    status - UserStatus enum value (ACTIVE, INACTIVE)
 * Returns:
 *    String representation of the user status
 */
std::string UserManagementService::userStatusToString(Enums::UserStatus status)
{
    switch (status)
    {
    case Enums::UserStatus::ACTIVE:
        return "ACTIVE";
        break;
    case Enums::UserStatus::INACTIVE:
        return "INACTIVE";
        break;
    }
}

/*
 * Function: UserManagementService::reverseString
 * Description: Reverses the given string in place.
 * Parameters:
 *    password - Reference to the string to reverse
 * Returns:
 *    None (modifies the string directly)
 */
void UserManagementService::reverseString(std::string& password)
{
    int left = 0, right = password.length() - 1;
    while (left < right)
    {
        char temp = password[left];
        password[left] = password[right];
        password[right] = temp;
        left++;
        right--;
    }
}

/*
 * Function: UserManagementService::encryption
 * Description: Encrypts the given password by shifting each character by +10 in ASCII
 *              and then reversing the string.
 * Parameters:
 *    password - Reference to the string to encrypt
 * Returns:
 *    None (modifies the string directly)
 */
void UserManagementService::encryption(std::string& password)
{
    int index = 0;
    while (password[index] != '\0')
    {
        password[index] = char(int(password[index]) + 10);
        index++;
    }
    reverseString(password);
}

/*
 * Function: UserManagementService::decryption
 * Description: Decrypts the given password by reversing the string and shifting each character by -10 in ASCII.
 * Parameters:
 *    password - Reference to the string to decrypt
 * Returns:
 *    None (modifies the string directly)
 */
void UserManagementService::decryption(std::string& password)
{
    int index = 0;
    while (password[index] != '\0')
    {
        password[index] = char(int(password[index]) - 10);
        index++;
    }
    reverseString(password);
}