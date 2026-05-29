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

/*
* Function Name : getTickets
* Description   : Returns all tickets stored in the datastore.
* Parameters    : None
* Return Type   : std::map<std::string, Ticket*>&
*/
std::map<std::string, Ticket*>& DataStore::getTickets()
{
    return m_tickets;
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
    return m_screens;
}

/*
 * Function: DataStore::getPayments
 * Description: Retrieves the collection of payments stored in the DataStore.
 * Parameters: None
 * Returns:
 *    Constant reference to a map of payment IDs to Payment pointers.
 */
const std::map<std::string, Payment*>& DataStore::getPayments() const
{
    return m_payments;
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
 * Function: DataStore::getRefunds
 * Description: Retrieves the collection of getRefunds stored in the DataStore.
 * Parameters: None
 * Returns:
 *    Constant reference to a map of getRefunds IDs to getRefunds pointers.
 */
const std::map<std::string, Refund*>& DataStore::getRefunds() const
{
    return m_refunds;
}

/*
 * Function: DataStore::getSeats
 * Description: Retrieves all seats stored in the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    A constant reference to a map containing all Seat objects,
 *    keyed by their unique seat IDs.
 */
const std::map<std::string, Seat*>& DataStore::getSeats() const
{
    return m_seats;
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

/*
 * Function: DataStore::getBookings
 * Description: Provides access to the collection of bookings stored in the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    const std::map<std::string, Booking*>& - Map of booking IDs to Booking pointers
 */
const std::map<std::string, Booking*>& DataStore::getBookings() const
{
    return m_bookings;
}

void DataStore::addTicket(Ticket* ticket)
{
    m_tickets[ticket->getTicketId()] = ticket;
}
void DataStore::addTheatre(Theatre* theatre)
{
    m_theatres[theatre->getTheatreId()] = theatre;
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
 * Function: DataStore::getMovieById
 * Description: Retrieves a Movie object from the DataStore by its unique movie ID.
 *              Looks up the movie in the internal map of movies and returns the pointer
 *              if found. If the movie ID does not exist in the map, this will return nullptr.
 * Parameters:
 *    movieId - A reference to the string containing the unique movie identifier.
 * Returns:
 *    A pointer to the Movie object associated with the given ID, or nullptr if not found.
 */
Movie* DataStore::getMovieById(std::string& movieId)
{
    return m_movies[movieId];
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
 * Function: DataStore::addSeat
 * Description: Adds a Seat object to the DataStore.
 *              Stores the seat in the internal map of seats, keyed by its unique Seat ID.
 *              If a seat with the same ID already exists, it will be overwritten with the new pointer.
 * Parameters:
 *    seat - A pointer to the Seat object to be added.
 * Returns:
 *    None
 */
void DataStore::addSeat(Seat* seat)
{
    m_seats[seat->getSeatId()] = seat;
}
