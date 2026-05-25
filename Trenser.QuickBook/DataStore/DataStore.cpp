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
 * Function: DataStore::getLogs
 * Description: Retrieves the collection of logs stored in the DataStore.
 * Returns:
 *    A constant reference to the map of log IDs to Log pointers.
 */
const std::map<std::string, Log*>& DataStore::getLogs() const
{
    return m_logs;
}

/*
 * Function: DataStore::addLog
 * Description: Adds a new log entry to the DataStore by inserting the log object
 *              into the internal map keyed by the log's unique ID.
 * Parameters:
 *    log (Log*) - Pointer to the Log object to be added
 * Returns:
 *    None
 */
void DataStore::addLog(Log* log)
{
    m_logs[log->getLogId()] = log;
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
 * Function: getAuthenticatedUserType
 * Description: Retrieves the user type of the currently authenticated user.
 * Parameters:
 *    None
 * Returns:
 *    UserType enum representing the role of the authenticated user
 */
const Enums::UserType DataStore::getAuthenticatedUserType() const
{
    return m_currentUser->getUserType();
}

/*
 * Function: setAuthenticatedUserName
 * Description: Updates the username of the currently authenticated user.
 * Parameters:
 *    username - The new username to be set
 * Returns:
 *    None
 */
void DataStore::setAuthenticatedUserName(const std::string& username)
{
    m_currentUser->setUserName(username);
}

/*
 * Function: setAuthenticatedUserEmail
 * Description: Updates the email address of the currently authenticated user.
 * Parameters:
 *    email - The new email address to be set
 * Returns:
 *    None
 */
void DataStore::setAuthenticatedUserEmail(const std::string& email)
{
    m_currentUser->setEmail(email);
}

/*
 * Function: setAuthenticatedUserPhoneNumber
 * Description: Updates the phone number of the currently authenticated user.
 * Parameters:
 *    phoneNumber - The new phone number to be set
 * Returns:
 *    None
 */
void DataStore::setAuthenticatedUserPhoneNumber(const std::string& phoneNumber)
{
    m_currentUser->setPhoneNumber(phoneNumber);
}

/*
 * Function: getAuthenticatedUser
 * Description: Retrieves the currently authenticated user object.
 * Parameters:
 *    None
 * Returns:
 *    A pointer to the authenticated User object, or nullptr if no user is authenticated
 */
User* DataStore::getAuthenticatedUser() const
{
    return m_currentUser;
}

/*
* Function Name : getNotifications
* Description   : Returns all notifications stored in the datastore.
* Parameters    : None
* Return Type   : std::map<std::string, Notification*>&
*/
std::map<std::string, Notification*>& DataStore::getNotifications()
{
    return m_notifications;
}

/*
 * Function: DataStore::getMovies
 * Description: Retrieves the collection of movies stored in the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    Constant reference to a map of movie IDs to Movie pointers
 */
const std::map<string, Movie*>& DataStore::getMovies() const
{
    return m_movies;
}

/*
 * Function: DataStore::addMovieToSystem
 * Description: Adds a new movie to the DataStore, indexed by its unique movie ID.
 * Parameters:
 *    movie - Pointer to the Movie object to be added
 * Returns:
 *    None
 */
void DataStore::addMovieToSystem(Movie* movie)
{
    m_movies[movie->getMovieId()] = movie;
}

/*
 * Function: getShows
 * Description: Returns read?only access to all shows.
 * Parameters:
 *    None
 * Returns:
 *    Const reference to map of Show pointers
 */
const std::map<std::string, Show*>& DataStore::getShows() const
{
    return m_shows;
}

/*
 * Function: getShowsForUpdation
 * Description: Returns modifiable access to all shows.
 * Parameters:
 *    None
 * Returns:
 *    Reference to map of Show pointers
 */
std::map<std::string, Show*>& DataStore::getShowsForUpdation()
{
    return m_shows;
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