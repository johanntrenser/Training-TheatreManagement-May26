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