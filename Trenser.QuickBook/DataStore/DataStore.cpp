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
    return m_screens;
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
 * Function: DataStore::getShowSeatAvailabilitys
 * Description: Retrieves the map of ShowSeatAvailability objects keyed by their unique IDs.
 * Returns:
 *    const std::map<std::string, ShowSeatAvailability*>& - Map of show seat availability entries
 */
const std::map<std::string, ShowSeatAvailability*>& DataStore::getShowSeatAvailabilitys() const
{
    return m_showSeatAvailabilitys;
}


/*
 * Function: DataStore::addShowSeatAvailability
 * Description: Adds a ShowSeatAvailability object to the datastore, keyed by its availability ID.
 * Parameters:
 *    ShowSeatAvailability* showSeatAvailability - Pointer to the ShowSeatAvailability object to add
 * Returns:
 *    void
 */
void DataStore::addShowSeatAvailability(ShowSeatAvailability* showSeatAvailability)
{
    m_showSeatAvailabilitys[showSeatAvailability->getShowAvailabiltyId()] = showSeatAvailability;
}

/*
 * Function: DataStore::getShowById
 * Description: Retrieves a show object in read-only mode using its unique ID.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    const Show* - Pointer to the show if found, nullptr otherwise
 */
const Show* DataStore::getShowById(const std::string& showId)
{
    std::map<std::string, Show*>::const_iterator iterator = m_shows.find(showId);
    if (iterator == m_shows.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
 * Function: DataStore::getShowByIdForUpdation
 * Description: Retrieves a modifiable show object using its unique ID for update operations.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    Show* - Pointer to the show if found, nullptr otherwise
 */
Show* DataStore::getShowByIdForUpdation(const std::string& showId)
{
    std::map<std::string, Show*>::const_iterator iterator = m_shows.find(showId);
    if (iterator == m_shows.end())
    {
        return nullptr;
    }
    return iterator->second;
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
* Function Name : addTicket
* Description   : Adds a ticket to the datastore.
* Parameters    :
*                  ticket - Ticket to be added
* Return Type   : void
*/
void DataStore::addTicket(Ticket* ticket)
{
    m_tickets[ticket->getTicketId()] = ticket;
}

/*
* Function Name : getTicketById
* Description   : Retrieves a ticket from the datastore based on the provided Ticket ID.
*                 Returns a pointer to the Ticket if found, otherwise returns nullptr.
* Parameters    :
*                  ticketId - The unique identifier of the ticket to be retrieved
* Return Type   : Ticket*
*/
Ticket* DataStore::getTicketById(const std::string& ticketId) const
{
    std::map<std::string, Ticket*>::const_iterator ticket = m_tickets.find(ticketId);
    return ticket->second;
}

/*
 * Function: DataStore::addPayment
 * Description: Adds a new payment object to the DataStore, indexed by its unique payment ID.
 * Parameters:
 *    payment - Pointer to the Payment object to be added.
 * Returns: None
 */
void DataStore::addPayment(Payment* payment)
{
    m_payments[payment->getPaymentId()] = payment;
}

/*
 * Function: DataStore::addRefund
 * Description: Adds a new addRefund object to the DataStore, indexed by its unique addRefund ID.
 * Parameters:
 *    payment - Pointer to the addRefund object to be added.
 * Returns: None
 */
void DataStore::addRefund(Refund* refund)
{
    m_refunds[refund->getRefundId()] = refund;
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
 * Function: DataStore::getBookingById
 * Description: Retrieves a booking object from the DataStore by its unique ID.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    const Booking* - Pointer to the booking if found, nullptr otherwise
 */
const Booking* DataStore::getBookingById(const std::string& bookingId)
{
    std::map<std::string, Booking*>::const_iterator iterator = m_bookings.find(bookingId);
    if (iterator == m_bookings.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
 * Function: DataStore::getBookingByIdForUpdation
 * Description: Retrieves a booking object by ID for modification.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    Booking* - Pointer to the booking if found, nullptr otherwise
 */
Booking* DataStore::getBookingByIdForUpdation(const std::string& bookingId)
{
    std::map<std::string, Booking*>::const_iterator iterator = m_bookings.find(bookingId);
    if (iterator == m_bookings.end())
    {
        return nullptr;
    }
    return iterator->second;
}
