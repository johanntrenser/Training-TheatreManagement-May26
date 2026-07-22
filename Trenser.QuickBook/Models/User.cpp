/*
 * File: User.cpp
 * Description: Implements the User class, providing constructors, getters,
 *              and setters for user attributes such as ID, name, email,
 *              password, phone number, type, and status.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "User.h"

 /*
  * Function: User::User
  * Description: Default constructor. Initializes all user attributes to
  *              empty strings or zero values.
  * Returns: None
  */
User::User()
    : m_userId(""),
    m_userName(""),
    m_email(""),
    m_password(""),
    m_phoneNumber(""),
    m_userType(Enums::UserType::CUSTOMER),
    m_status(Enums::UserStatus::ACTIVE)
{}

/*
 * Function: User::User
 * Description: Parameterized constructor. Initializes user attributes with
 *              provided values.
 * Parameters:
 *    const std::string& userId - Unique identifier for the user
 *    const std::string& userName - Name of the user
 *    const std::string& email - Email address of the user
 *    const std::string& password - Password for authentication
 *    long int phoneNumber - Phone number of the user
 *    enum userType - Type of user (role)
 *    enum status - Current status of the user
 * Returns: None
 */
User::User(const std::string& userId,
    const std::string& userName,
    const std::string& email,
    const std::string& password,
    const std::string& phoneNumber,
    Enums::UserType userType)
    : m_userId(userId),
    m_userName(userName),
    m_email(email),
    m_password(password),
    m_phoneNumber(phoneNumber),
    m_userType(userType),
    m_status(Enums::UserStatus::ACTIVE)
{}

/*
 * Function: User::getUserId
 * Description: Retrieves the user ID.
 * Returns:
 *    const std::string& - User ID
 */
const std::string& User::getUserId() const
{
    return m_userId;
}

/*
 * Function: User::getUserName
 * Description: Retrieves the user name.
 * Returns:
 *    const std::string& - User name
 */
const std::string& User::getUserName() const
{
    return m_userName;
}

/*
 * Function: User::getEmail
 * Description: Retrieves the user email.
 * Returns:
 *    const std::string& - User email
 */
const std::string& User::getEmail() const
{
    return m_email;
}

/*
 * Function: User::getPassword
 * Description: Retrieves the user password.
 * Returns:
 *    const std::string& - User password
 */
const std::string& User::getPassword() const
{
    return m_password;
}

/*
 * Function: User::getPhoneNumber
 * Description: Retrieves the user phone number.
 * Returns:
 *    long int - Phone number
 */
const std::string& User::getPhoneNumber() const
{
    return m_phoneNumber;
}

/*
 * Function: User::getUserType
 * Description: Retrieves the user type.
 * Returns:
 *    int - User type
 */
Enums::UserType User::getUserType() const
{
    return m_userType;
}

/*
 * Function: User::getStatus
 * Description: Retrieves the user status.
 * Returns:
 *    int - User status
 */
Enums::UserStatus User::getStatus() const
{
    return m_status;
}

/*
 * Function: User::setUserId
 * Description: Updates the user ID.
 * Parameters:
 *    const std::string& userId - New user ID
 * Returns: None
 */
void User::setUserId(const std::string& userId)
{
    m_userId = userId;
}

/*
 * Function: User::setUserName
 * Description: Updates the user name.
 * Parameters:
 *    const std::string& userName - New user name
 * Returns: None
 */
void User::setUserName(const std::string& userName)
{
    m_userName = userName;
}

/*
 * Function: User::setEmail
 * Description: Updates the user email.
 * Parameters:
 *    const std::string& email - New email
 * Returns: None
 */
void User::setEmail(const std::string& email)
{
    m_email = email;
}

/*
 * Function: User::setPassword
 * Description: Updates the user password.
 * Parameters:
 *    const std::string& password - New password
 * Returns: None
 */
void User::setPassword(const std::string& password)
{
    m_password = password;
}

/*
 * Function: User::setPhoneNumber
 * Description: Updates the user phone number.
 * Parameters:
 *    long int phoneNumber - New phone number
 * Returns: None
 */
void User::setPhoneNumber(const std::string& phoneNumber)
{
    m_phoneNumber = phoneNumber;
}

/*
 * Function: User::setUserType
 * Description: Updates the user type.
 * Parameters:
 *    int userType - New user type
 * Returns: None
 */
void User::setUserType(Enums::UserType userType)
{
    m_userType = userType;
}

/*
 * Function: User::setStatus
 * Description: Updates the user status.
 * Parameters:
 *    int status - New status
 * Returns: None
 */
void User::setStatus(Enums::UserStatus status)
{
    m_status = status;
}

/*
* Function: serialize
* Description: Converts a User object into a SharedUser struct
*              suitable for storage in the memory-mapped file.
*              Encrypts the password before storing.
* Parameters: None
* Returns:
*    SharedUser - flat struct representation of this User
*/
SharedUser User::serialize()
{
    SharedUser sharedUser{};
    strncpy_s(sharedUser.userId, sizeof(sharedUser.userId), m_userId.c_str(), _TRUNCATE);
    strncpy_s(sharedUser.username, sizeof(sharedUser.username), m_userName.c_str(), _TRUNCATE);
    strncpy_s(sharedUser.email, sizeof(sharedUser.email), m_email.c_str(), _TRUNCATE);
    std::string passwordCopy = m_password;
    strncpy_s(sharedUser.password, sizeof(sharedUser.password), passwordCopy.c_str(), _TRUNCATE);
    strncpy_s(sharedUser.phoneNumber, sizeof(sharedUser.phoneNumber), m_phoneNumber.c_str(), _TRUNCATE);
    sharedUser.userType = static_cast<int>(m_userType);
    sharedUser.status = static_cast<int>(m_status);
    return sharedUser;
}

/*
 * Function: User::deserialize
 * Description: Deserializes a single SharedUser object into a User object.
 *              Extracts fields such as User ID, name, email, encrypted password, phone number,
 *              type, and status. The password is decrypted before constructing the User object.
 *              User type and status are converted from integer values into their respective enums.
 * Parameters:
 *    sharedUser - A pointer to a SharedUser object containing serialized user data.
 * Returns:
 *    A pointer to a newly created User object populated with the deserialized data.
 */
User* User::deserialize(const SharedUser* sharedUser)
{
    if (sharedUser == nullptr)
    {
        return nullptr;
    }
    std::string password(sharedUser->password);
    Enums::UserType userType = static_cast<Enums::UserType>(sharedUser->userType);
    Enums::UserStatus userStatus = static_cast<Enums::UserStatus>(sharedUser->status);
    User* user = Factory::getObject<User>(
        sharedUser->userId,
        sharedUser->username,
        sharedUser->email,
        password,
        sharedUser->phoneNumber,
        userType);
    if (user != nullptr)
    {
        user->setStatus(userStatus);
    }
    return user;
}