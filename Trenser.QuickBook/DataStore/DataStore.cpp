/*
 * File: DataStore.cpp
 * Description: Defines the DataStore singleton class responsible for managing
 *              in-memory storage of system entities such as users, bookings,
 *              logs, movies, notifications, payments, refunds, screens, seats,
 *              shows, show seat availability, theatres, and tickets.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "DataStore.h"
#include <sstream>
using namespace std;

/*
 * Function: DataStore::getUsers
 * Description: Retrieves the collection of users stored in the DataStore.
 * Returns:
 *    A constant reference to the map of user IDs to User pointers.
 */
const std::map<string, User*>& DataStore::getUsers() const
{
        return m_users;
}

/*
 * Function: DataStore::addUser
 * Description: Adds a new user to the DataStore by inserting the user object
 *              into the internal map keyed by the user's unique ID.
 * Parameters:
 *    user (User*) - Pointer to the User object to be added
 * Returns:
 *    None
 */
void DataStore::addUser(User* user)
{
    m_users[user->getUserId()] = user;
}

/*
  * Function: DataStore::getInstance
  * Description: Provides access to the singleton instance of the DataStore class.
  *              Ensures only one instance exists throughout the application.
  * Returns:
  *    Reference to the single DataStore instance
  */
DataStore& DataStore::getInstance()
{
    static DataStore instance;
    return instance;
}

/*
 * Function: DataStore::setAuthenticatedUser
 * Description: Sets the currently authenticated user in the DataStore.
 * Parameters:
 *    user - Pointer to the User object representing the authenticated user
 * Returns:
 *    None
 */
void DataStore::setAuthenticatedUser(User* user)
{
    m_currentUser = user;
}

/*
 * Function: getScreens
 * Description: get all the screens in the datastore
 * Parameters:
 *    None
 * Returns:
 *    std::map<std::string, Screen*>& - reference of map of screens
 */
const std::map<std::string, Screen*>& DataStore::getScreens() const
{
    return m_screen;
}

/*
 * Function: DataStore::getTheatreById
 * Description: Retrieves a theatre object based on its unique identifier.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    Theatre* - Pointer to the theatre if found, nullptr otherwise
 */
Theatre* DataStore::getTheatreById(const std::string& theatreId) const
{
    std::map<std::string, Theatre*>::const_iterator iterator = m_theatre.find(theatreId);
    if (iterator == m_theatre.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
 * Function: DataStore::getScreenById
 * Description: Retrieves a screen object based on its unique identifier.
 * Parameters:
 *    screenId (const std::string&) - Unique identifier of the screen
 * Returns:
 *    Screen* - Pointer to the screen if found, nullptr otherwise
 */
Screen* DataStore::getScreenById(const std::string& screenId) const
{
    std::map<std::string, Screen*>::const_iterator iterator = m_screen.find(screenId);
    if (iterator == m_screen.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
* Function Name : getShows
* Description   : Returns all shows stored in the datastore.
* Parameters    : None
* Return Type   : const std::map<std::string, Show*>&
*/
const std::map<std::string, Show*>& DataStore::getShows() const
{
    return m_show;
}

/*
* Function Name : getAuthenticatedUserType
* Description   : Returns the type of the currently authenticated user.
* Parameters    : None
* Return Type   : Enums::UserType
*/
Enums::UserType DataStore::getAuthenticatedUserType()
{
    return m_currentUser->getUserType();
}