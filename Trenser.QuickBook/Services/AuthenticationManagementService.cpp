#include <sstream>
#include <iomanip>
#include "AuthenticationManagementService.h"
#include "Factory.h"
#include "DataStore.h"

/*
 * Function: AuthenticationManagementService
 * Description: Default constructor that initializes the authentication service
 *              with a reference to the shared DataStore instance.
 * Parameters: None
 * Returns: None
 */
AuthenticationManagementService::AuthenticationManagementService() :
    m_dataStore(DataStore::getInstance())
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
                    m_dataStore.setAuthenticatedUser(iterator->second);
                    return std::make_pair(Enums::LoginStatus::USER_FOUND, iterator->second->getUserType());
                }
                else
                {
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
    User* user = Factory::getObject<User>(generateUserId(), userName, email, password, phoneNumber, userType);
	if (user != nullptr)
	{
		m_dataStore.addUser(user);
		return Enums::ProcessStatus::SUCCESS;
	}
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
	const std::map<std::string, User*>& users = m_dataStore.getUsers();
	int idNumber = static_cast<int>(users.size()) + 1;
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
