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
 * Function: DataStore::getTheatres
 * Description: Provides access to the collection of theatres stored in the datastore.
 *              Returns a reference to the internal map containing all theatre records.
 * Parameters: None
 * Returns:
 *    A constant reference to a map where the key is the theatre ID (string) and
 *    the value is a pointer to the Theatre object. The map may be empty if no
 *    theatres are currently stored.
 */
const std::map<std::string, Theatre*>& DataStore::getTheatres() const
{
    return m_theatres;
}

/*
 * Function: DataStore::getAuthenticatedUser
 * Description: Retrieves the currently authenticated user from the datastore.
 *              Provides access to the user object representing the active session.
 * Parameters: None
 * Returns:
 *    Pointer to the User object representing the authenticated user, or nullptr
 *    if no user is currently logged in.
 */
User* const DataStore::getAuthenticatedUser() const
{
    return m_currentUser;
}

/*
 * Function: DataStore::addTheatre
 * Description: Adds a new theatre object to the data store using
 *              the theatre ID as the key.
 * Parameters:
 *    theatre (Theatre*) - Pointer to the theatre object to be added
 * Returns:
 *    void
 */
void DataStore::addTheatre(Theatre* theatre)
{
    m_theatres[theatre->getTheatreId()] = theatre;
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
    std::map<std::string, Theatre*>::const_iterator iterator = m_theatres.find(theatreId);
    if (iterator == m_theatres.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
 * Function: DataStore::getMovieById
 * Description: Retrieves a movie object based on its unique identifier.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 * Returns:
 *    Movie* - Pointer to the movie if found, nullptr otherwise
 */
Movie* DataStore::getMovieById(const std::string& movieId) const
{
    std::map<std::string, Movie*>::const_iterator iterator = m_movies.find(movieId);
    if (iterator == m_movies.end())
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
    std::map<std::string, Screen*>::const_iterator iterator = m_screens.find(screenId);
    if (iterator == m_screens.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
 * Function: DataStore::addShow
 * Description: Stores a new show in the data store.
 * Parameters:
 *    show (Show*) - Pointer to the show object to be added
 * Returns:
 *    void
 */
void DataStore::addShow(Show* show)
{
    m_shows[show->getShowId()] = show;
}

/*
 * Function: DataStore::getShows
 * Description: Retrieves all shows currently stored in the system.
 * Parameters: None
 * Returns:
 *    const std::map<std::string, Show*>& - Map of show IDs to show objects
 */
const std::map<std::string, Show*>& DataStore::getShows() const
{
    return m_shows;
}
