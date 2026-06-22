/*
 * File: DataStore.cpp
 * Description: Defines the DataStore singleton class responsible for managing
 *              in-memory storage of system entities such as users, bookings,
 *              logs, movies, notifications, payments, refunds, screens, seats,
 *              shows, show seat availability, theatres, and tickets.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <sstream>
#include <string>
#include "DataStore.h"
using namespace std;

/*
 * Function: DataStore::initialize
 * Description: Opens and initializes all mapped files managed by the
 *              MappedFileRegistry and loads data into datastore.
 * Parameters: None
 * Returns:
 *    true if all mapped files were initialized successfully and all data were loaded into datastore,
 *    false otherwise
 */
bool DataStore::initialize()
{
    ensureFolder(config::File::FILEPATH);
    if (!(m_registry.openAll() && m_sessionManager.open()))
    {
        return false;
    }
    refreshUsers();
    refreshMovies();
    refreshTheatres();
    refreshScreens();
    refreshSeats();
    refreshShows();
    refreshShowSeatAvailabilitys();
    refreshBookings();
    refreshPayments();
    refreshTickets();
    refreshRefunds();
    refreshNotifications();
    refreshLogs();
    rebuildTheatreRelationships();
    rebuildScreenRelationships();
    rebuildSeatRelationships();
    rebuildShowRelationships();
    rebuildShowSeatAvailabilityRelationships();
    rebuildBookingRelationships();
    rebuildPaymentRelationships();
    rebuildTicketRelationships();
    rebuildRefundRelationships();
    rebuildNotificationRelationships();
    return true;
}

/*
 * Function: DataStore::getUsers
 * Description: Retrieves the collection of users stored in the DataStore.
 * Returns:
 *    A constant reference to the map of user IDs to User pointers.
 */
const std::map<string, User*>& DataStore::getUsers()
{
    refreshUsers();
    return m_users;
}

/*
 * Function: DataStore::add User
 * Description: Serializes a user object, adds it to the mapped users file, and releases heap memory.
 * Parameters:
 *    user - Pointer to the User object to be added
 * Returns:
 *    None
 */
void DataStore::addUser(User* user)
{
    SharedUser sharedUser = user->serialize();
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    if (usersFile)
    {
        usersFile->addRecord(sharedUser);
    }
    m_users[user->getUserId()] = user;
}

/*
 * Function: DataStore::getLogs
 * Description: Retrieves the collection of logs stored in the DataStore.
 * Returns:
 *    A constant reference to the map of log IDs to Log pointers.
 */
const std::map<std::string, Log*>& DataStore::getLogs()
{
    refreshLogs();
    return m_logs;
}

/*
 * Function: DataStore::addLog
 * Description: Serializes a log object, adds it to the mapped logs file, and releases heap memory.
 * Parameters:
 *    user - Pointer to the Log object to be added
 * Returns:
 *    None
 */
void DataStore::addLog(Log* log)
{
    SharedLog sharedLog = log->serialize();
    MappedFile<SharedLog>* logsFile = m_registry.getLogs();
    if (logsFile)
    {
        logsFile->addRecord(sharedLog);
    }
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
    if (!m_currentUser)
    {
        return;
    }
    m_currentUser->setUserName(username);
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    SharedUser* sharedUser = usersFile->findById(m_currentUser->getUserId().c_str());
    if (sharedUser)
    {
        strncpy_s(sharedUser->username, username.c_str(), sizeof(sharedUser->username));
    }
    usersFile->flush();
    return;
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
    if (!m_currentUser)
    {
        return;
    }
    m_currentUser->setEmail(email);
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    SharedUser* sharedUser = usersFile->findById(m_currentUser->getUserId().c_str());
    if (sharedUser)
    {
        strncpy_s(sharedUser->email, email.c_str(), sizeof(sharedUser->email));
    }
    usersFile->flush();
    return;
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
    if (!m_currentUser)
    {
        return;
    }
    m_currentUser->setPhoneNumber(phoneNumber);
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    SharedUser* sharedUser = usersFile->findById(m_currentUser->getUserId().c_str());
    if (sharedUser)
    {
        strncpy_s(sharedUser->phoneNumber, phoneNumber.c_str(), sizeof(sharedUser->phoneNumber));
    }
    usersFile->flush();
    return;
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
 * Function: DataStore::getNotifications
 * Description: Retrieves all Notification records from shared memory and loads them into the DataStore.
 *              Clears any existing notification data, fetches the mapped file of SharedNotification records
 *              from the registry, deserializes each record into a Notification object, restores its
 *              association with the corresponding User (receiver), and stores it in the internal map
 *              keyed by Notification ID. Returns the updated map of notifications.
 * Parameters:
 *    None
 * Returns:
 *    A reference to std::map<std::string, Notification*> containing all Notification objects
 *    currently loaded in the DataStore.
 */
std::map<std::string, Notification*>& DataStore::getNotifications()
{
    refreshNotifications();
    rebuildNotificationRelationships();
    return m_notifications;
}

/*
 * Function: DataStore::getMovies
 * Description: Retrieves all Movie records from shared memory and loads them into the DataStore.
 *              Clears any existing movie data, fetches the mapped file of SharedMovie records
 *              from the registry, deserializes each record into a Movie object, and stores
 *              them in the internal map keyed by Movie ID. Returns the updated map of movies.
 * Parameters:
 *    None
 * Returns:
 *    A const reference to std::map<std::string, Movie*> containing all Movie objects
 *    currently loaded in the DataStore.
 */
const std::map<string, Movie*>& DataStore::getMovies()
{
    refreshMovies();
    return m_movies;
}

/*
 * Function: DataStore::addMovieToSystem
 * Description: Adds a Movie object to the system by serializing it into a SharedMovie record
 *              and persisting it in shared memory via the registry. Retrieves the mapped file
 *              for movies, appends the serialized record if available, and then deletes the
 *              original Movie pointer to prevent memory leaks. This ensures that movies are
 *              stored centrally in shared memory for system-wide access.
 * Parameters:
 *    movie - A pointer to the Movie object to be added to the system.
 * Returns:
 *    None
 */
void DataStore::addMovieToSystem(Movie* movie)
{
    SharedMovie sharedMovie = movie->serialize();
    MappedFile<SharedMovie>* movieFile = m_registry.getMovies();
    if (movieFile)
    {
        movieFile->addRecord(sharedMovie);
    }
    m_movies[movie->getMovieId()] = movie;
}

/*
 * Function: DataStore::getShows
 * Description: Retrieves all Show objects from the mapped file registry.
 *              Deserializes each SharedShow record, links associated Movie,
 *              Screen, and ShowSeatAvailability objects, and caches them in
 *              the internal map. Provides read-only access to the collection.
 * Parameters:
 *    None
 * Returns:
 *    const std::map<std::string, Show*>& - Constant reference to the map of Show
 *    objects keyed by Show ID. May be empty if no shows exist.
 */
const std::map<std::string, Show*>& DataStore::getShows()
{
    refreshShows();
    rebuildShowRelationships();
    return m_shows;
}

/*
 * Function: DataStore::getShowsForUpdation
 * Description: Retrieves all Show objects from the mapped file registry with
 *              modifiable access. Deserializes each SharedShow record, links
 *              associated Movie, Screen, and ShowSeatAvailability objects, and
 *              caches them in the internal map. Allows updates to the collection.
 * Parameters:
 *    None
 * Returns:
 *    std::map<std::string, Show*>& - Reference to the map of Show objects keyed
 *    by Show ID. May be empty if no shows exist.
 */
std::map<std::string, Show*>& DataStore::getShowsForUpdation()
{
    refreshShows();
    rebuildShowRelationships();
    return m_shows;
}

/*
 * Function: getScreens
 * Description: Retrieves all Screen objects from the mapped screens file.
 *              Deserializes SharedScreen records, links them to their Theatre,
 *              and registers them in the internal screen map. Avoids duplicate
 *              entries by checking existing screen IDs.
 * Parameters:
 *    None
 * Returns:
 *    const std::map<std::string, Screen*>& - Constant reference to the map of screen IDs to Screen pointers
 */
const std::map<std::string, Screen*>& DataStore::getScreens()
{
    refreshScreens();
    rebuildScreenRelationships();
    return m_screens;
}

/*
 * Function: DataStore::getTheatres
 * Description: Retrieves all Theatre objects from the mapped file registry.
 *              Deserializes each SharedTheatre record, links TheatreOwner,
 *              associated Movies, and Screens, and stores them in the internal map.
 * Parameters:
 *    None
 * Returns:
 *    const std::map<std::string, Theatre*>& - Reference to the map of Theatre objects,
 *    keyed by theatre ID. May be empty if no theatres exist.
 */
const std::map<std::string, Theatre*>& DataStore::getTheatres()
{
    refreshTheatres();
    rebuildTheatreRelationships();
    return m_theatres;
}

/*
 * Function: getBookings
 * Description: Retrieves all Booking objects from the mapped bookings file.
 *              Deserializes SharedBooking records, links them to their associated
 *              User, Show, and Seat objects, and registers them in the internal
 *              bookings map keyed by booking ID.
 * Parameters:
 *    None
 * Returns:
 *    Constant reference to the map of Booking pointers keyed by booking ID,
 *    representing all bookings currently loaded into the DataStore
 */
const std::map<std::string, Booking*>& DataStore::getBookings()
{
    refreshBookings();
    rebuildBookingRelationships();
    return m_bookings;
}

/*
 * Function: DataStore::addTheatre
 * Description: Adds a Theatre object to the datastore and persists it in the
 *              mapped file registry. Serializes the Theatre into a SharedTheatre
 *              record, appends it to the mapped file, and updates the internal
 *              theatre map. If an existing entry is present, it is deleted and
 *              replaced with the new one.
 * Parameters:
 *    theatre (Theatre*) - Pointer to the Theatre object to be added
 * Returns:
 *    void
 */
void DataStore::addTheatre(Theatre* theatre)
{
    SharedTheatre sharedTheatre = theatre->serialize();
    MappedFile<SharedTheatre>* theatreFile = m_registry.getTheatres();
    if (theatreFile)
    {
        theatreFile->addRecord(sharedTheatre);
    }
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
Theatre* DataStore::getTheatreById(const std::string& theatreId)
{
    refreshTheatreById(theatreId);
    rebuildTheatreRelationship(theatreId);
    return findTheatreById(theatreId);
}

/*
 * Function: DataStore::getMovieById
 * Description: Retrieves a Movie object by its unique identifier from the mapped file.
 *              Deserializes the corresponding SharedMovie record and caches the Movie
 *              in the datastore for reuse. Linking to related entities (e.g., theatres)
 *              is deferred to higher-level services.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 * Returns:
 *    Movie* - Pointer to the Movie object if found and deserialized successfully,
 *             nullptr otherwise
 */
Movie* DataStore::getMovieById(const std::string& movieId)
{
    refreshMovieById(movieId);
    return findMovieById(movieId);
}

/*
 * Function: DataStore::getScreenById
 * Description: Retrieves a Screen object by its unique identifier from the mapped file.
 *              Deserializes the SharedScreen record, constructs the seat grid,
 *              links each Seat to the Screen, and caches the Screen in the datastore.
 * Parameters:
 *    screenId (const std::string&) - Unique identifier of the screen
 * Returns:
 *    Screen* - Pointer to the Screen object if found, nullptr otherwise
 */
Screen* DataStore::getScreenById(const std::string& screenId)
{
    refreshScreenById(screenId);
    rebuildScreenRelationship(screenId);
    return findScreenById(screenId);
}

/*
 * Function: DataStore::addShow
 * Description: Serializes a Show object, adds it to the mapped shows file,
 *              and registers the Show pointer in the internal map keyed by Show ID.
 * Parameters:
 *    show (Show*) - Pointer to the Show object to be added
 * Returns:
 *    void
 */
void DataStore::addShow(Show* show)
{
    SharedShow sharedShow = show->serialize();
    MappedFile<SharedShow>* showsFile = m_registry.getShows();
    if (showsFile)
    {
        showsFile->addRecord(sharedShow);
    }
    m_shows[show->getShowId()] = show;
}

/*
 * Function: DataStore::getShowSeatAvailabilitys
 * Description: Retrieves the map of ShowSeatAvailability objects keyed by their unique IDs.
 * Returns:
 *    const std::map<std::string, ShowSeatAvailability*>& - Map of show seat availability entries
 */
const std::map<std::string, ShowSeatAvailability*>& DataStore::getShowSeatAvailabilitys()
{
    refreshShowSeatAvailabilitys();
    rebuildShowSeatAvailabilityRelationships();
    return m_showSeatAvailabilitys;
}

/*
 * Function: DataStore::addShowSeatAvailability
 * Description: Serializes a ShowSeatAvailability object, adds it to the mapped file,
 *              and registers it in the internal map keyed by availability ID.
 * Parameters:
 *    showSeatAvailability (ShowSeatAvailability*) - Pointer to the object to add
 * Returns:
 *    void
 */
void DataStore::addShowSeatAvailability(ShowSeatAvailability* showSeatAvailability)
{

    SharedShowSeatAvailability* sharedSeatAvailability = new SharedShowSeatAvailability();
    showSeatAvailability->serialize(*sharedSeatAvailability);
    MappedFile<SharedShowSeatAvailability>* availabilityFile = m_registry.getAvailability();
    if (availabilityFile)
    {
        availabilityFile->addRecord(*sharedSeatAvailability);
    }
    m_showSeatAvailabilitys[showSeatAvailability->getShowAvailabiltyId()] = showSeatAvailability;
    delete sharedSeatAvailability;
}

/*
 * Function: DataStore::getShowById
 * Description: Retrieves a Show object in read-only mode using its unique identifier.
 *              Deserializes the corresponding SharedShow record, links associated
 *              Movie, Screen, and ShowSeatAvailability objects, and caches the Show
 *              in the datastore. Existing entries are deleted and replaced.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    const Show* - Pointer to the Show object if found and deserialized successfully,
 *                  nullptr otherwise
 */
const Show* DataStore::getShowById(const std::string& showId)
{
    refreshShowById(showId);
    rebuildShowRelationship(showId);
    return findShowById(showId);
}

/*
 * Function: DataStore::getShowByIdForUpdation
 * Description: Retrieves a Show object in modifiable mode using its unique identifier.
 *              Deserializes the corresponding SharedShow record, links associated
 *              Movie, Screen, and ShowSeatAvailability objects, and caches the Show
 *              in the datastore. Existing entries are deleted and replaced, allowing
 *              updates to the Show object.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    Show* - Pointer to the Show object if found and deserialized successfully,
 *            nullptr otherwise
 */
Show* DataStore::getShowByIdForUpdation(const std::string& showId)
{
    refreshShowById(showId);
    rebuildShowRelationship(showId);
    return findShowById(showId);
}

/*
* Function Name : getTickets
* Description   : Returns all tickets stored in the mapped file of ticket records.
* Parameters    : None
* Return Type   : std::map<std::string, Ticket*>&
*/
std::map<std::string, Ticket*>& DataStore::getTickets()
{
    refreshTickets();
    rebuildTicketRelationships();
    return m_tickets;
}

/*
 * Function: addTicket
 * Description: Serializes a Ticket object into a SharedTicket struct,
 *              adds it to the mapped tickets file,
 *              and registers the Ticket pointer in the internal map
 *              keyed by its unique Ticket ID.
 * Parameters:
 *    ticket - Pointer to the Ticket object to be added
 * Returns:
 *    None
 */
void DataStore::addTicket(Ticket* ticket)
{
    SharedTicket sharedTicket{};
    ticket->serialize(sharedTicket);
    MappedFile<SharedTicket>* ticketFile = m_registry.getTickets();
    if (ticketFile)
    {
        ticketFile->addRecord(sharedTicket);
    }
    m_tickets[ticket->getTicketId()] = ticket;
}

/*
 * Function: getTicketById
 * Description: Retrieves a Ticket object from the mapped tickets file by its unique ID.
 *              Locates the corresponding SharedTicket record, deserializes it into
 *              a Ticket object, and registers it in the internal tickets map for quick lookup.
 * Parameters:
 *    ticketId - Unique identifier of the ticket to retrieve
 * Returns:
 *    Pointer to the Ticket object if found and deserialized successfully,
 *    nullptr if the ticket record does not exist or deserialization fails
 */
Ticket* DataStore::getTicketById(const std::string& ticketId)
{
    refreshTicketById(ticketId);
    rebuildTicketRelationship(ticketId);
    return findTicketById(ticketId);
}

/*
 * Function: DataStore::addPayment
 * Description: Adds a new Payment object to the DataStore. Serializes the Payment
 *              into a SharedPayment structure and persists it into the mapped file
 *              storage. Also updates the in-memory payments collection with the
 *              Payment pointer keyed by its unique payment ID.
 * Parameters:
 *    payment - Pointer to the Payment object to be added.
 * Returns: None
 */
void DataStore::addPayment(Payment* payment)
{
    SharedPayment sharedPayment{};
    payment->serialize(sharedPayment);
    MappedFile<SharedPayment>* paymentFile = m_registry.getPayments();
    if (paymentFile)
    {
        paymentFile->addRecord(sharedPayment);
    }
    m_payments[payment->getPaymentId()] = payment;
}

/*
 * Function: DataStore::addRefund
 * Description: Adds a new Refund object to the DataStore. Serializes the Refund
 *              into a SharedRefund structure and persists it into the mapped file
 *              storage. Also updates the in-memory refunds collection with the
 *              Refund pointer keyed by its unique refund ID.
 * Parameters:
 *    refund - Pointer to the Refund object to be added.
 * Returns: None
 */
void DataStore::addRefund(Refund* refund)
{
    SharedRefund sharedRefund{};
    refund->serialize(sharedRefund);
    MappedFile<SharedRefund>* refundFile = m_registry.getRefunds();
    if (refundFile)
    {
        refundFile->addRecord(sharedRefund);
    }
    m_refunds[refund->getRefundId()] = refund;
}

/*
 * Function: DataStore::getRefunds
 * Description: Retrieves all refunds from persistent storage and reconstructs
 *              them into Refund objects. Deserializes records from the mapped file,
 *              associates each Refund with its corresponding Ticket if available,
 *              and updates the internal refunds map keyed by refund ID.
 * Parameters: None
 * Returns:
 *    A constant reference to the map of Refund pointers, keyed by their unique
 *    refund IDs, representing all refunds currently loaded into the DataStore.
 */
const std::map<std::string, Refund*>& DataStore::getRefunds()
{
    refreshRefunds();
    rebuildRefundRelationships();
    return m_refunds;
}

/*
 * Function: DataStore::getPayments
 * Description: Retrieves all payments from persistent storage and reconstructs
 *              them into Payment objects. Clears existing in-memory data,
 *              deserializes records from the mapped file, and associates each
 *              Payment with its corresponding Booking if available. Updates
 *              the internal payments map keyed by payment ID.
 * Parameters: None
 * Returns:
 *    A constant reference to the map of Payment pointers, keyed by their unique
 *    payment IDs, representing all payments currently loaded into the DataStore.
 */
const std::map<std::string, Payment*>& DataStore::getPayments()
{
    refreshPayments();
    rebuildPaymentRelationships();
    return m_payments;
}

/*
 * Function: DataStore::getBookingById
 * Description: Retrieves a Booking object from the mapped bookings file by its unique ID.
 *              Locates the corresponding SharedBooking record, deserializes it into
 *              a Booking object, and restores associations with its related Customer,
 *              Show, and Seat objects. Registers the reconstructed Booking in the
 *              internal bookings map keyed by booking ID.
 * Parameters:
 *    bookingId - Unique identifier of the booking to retrieve
 * Returns:
 *    Pointer to the Booking object if found and deserialized successfully,
 *    nullptr if the booking record does not exist or deserialization fails
 */
Booking* DataStore::getBookingById(const std::string& bookingId)
{
    refreshBookingById(bookingId);
    rebuildBookingRelationship(bookingId);
    return findBookingById(bookingId);
}

/*
 * Function: DataStore::getBookingByIdForUpdation
 * Description: Retrieves a Booking object from the mapped bookings file by its unique ID.
 *              Locates the corresponding SharedBooking record, deserializes it into
 *              a Booking object, and restores associations with its related Customer,
 *              Show, and Seat objects. Registers the reconstructed Booking in the
 *              internal bookings map keyed by booking ID.
 * Parameters:
 *    bookingId - Unique identifier of the booking to retrieve
 * Returns:
 *    Pointer to the Booking object if found and deserialized successfully,
 *    nullptr if the booking record does not exist or deserialization fails
 */
Booking* DataStore::getBookingByIdForUpdation(const std::string& bookingId)
{
    refreshBookingById(bookingId);
    rebuildBookingRelationship(bookingId);
    return findBookingById(bookingId);
}

/*
 * Function: DataStore::getSeatById
 * Description: Retrieves a Seat object by its unique identifier from the mapped file.
 *              Deserializes the SharedSeat record and caches the Seat in the datastore.
 *              Linking to its Screen is deferred until Screen construction.
 * Parameters:
 *    seatId (const std::string) - Unique identifier of the seat
 * Returns:
 *    Seat* - Pointer to the Seat object if found, nullptr otherwise
 */
Seat* DataStore::getSeatById(const std::string seatId)
{
    refreshSeatById(seatId);
    rebuildSeatRelationship(seatId);
    return findSeatById(seatId);
}

/*
 * Function: DataStore::addBooking
 * Description: Serializes a Booking object into a SharedBooking struct,
 *              adds it to the mapped bookings file for persistence,
 *              and registers the Booking pointer in the internal map
 *              keyed by its unique Booking ID.
 * Parameters:
 *    booking - Pointer to the Booking object to be added
 * Returns:
 *    None
 */
void DataStore::addBooking(Booking* booking)
{
    SharedBooking* sharedBooking = new SharedBooking();
    booking->serialize(*sharedBooking);
    MappedFile<SharedBooking>* bookingFile = m_registry.getBookings();
    if (bookingFile)
    {
        bookingFile->addRecord(*sharedBooking);
    }
    m_bookings[booking->getBookingId()] = booking;
    delete sharedBooking;
}

/*
* Function Name : getTicketForBooking
* Description   : Retrieves the ticket associated with the provided booking.
*                 Searches for the payment linked to the booking and returns
*                 the corresponding ticket if found.
* Parameters    :
*                  booking - Booking whose ticket is to be retrieved
* Return Type   : const Ticket*
*/
const Ticket* DataStore::getTicketForBooking(const Booking* booking)
{
    if (!booking)
    {
        return nullptr;
    }
    const std::map<std::string, Payment*>& payments = getPayments();
    std::string paymentId;
    std::string bookingId = booking->getBookingId();
    for (std::map<std::string, Payment*>::const_iterator iterator = payments.begin(); iterator != payments.end(); ++iterator)
    {
        if (iterator->second != nullptr)
        {
            Booking* paymentBooking = iterator->second->getBooking();
            if (paymentBooking != nullptr && paymentBooking->getBookingId() == bookingId)
            {
                paymentId = iterator->second->getPaymentId();
                break;
            }
        }
    }
    if (paymentId.empty())
    {
        return nullptr;
    }
    std::map<std::string, Ticket*>& tickets = getTickets();
    for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getPayment() && iterator->second->getPayment()->getPaymentId() == paymentId)
        {
            return iterator->second;
        }
    }
    return nullptr;
}

/*
 * Function: DataStore::getSeats
 * Description: Retrieves all Seat objects from the mapped seats file.
 *              Clears existing in-memory data, deserializes SharedSeat records,
 *              links each Seat to its corresponding Screen, and registers them
 *              in the internal seats map keyed by seat ID.
 * Parameters:
 *    None
 * Returns:
 *    Constant reference to the map of Seat pointers keyed by seat ID,
 *    representing all seats currently loaded into the DataStore
 */
const std::map<std::string, Seat*>& DataStore::getSeats()
{
    refreshSeats();
    rebuildSeatRelationships();
    return m_seats;
}

/*
 * Function: getUserById
 * Description: Retrieves a User object from the mapped users file by its unique ID.
 *              Locates the corresponding SharedUser record, deserializes it into
 *              a User object, and registers it in the internal user map for quick lookup.
 * Parameters:
 *    userId - Unique identifier of the user to retrieve
 * Returns:
 *    Pointer to the User object if found and deserialized successfully,
 *    nullptr if the user record does not exist or deserialization fails
 */
User* DataStore::getUserById(const std::string& userId)
{
    refreshUserById(userId);
    return findUserById(userId);
}

/*
 * Function: addSeat
 * Description: Serializes a Seat object, adds it to the mapped seats file,
 *              and registers it in the internal seat map for quick lookup.
 * Parameters:
 *    seat - Pointer to the Seat object to be added
 * Returns:
 *    None
 */

void DataStore::addSeat(Seat* seat)
{
    SharedSeat sharedSeat = seat->serialize();
    MappedFile<SharedSeat>* seatFile = m_registry.getSeats();
    if (seatFile)
    {
        seatFile->addRecord(sharedSeat);
    }
    m_seats[seat->getSeatId()] = seat;
}

/*
 * Function: addScreen
 * Description: Serializes a Screen object, adds it to the mapped screens file,
 *              and registers it in the internal screen map for quick lookup.
 * Parameters:
 *    screen - Pointer to the Screen object to be added
 * Returns:
 *    None
 */
void DataStore::addScreen(Screen* screen)
{
    SharedScreen* sharedScreen = new SharedScreen();
    screen->serialize(*sharedScreen);
    MappedFile<SharedScreen>* screenFile = m_registry.getScreens();
    if (screenFile)
    {
        screenFile->addRecord(*sharedScreen);
    }
    m_screens[screen->getScreenId()] = screen;
    delete sharedScreen;
}

/*
 * Function: DataStore::addNotification
 * Description: Adds a Notification object to the system by serializing it into a SharedNotification record
 *              and persisting it in shared memory via the registry. Retrieves the mapped file for notifications,
 *              appends the serialized record if available, and then deletes the original Notification pointer
 *              to prevent memory leaks. This ensures that notifications are stored centrally in shared memory
 *              for system-wide access.
 * Parameters:
 *    notification - A pointer to the Notification object to be added to the system.
 * Returns:
 *    None
 */
void DataStore::addNotification(Notification* notification)
{
    SharedNotification sharedNotification = notification->serialize();
    MappedFile<SharedNotification>* notificationFile = m_registry.getNotifications();
    if (notificationFile)
    {
        notificationFile->addRecord(sharedNotification);
    }
    m_notifications[notification->getNotificationId()] = notification;
}

/*
 * Function: DataStore::getBookingDetailsById
 * Description: Retrieves a booking object from the DataStore by its unique ID.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    const Booking* - Pointer to the booking if found, nullptr otherwise
 */
Booking* DataStore::getBookingDetailsById(const std::string& bookingId)
{
    refreshBookingById(bookingId);
    rebuildBookingRelationship(bookingId);
    return findBookingById(bookingId);
}

/*
 * Function: getUsersCount
 * Description: Retrieves the total number of users from the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of users
 */
int DataStore::getUsersCount() const
{
    int count = m_registry.getUsersCount();
    return count;
}

/*
 * Function: getLogsCount
 * Description: Retrieves the total number of logs from the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of logs
 */
int DataStore::getLogsCount() const
{
    int count = m_registry.getLogsCount();
    return count;
}

/*
 * Function: getTheatresCount
 * Description: Retrieves the total number of theatres from the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of theatres
 */
int DataStore::getTheatresCount() const
{
    int count = m_registry.getTheatresCount();
    return count;
}

/*
 * Function: DataStore::getShowById
 * Description: Retrieves a Show object from the DataStore by its unique show ID.
 *              Looks up the show in the internal map of shows and returns the pointer
 *              if found. If the show ID does not exist in the map, this will return nullptr.
 * Parameters:
 *    id - A reference to the string containing the unique show identifier.
 * Returns:
 *    A pointer to the Show object associated with the given ID, or nullptr if not found.
 */
Show* DataStore::getShowDetailsById(std::string& id)
{
    refreshShowById(id);
    rebuildShowRelationship(id);
    return findShowById(id);
}

/*
 * Function: updateTicketStatus
 * Description: Updates the status of a ticket in the mapped tickets file.
 *              Locates the ticket record by ID, modifies its status.
 * Parameters:
 *    ticketId - Identifier of the ticket to update
 *    status   - New ticket status to be applied
 * Returns:
 *    ProcessStatus::SUCCESS if update applied successfully,
 *    ProcessStatus::FAILED if the ticket or file could not be found
 */
Enums::ProcessStatus DataStore::updateTicketStatus(const std::string& ticketId, Enums::TicketStatus status)
{
    MappedFile<SharedTicket>* ticketFile = m_registry.getTickets();
    if (!ticketFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTicket* sharedTicket = ticketFile->findById(ticketId.c_str());
    if (!sharedTicket)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedTicket->status = static_cast<int>(status);
    ticketFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::clearData
 * Description: Clears all in-memory maps maintained by the DataStore singleton.
 *              Iterates through each container, deletes every heap-allocated
 *              object, and then empties the map to release ownership. Ensures
 *              proper cleanup of dynamically allocated resources to prevent
 *              memory leaks.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::clearData()
{
    for (std::map<std::string, Ticket*>::iterator iterator = m_tickets.begin(); iterator != m_tickets.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_tickets.clear();
    for (std::map<std::string, Refund*>::iterator iterator = m_refunds.begin(); iterator != m_refunds.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_refunds.clear();
    for (std::map<std::string, Payment*>::iterator iterator = m_payments.begin(); iterator != m_payments.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_payments.clear();
    for (std::map<std::string, Booking*>::iterator iterator = m_bookings.begin(); iterator != m_bookings.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_bookings.clear();
    for (std::map<std::string, ShowSeatAvailability*>::iterator iterator = m_showSeatAvailabilitys.begin(); iterator != m_showSeatAvailabilitys.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_showSeatAvailabilitys.clear();
    for (std::map<std::string, Show*>::iterator iterator = m_shows.begin(); iterator != m_shows.end(); ++iterator)
    {
        delete iterator->second;
    }

    m_shows.clear();
    for (std::map<std::string, Seat*>::iterator iterator = m_seats.begin(); iterator != m_seats.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_seats.clear();
    for (std::map<std::string, Screen*>::iterator iterator = m_screens.begin(); iterator != m_screens.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_screens.clear();
    for (std::map<std::string, Theatre*>::iterator iterator = m_theatres.begin(); iterator != m_theatres.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_theatres.clear();
    for (std::map<std::string, Movie*>::iterator iterator = m_movies.begin(); iterator != m_movies.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_movies.clear();
    for (std::map<std::string, Notification*>::iterator iterator = m_notifications.begin(); iterator != m_notifications.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_notifications.clear();
    for (std::map<std::string, Log*>::iterator iterator = m_logs.begin(); iterator != m_logs.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_logs.clear();
    for (std::map<std::string, User*>::iterator iterator = m_users.begin(); iterator != m_users.end(); ++iterator)
    {
        delete iterator->second;
    }
    m_users.clear();
}

/*
* Function: setAuthenticatedUserPassword
* Description : Updates the password of the currently authenticated user.
* Parameters :
*phoneNumber - The new password to be set
* Returns :
*None
*/
void DataStore::setAuthenticatedUserPassword(const std::string& password)
{
    if (!m_currentUser)
    {
        return;
    }
    m_currentUser->setPassword(password);
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    SharedUser* sharedUser = usersFile->findById(m_currentUser->getUserId().c_str());
    if (sharedUser)
    {
        strncpy_s(sharedUser->password, password.c_str(), sizeof(sharedUser->password));
    }
    usersFile->flush();
    return;
}

/*
 * Function: getScreenCount
 * Description: Retrieves the total number of records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of records
 */
int DataStore::getScreenCount() const
{
    int count = m_registry.getScreenCount();
    return count;
}

/*
 * Function: getSeatCount
 * Description: Retrieves the total number of records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of records
 */
int DataStore::getSeatCount() const
{
    int count = m_registry.getSeatCount();
    return count;
}

/*
 * Function: getPaymentCount
 * Description: Retrieves the total number of records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of records
 */
int DataStore::getPaymentCount() const
{
    int count = m_registry.getPaymentCount();
    return count;
}

/*
 * Function: getRefundCount
 * Description: Retrieves the total number of records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of records
 */
int DataStore::getRefundCount() const
{
    int count = m_registry.getRefundCount();
    return count;
}

/*
 * Function: getShowCount
 * Description: Retrieves the total number of records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of records
 */
int DataStore::getShowCount() const
{
    int count = m_registry.getShowCount();
    return count;
}

/*
 * Function: getShowSeatAvailabilityCount
 * Description: Retrieves the total number of records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of records
 */
int DataStore::getShowSeatAvailabilityCount() const
{
    int count = m_registry.getShowSeatAvailabilityCount();
    return count;
}

/*
 * Function: updateUserStatus
 * Description: Updates the status of a user in the mapped users file.
 * Parameters:
 *    userId - Identifier of the user
 *    status - New user status (must not be NOT_FOUND)
 * Returns:
 *    ProcessStatus::SUCCESS if update applied, FAILED otherwise
 */
Enums::ProcessStatus DataStore::updateUserStatus(const std::string& userId, Enums::UserStatus status)
{
    if (status == Enums::UserStatus::NOT_FOUND)
    {
        return Enums::ProcessStatus::FAILED;
    }
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    SharedUser* sharedUser = usersFile->findById(userId.c_str());
    sharedUser->status = static_cast<int>(status);
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: updateScreenName
 * Description: Updates the name of a screen in the mapped screens file.
 *              Locates the screen record by ID, modifies its name, and flushes
 *              changes to shared memory.
 * Parameters:
 *    screenId   - Identifier of the screen to update
 *    screenName - New name to be applied to the screen
 * Returns:
 *    ProcessStatus::SUCCESS if update applied successfully,
 *    ProcessStatus::FAILED if the screen or file could not be found
 */
Enums::ProcessStatus DataStore::updateScreenName(const std::string& screenId, const std::string& screenName)
{
    MappedFile<SharedScreen>* screensFile = m_registry.getScreens();
    if (!screensFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedScreen* sharedScreen = screensFile->findById(screenId.c_str());
    if (!sharedScreen)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strcpy_s(sharedScreen->name, sizeof(sharedScreen->name), screenName.c_str());
    screensFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: updateScreenStatus
 * Description: Updates the status of a screen in the mapped screens file.
 *              Locates the screen record by ID, modifies its status, and flushes
 *              changes to shared memory.
 * Parameters:
 *    screenId - Identifier of the screen to update
 *    status   - New screen status to be applied
 * Returns:
 *    ProcessStatus::SUCCESS if update applied successfully,
 *    ProcessStatus::FAILED if the screen or file could not be found
 */
Enums::ProcessStatus DataStore::updateScreenStatus(const std::string& screenId, Enums::ScreenStatus status)
{
    MappedFile<SharedScreen>* screensFile = m_registry.getScreens();
    if (!screensFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedScreen* sharedScreen = screensFile->findById(screenId.c_str());
    if (!sharedScreen)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedScreen->status = static_cast<int>(status);
    screensFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: updateSeatStatus
 * Description: Updates the status of a seat in the mapped seats file.
 *              Locates the seat record by ID, modifies its status, and flushes
 *              changes to shared memory.
 * Parameters:
 *    seatId - Identifier of the seat to update
 *    status   - New seat status to be applied
 * Returns:
 *    ProcessStatus::SUCCESS if update applied successfully,
 *    ProcessStatus::FAILED if the seat or file could not be found
 */
Enums::ProcessStatus DataStore::updateSeatStatus(const std::string& seatId, Enums::SeatStatus status)
{
    MappedFile<SharedSeat>* seatsFile = m_registry.getSeats();
    if (!seatsFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedSeat* sharedSeat = seatsFile->findById(seatId.c_str());
    if (!sharedSeat)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedSeat->status = static_cast<int>(status);
    seatsFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: getTicketCount
 * Description: Retrieves the total number of ticket records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of ticket records
 */
int DataStore::getTicketCount() const
{
    int count = m_registry.getTicketCount();
    return count;
}

/*
 * Function: getPaymentById
 * Description: Retrieves a Payment object from the mapped payments file by its unique ID.
 *              Locates the corresponding SharedPayment record, deserializes it into
 *              a Payment object, and registers it in the internal payment map for quick lookup.
 * Parameters:
 *    paymentId - Unique identifier of the payment to retrieve
 * Returns:
 *    Pointer to the Payment object if found and deserialized successfully,
 *    nullptr if the payment record does not exist or deserialization fails
 */
Payment* DataStore::getPaymentById(const std::string& paymentId)
{
    refreshPaymentById(paymentId);
    rebuildPaymentRelationship(paymentId);
    return findPaymentById(paymentId);
}

/*
 * Function: DataStore::updatePaymentStatus
 * Description: Updates the status of a payment record in persistent storage.
 *              Locates the SharedPayment entry by its unique payment ID, modifies
 *              the status field, and flushes changes to the mapped file to ensure
 *              persistence.
 * Parameters:
 *    paymentId - The unique identifier of the payment whose status is to be updated.
 *    status    - The new status to be applied, represented as an Enums::PaymentStatus value.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the payment status was successfully updated.
 *    Enums::ProcessStatus::FAILED if the mapped file or payment record could not be found.
 */
Enums::ProcessStatus DataStore::updatePaymentStatus(const std::string& paymentId, Enums::PaymentStatus status)
{
    MappedFile<SharedPayment>* paymentFile = m_registry.getPayments();
    if (!paymentFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedPayment* sharedPayment = paymentFile->findById(paymentId.c_str());
    if (!sharedPayment)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedPayment->status = static_cast<int>(status);
    paymentFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateRefundStatus
 * Description: Updates the status of a refund record in persistent storage.
 *              Locates the SharedRefund entry by its unique refund ID, modifies
 *              the status field, and flushes changes to the mapped file to ensure
 *              persistence.
 * Parameters:
 *    refundId - The unique identifier of the refund whose status is to be updated.
 *    status   - The new status to be applied, represented as an Enums::RefundStatus value.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the refund status was successfully updated.
 *    Enums::ProcessStatus::FAILED if the mapped file or refund record could not be found.
 */
Enums::ProcessStatus DataStore::updateRefundStatus(const std::string& refundId, Enums::RefundStatus status)
{
    MappedFile<SharedRefund>* refundFile = m_registry.getRefunds();
    if (!refundFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedRefund* sharedRefund = refundFile->findById(refundId.c_str());
    if (!sharedRefund)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedRefund->status = static_cast<int>(status);
    refundFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateShowStatus
 * Description: Updates the status of a Show record in the mapped file registry.
 *              Locates the SharedShow record by its unique identifier, modifies
 *              the status field, and flushes the changes to persistent storage.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 *    status (Enums::ShowStatus)  - New status value to assign to the show
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the show record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateShowStatus(const std::string& showId, Enums::ShowStatus status)
{
    MappedFile<SharedShow>* showsFile = m_registry.getShows();
    if (!showsFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedShow* sharedShow = showsFile->findById(showId.c_str());
    if (!sharedShow)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedShow->status = static_cast<int>(status);
    showsFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::addMovieToTheatre
 * Description: Adds a Movie to a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, copies
 *              the Movie ID into the movieIds array, increments the movieCount,
 *              and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    movieId   (const std::string&) - Unique identifier of the movie to add
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the movie was added and flushed successfully,
 *                           FAILED if the theatre record is not found or the
 *                           mapped file is unavailable
 */
Enums::ProcessStatus DataStore::addMovieToTheatre(const std::string& theatreId, const std::string& movieId)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->movieIds[sharedTheatre->movieCount], movieId.c_str(), sizeof(sharedTheatre->movieIds[0]));
    sharedTheatre->movieCount++;
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateTheatreName
 * Description: Updates the name of a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, copies
 *              the new name into the struct, and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    name      (const std::string&) - New name to assign to the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateTheatreName(const std::string& theatreId, const std::string& name)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->name, name.c_str(), sizeof(sharedTheatre->name));
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateTheatreCity
 * Description: Updates the city field of a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, copies
 *              the new city value into the struct, and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    city      (const std::string&) - New city value to assign to the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateTheatreCity(const std::string& theatreId, const std::string& city)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->city, city.c_str(), sizeof(sharedTheatre->city));
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateTheatreAddress
 * Description: Updates the address field of a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, copies
 *              the new address value into the struct, and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    address   (const std::string&) - New address value to assign to the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateTheatreAddress(const std::string& theatreId, const std::string& address)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->address, address.c_str(), sizeof(sharedTheatre->address));
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateTheatrePhoneNumber
 * Description: Updates the phone number field of a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, copies
 *              the new phone number value into the struct, and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId   (const std::string&) - Unique identifier of the theatre
 *    phoneNumber (const std::string&) - New phone number value to assign to the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateTheatrePhoneNumber(const std::string& theatreId, const std::string& phoneNumber)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->phoneNumber, phoneNumber.c_str(), sizeof(sharedTheatre->phoneNumber));
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateTheatreEmail
 * Description: Updates the email field of a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, copies
 *              the new email value into the struct, and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    email     (const std::string&) - New email value to assign to the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateTheatreEmail(const std::string& theatreId, const std::string& email)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->email, email.c_str(), sizeof(sharedTheatre->email));
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateTheatreStatus
 * Description: Updates the status field of a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, modifies
 *              the status field, and flushes the changes to persistent storage.
 * Parameters:
 *    theatreId (const std::string&)    - Unique identifier of the theatre
 *    status    (Enums::TheatreStatus)  - New status value to assign to the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateTheatreStatus(const std::string& theatreId, Enums::TheatreStatus status)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedTheatre->status = static_cast<int>(status);
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::clearMoviesFromTheatre
 * Description: Removes all movies linked to a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, clears the
 *              movieIds array, resets movieCount to zero, and flushes the changes.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the theatre record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::clearMoviesFromTheatre(const std::string& theatreId)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    memset(sharedTheatre->movieIds, 0, sizeof(sharedTheatre->movieIds));
    sharedTheatre->movieCount = 0;
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::removeMovieFromTheatre
 * Description: Removes a Movie from a Theatre record in the mapped file registry.
 *              Locates the SharedTheatre record by its unique identifier, searches
 *              for the given Movie ID in the movieIds array, shifts remaining entries
 *              to close the gap, decrements movieCount, and flushes the changes.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    movieId   (const std::string&) - Unique identifier of the movie to remove
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the movie was removed and flushed successfully,
 *                           FAILED if the theatre record is not found, the movie
 *                           is not present, or the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::removeMovieFromTheatre(const std::string& theatreId, const std::string& movieId)
{
    MappedFile<SharedTheatre>* theatresFile = m_registry.getTheatres();
    if (!theatresFile)
    {
        return Enums::ProcessStatus::FAILED;
    }

    SharedTheatre* sharedTheatre = theatresFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    int index = -1;
    for (int movieIndex = 0; movieIndex < sharedTheatre->movieCount; ++movieIndex)
    {
        if (strcmp(sharedTheatre->movieIds[movieIndex], movieId.c_str()) == 0)
        {
            index = movieIndex;
            break;
        }
    }
    if (index == -1)
    {
        return Enums::ProcessStatus::FAILED;
    }
    for (int movieIndex = index; movieIndex < sharedTheatre->movieCount - 1; ++movieIndex)
    {
        strncpy_s(sharedTheatre->movieIds[movieIndex], sharedTheatre->movieIds[movieIndex + 1], sizeof(sharedTheatre->movieIds[0]));
    }
    memset(sharedTheatre->movieIds[sharedTheatre->movieCount - 1], 0, sizeof(sharedTheatre->movieIds[0]));
    sharedTheatre->movieCount--;
    theatresFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateShowTime
 * Description: Updates the start and end time fields of a Show record in the
 *              mapped file registry. Locates the SharedShow record by its unique
 *              identifier, serializes the provided time values into string format,
 *              copies them into the struct, and flushes the changes to persistent storage.
 * Parameters:
 *    showId   (const std::string&) - Unique identifier of the show
 *    startTime (const time_t&)     - New start time to assign to the show
 *    endTime   (const time_t&)     - New end time to assign to the show
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the show record is not found or
 *                           the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateShowTime(const std::string& showId, const time_t& startTime, const time_t& endTime)
{
    MappedFile<SharedShow>* showsFile = m_registry.getShows();
    if (!showsFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedShow* sharedShow = showsFile->findById(showId.c_str());
    if (!sharedShow)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedShow->startTime, util::serializeTime(startTime).c_str(), sizeof(sharedShow->startTime));
    strncpy_s(sharedShow->endTime, util::serializeTime(endTime).c_str(), sizeof(sharedShow->endTime));
    showsFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::addScreenToTheatre
 * Description: Associates a screen with a theatre by appending the screen ID
 *              to the SharedTheatre record in the mapped file. Updates the
 *              screen count and ensures the maximum limit is not exceeded.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 *    screenId  (const std::string&) - Unique identifier of the screen
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the screen is added,
 *                           FAILED if the theatre record is not found,
 *                           the mapped file is unavailable,
 *                           or the screen count exceeds the maximum limit
 */
Enums::ProcessStatus DataStore::addScreenToTheatre(const std::string& theatreId, const std::string& screenId)
{
    MappedFile<SharedTheatre>* theatreFile = m_registry.getTheatres();
    if (!theatreFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedTheatre* sharedTheatre = theatreFile->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (sharedTheatre->screenCount >= config::Limit::SCREEN_MAX_COUNT)
    {
        return Enums::ProcessStatus::FAILED;
    }
    strncpy_s(sharedTheatre->screenIds[sharedTheatre->screenCount], sizeof(sharedTheatre->screenIds[0]), screenId.c_str(), _TRUNCATE);
    sharedTheatre->screenCount++;
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: DataStore::updateSeatStatusInAvailability
 * Description: Updates the booking status of a specific seat in a ShowSeatAvailability record.
 *              Locates the SharedShowSeatAvailability struct by its availability ID, searches
 *              for the matching seatId in the seatIds array, and updates its corresponding
 *              seatStatuses entry. Flushes the changes to persistent storage.
 * Parameters:
 *    availabilityId (const std::string&) - Unique identifier of the ShowSeatAvailability record
 *    seatId         (const std::string&) - Unique identifier of the seat to update
 *    status         (Enums::BookingStatus) - New booking status to assign to the seat
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the update and flush succeed,
 *                           FAILED if the record or seat is not found,
 *                           or the mapped file is unavailable
 */
Enums::ProcessStatus DataStore::updateShowSeatAvailabilityStatus(const std::string& availabilityId, const std::string& seatId, Enums::BookingStatus status)
{
    MappedFile<SharedShowSeatAvailability>* availabilityFile = m_registry.getAvailability();
    if (!availabilityFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedShowSeatAvailability* sharedAvailability = availabilityFile->findById(availabilityId.c_str());
    if (!sharedAvailability)
    {
        return Enums::ProcessStatus::FAILED;
    }
    for (int index = 0; index < sharedAvailability->seatCount; ++index)
    {
        if (seatId == sharedAvailability->seatIds[index])
        {
            sharedAvailability->seatStatuses[index] = static_cast<int>(status);
            availabilityFile->flush();
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: DataStore::getTheatreIdFromScreen
 * Description: Retrieves the theatre ID associated with a given screen ID
 *              by looking up the SharedScreen record in the mapped file.
 * Parameters:
 *    screenId (const std::string&) - Unique identifier of the screen
 * Returns:
 *    const std::string - Theatre ID if found, empty string otherwise
 */
const std::string DataStore::getTheatreIdFromScreen(const std::string& screenId)
{
    std::string theatreId = "";
    MappedFile<SharedScreen>* screensFile = m_registry.getScreens();
    if(!screensFile)
    {
        return theatreId;
    }
    SharedScreen* sharedScreen = screensFile->findById(screenId.c_str());
    if(sharedScreen)
    {
        theatreId = sharedScreen->theatreId;
    }
    return theatreId;
}

/*
 * Function: DataStore::isUserLoggedIn
 * Description: Checks whether the specified user currently has an active
 *              session in the shared session manager. Access is synchronized
 *              using the session mutex to ensure thread safety.
 * Parameters:
 *    userId - Unique identifier of the user to be checked.
 * Returns:
 *    true if the user is currently logged in,
 *    false otherwise.
 */
bool DataStore::isUserLoggedIn(const std::string& userId)
{
    ScopedLock lock(m_sessionMutex);
    return m_sessionManager.isLoggedIn(userId);
}

/*
 * Function: DataStore::addLoggedInUser
 * Description: Adds the specified user to the shared session manager,
 *              marking the user as currently logged in. Access is
 *              synchronized using the session mutex.
 * Parameters:
 *    userId - Unique identifier of the user to be added to the active sessions.
 * Returns:
 *    true if the user session was added successfully,
 *    false otherwise.
 */
bool DataStore::addLoggedInUser(const std::string& userId)
{
    ScopedLock lock(m_sessionMutex);
    return m_sessionManager.addSession(userId);
}

/*
 * Function: DataStore::removeLoggedInUser
 * Description: Removes the specified user from the shared session manager,
 *              marking the user as logged out. Access is synchronized
 *              using the session mutex.
 * Parameters:
 *    userId - Unique identifier of the user to be removed from the active sessions.
 * Returns:
 *    true if the user session was removed successfully, 
 *    false otherwise.
 */
bool DataStore::removeLoggedInUser(const std::string& userId)
{
    ScopedLock lock(m_sessionMutex);
    return m_sessionManager.removeSession(userId);
}

/*
 * Function: updateBookingStatus
 * Description: Updates the status of a booking in the mapped bookings file.
 *              Locates the booking record by its unique ID, modifies the status.
 * Parameters:
 *    bookingId - Identifier of the booking to update
 *    status    - New booking status to be applied (Enums::BookingStatus)
 * Returns:
 *    ProcessStatus::SUCCESS if the update was applied successfully,
 *    ProcessStatus::FAILED if the booking or file could not be found
 */
Enums::ProcessStatus DataStore::updateBookingStatus(const std::string& bookingId, Enums::BookingStatus status)
{
    MappedFile<SharedBooking>* bookingFile = m_registry.getBookings();
    if (!bookingFile)
    {
        return Enums::ProcessStatus::FAILED;
    }
    SharedBooking* sharedBooking = bookingFile->findById(bookingId.c_str());
    if (!sharedBooking)
    {
        return Enums::ProcessStatus::FAILED;
    }
    sharedBooking->status = static_cast<int>(status);
    bookingFile->flush();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: getBookingCount
 * Description: Retrieves the total number of booking records managed by the registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of booking records
 */
int DataStore::getBookingCount() const
{
    int count = m_registry.getBookingCount();
    return count;
}

/*
* Function: DataStore::getShowSeatAvailabilityById
* Description: Retrieves a ShowSeatAvailability object by its unique identifier
*              from the mapped file. Deserializes the corresponding
*              SharedShowSeatAvailability record, rebuilds the seat availability
*              map, and caches the object in the datastore. If an existing entry
*              is present, it is deleted and replaced with the new one.
* Parameters:
*    seatAvailabilityId (const std::string&) - Unique identifier of the seat availability record
* Returns:
*    ShowSeatAvailability* - Pointer to the ShowSeatAvailability object if found and
*                            deserialized successfully, nullptr otherwise
*/
ShowSeatAvailability* DataStore::getShowSeatAvailabilityById(const std::string& seatAvailabilityId)
{
    refreshShowSeatAvailabilityById(seatAvailabilityId);
    rebuildShowSeatAvailabilityRelationship(seatAvailabilityId);
    return findShowSeatAvailabilityById(seatAvailabilityId);
}

/*
 * Function: DataStore::isCurrentUserStillActive
 * Description: Checks whether the currently authenticated user is still marked
 *              as ACTIVE in the mapped users file. Returns false if no user is
 *              authenticated or if the user record is missing.
 * Parameters:
 *    None
 * Returns:
 *    true if the current user exists and has ACTIVE status,
 *    false otherwise
 */
bool DataStore::isCurrentUserStillActive()
{
    if (!m_currentUser)
    {
        return false;
    }
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    if (!usersFile)
    {
        return true;
    }
    SharedUser* sharedUser = usersFile->findById(m_currentUser->getUserId().c_str());
    if (!sharedUser)
    {
        return false;
    }
    return static_cast<Enums::UserStatus>(sharedUser->status) == Enums::UserStatus::ACTIVE;
}

/*
 * Function: DataStore::refreshUsers
 * Description: Synchronizes all User objects in the in-memory cache with the
 *              latest SharedUser records from the mapped file. Existing objects
 *              are updated in place and missing objects are created and added
 *              to the internal users map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshUsers()
{
    int count = 0;
    SharedUser* sharedUsers = m_registry.getUsers()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedUser& sharedUser = sharedUsers[index];
        std::map<std::string, User*>::iterator iterator = m_users.find(sharedUser.userId);
        if (iterator != m_users.end())
        {
            User* user = iterator->second;
            user->setUserName(sharedUser.username);
            user->setEmail(sharedUser.email);
            std::string password = sharedUser.password;
            user->setPassword(User::decryption(password));
            user->setPhoneNumber(sharedUser.phoneNumber);
            user->setStatus(static_cast<Enums::UserStatus>(sharedUser.status));
        }
        else
        {
            std::string password = sharedUser.password;
            User* user =
                Factory::getObject<User>(
                    sharedUser.userId,
                    sharedUser.username,
                    sharedUser.email,
                    User::decryption(password),
                    sharedUser.phoneNumber,
                    static_cast<Enums::UserType>(
                        sharedUser.userType));

            if (user)
            {
                user->setStatus(static_cast<Enums::UserStatus>(sharedUser.status));
                m_users[user->getUserId()] = user;
            }
        }
    }
}

/*
 * Function: DataStore::refreshTheatres
 * Description: Synchronizes all Theatre objects in the in-memory cache with the
 *              latest SharedTheatre records from the mapped file. Existing
 *              objects are updated and missing objects are created and inserted
 *              into the internal theatres map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshTheatres()
{
    int count = 0;
    SharedTheatre* sharedTheatres = m_registry.getTheatres()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedTheatre& sharedTheatre = sharedTheatres[index];
        std::map<std::string, Theatre*>::iterator iterator = m_theatres.find(sharedTheatre.theatreId);
        if (iterator != m_theatres.end())
        {
            Theatre* theatre = iterator->second;
            theatre->setName(sharedTheatre.name);
            theatre->setCity(sharedTheatre.city);
            theatre->setAddress(sharedTheatre.address);
            theatre->setTheatrePhoneNumber(sharedTheatre.phoneNumber);
            theatre->setTheatreEmail(sharedTheatre.email);
            theatre->setStatus(static_cast<Enums::TheatreStatus>(sharedTheatre.status));
        }
        else
        {
            Theatre* theatre =
                Factory::getObject<Theatre>(
                    sharedTheatre.theatreId,
                    sharedTheatre.name,
                    sharedTheatre.city,
                    sharedTheatre.address,
                    sharedTheatre.phoneNumber,
                    sharedTheatre.email,
                    nullptr);
            if (theatre)
            {
                theatre->setStatus(static_cast<Enums::TheatreStatus>(sharedTheatre.status));
                m_theatres[theatre->getTheatreId()] = theatre;
            }
        }
    }
}

/*
 * Function: DataStore::refreshScreens
 * Description: Synchronizes all Screen objects in the in-memory cache with the
 *              latest SharedScreen records from the mapped file. Updates
 *              existing objects and creates missing ones.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshScreens()
{
    int count = 0;
    SharedScreen* sharedScreens =
        m_registry.getScreens()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedScreen& sharedScreen = sharedScreens[index];
        std::map<std::string, Screen*>::iterator iterator = m_screens.find(sharedScreen.screenId);
        if (iterator != m_screens.end())
        {
            Screen* screen = iterator->second;
            screen->setName(sharedScreen.name);
            screen->setTotalRows(sharedScreen.totalRows);
            screen->setTotalColumns(sharedScreen.totalColumns);
            screen->setScreenStatus(static_cast<Enums::ScreenStatus>(sharedScreen.status));
        }
        else
        {
            std::vector<std::vector<Seat*>> emptyGrid;
            Screen* screen =
                Factory::getObject<Screen>(
                    sharedScreen.screenId,
                    nullptr,
                    sharedScreen.name,
                    sharedScreen.totalRows,
                    sharedScreen.totalColumns,
                    emptyGrid);
            if (screen)
            {
                screen->setScreenStatus(static_cast<Enums::ScreenStatus>(sharedScreen.status));
                m_screens[screen->getScreenId()] = screen;
            }
        }
    }
}

/*
 * Function: DataStore::refreshMovies
 * Description: Synchronizes all Movie objects in the in-memory cache with the
 *              latest SharedMovie records from the mapped file. Existing movie
 *              objects are updated and new movies are constructed when required.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshMovies()
{
    int count = 0;
    SharedMovie* sharedMovies = m_registry.getMovies()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedMovie& sharedMovie = sharedMovies[index];
        std::map<std::string, Movie*>::iterator iterator = m_movies.find(sharedMovie.movieId);
        if (iterator != m_movies.end())
        {
            Movie* movie = iterator->second;
            movie->setTitle(sharedMovie.title);
            movie->setGenre(sharedMovie.genre);
            movie->setLanguage(sharedMovie.language);
            movie->setDuration(sharedMovie.duration);
            movie->setStatus(static_cast<Enums::MovieStatus>(sharedMovie.status));
        }
        else
        {
            Movie* movie =
                Factory::getObject<Movie>(
                    sharedMovie.movieId,
                    sharedMovie.title,
                    sharedMovie.language,
                    sharedMovie.genre,
                    sharedMovie.duration);

            if (movie)
            {
                movie->setStatus(static_cast<Enums::MovieStatus>(sharedMovie.status));
                m_movies[movie->getMovieId()] = movie;
            }
        }
    }
}

/*
 * Function: DataStore::refreshShows
 * Description: Synchronizes all Show objects in the in-memory cache with the
 *              latest SharedShow records from the mapped file. Existing objects
 *              are updated in place and missing objects are reconstructed and
 *              inserted into the internal shows map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshShows()
{
    int count = 0;
    SharedShow* sharedShows = m_registry.getShows()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedShow& sharedShow = sharedShows[index];
        std::map<std::string, Show*>::iterator iterator = m_shows.find(sharedShow.showId);
        if (iterator != m_shows.end())
        {
            Show* show = iterator->second;
            show->setStartTime(util::deserializeTime(sharedShow.startTime));
            show->setEndTime(util::deserializeTime(sharedShow.endTime));
            show->setShowStatus(static_cast<Enums::ShowStatus>(sharedShow.status));
        }
        else
        {
            Show* show =
                Factory::getObject<Show>(
                    sharedShow.showId,
                    nullptr,
                    nullptr,
                    util::deserializeTime(sharedShow.startTime),
                    util::deserializeTime(sharedShow.endTime),
                    nullptr);
            if (show)
            {
                show->setShowStatus(static_cast<Enums::ShowStatus>(sharedShow.status));
                m_shows[show->getShowId()] = show;
            }
        }
    }
}

/*
 * Function: DataStore::refreshSeats
 * Description: Synchronizes all Seat objects in the in-memory cache with the
 *              latest SharedSeat records from the mapped file. Existing seats
 *              are updated and missing seats are reconstructed and inserted
 *              into the internal seats map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshSeats()
{
    int count = 0;
    SharedSeat* sharedSeats = m_registry.getSeats()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedSeat& sharedSeat = sharedSeats[index];
        std::map<std::string, Seat*>::iterator iterator = m_seats.find(sharedSeat.seatId);
        if (iterator != m_seats.end())
        {
            Seat* seat = iterator->second;
            seat->setSeatRow(static_cast<char>(sharedSeat.seatRow));
            seat->setSeatColumn(sharedSeat.seatColumn);
            seat->setSeatAmount(sharedSeat.amount);
            seat->setSeatStatus(static_cast<Enums::SeatStatus>(sharedSeat.status));
        }
        else
        {
            Seat* seat =
                Factory::getObject<Seat>(
                    sharedSeat.seatId,
                    nullptr,
                    static_cast<char>(sharedSeat.seatRow),
                    sharedSeat.seatColumn,
                    sharedSeat.amount,
                    static_cast<Enums::SeatStatus>(sharedSeat.status));
            if (seat)
            {
                m_seats[seat->getSeatId()] = seat;
            }
        }
    }
}

/*
 * Function: DataStore::refreshBookings
 * Description: Synchronizes all Booking objects in the in-memory cache with the
 *              latest SharedBooking records from the mapped file. Existing
 *              bookings are updated and missing bookings are reconstructed and
 *              inserted into the internal bookings map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshBookings()
{
    int count = 0;

    SharedBooking* sharedBookings = m_registry.getBookings()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedBooking& sharedBooking =sharedBookings[index];
        std::map<std::string, Booking*>::iterator iterator = m_bookings.find(sharedBooking.bookingId);
        if (iterator != m_bookings.end())
        {
            Booking* booking = iterator->second;
            booking->setStatus(static_cast<Enums::BookingStatus>(sharedBooking.status));
            booking->setAmount(sharedBooking.amount);
        }
        else
        {
            std::vector<Seat*> seats;
            Booking* booking =
                Factory::getObject<Booking>(
                    sharedBooking.bookingId,
                    nullptr,
                    nullptr,
                    seats,
                    static_cast<Enums::BookingStatus>(sharedBooking.status), sharedBooking.amount);
            if (booking)
            {
                m_bookings[booking->getBookingId()] = booking;
            }
        }
    }
}

/*
 * Function: DataStore::refreshPayments
 * Description: Synchronizes all Payment objects in the in-memory cache with the
 *              latest SharedPayment records from the mapped file. Existing
 *              payments are updated and missing payments are reconstructed and
 *              inserted into the internal payments map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshPayments()
{
    int count = 0;
    SharedPayment* sharedPayments = m_registry.getPayments()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedPayment& sharedPayment = sharedPayments[index];
        std::map<std::string, Payment*>::iterator iterator = m_payments.find(sharedPayment.paymentId);
        if (iterator != m_payments.end())
        {
            Payment* payment = iterator->second;
            payment->setAmount(sharedPayment.amount);
            payment->setPaymentMethod(static_cast<Enums::PaymentMethod>(sharedPayment.paymentMethod));
            payment->setStatus(static_cast<Enums::PaymentStatus>(sharedPayment.status));
            payment->setTimeStamp(util::deserializeTime(sharedPayment.time));
        }
        else
        {
            Payment* payment =
                Factory::getObject<Payment>(
                    sharedPayment.paymentId,
                    nullptr,
                    sharedPayment.amount,
                    static_cast<Enums::PaymentMethod>(sharedPayment.paymentMethod),
                    util::deserializeTime(sharedPayment.time));
            if (payment)
            {
                payment->setStatus(static_cast<Enums::PaymentStatus>(sharedPayment.status));
                m_payments[payment->getPaymentId()] = payment;
            }
        }
    }
}

/*
 * Function: DataStore::refreshTickets
 * Description: Synchronizes all Ticket objects in the in-memory cache with the
 *              latest SharedTicket records from the mapped file. Existing
 *              tickets are updated and missing tickets are reconstructed and
 *              inserted into the internal tickets map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshTickets()
{
    int count = 0;
    SharedTicket* sharedTickets = m_registry.getTickets()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedTicket& sharedTicket = sharedTickets[index];
        std::map<std::string, Ticket*>::iterator iterator = m_tickets.find(sharedTicket.ticketId);
        if (iterator != m_tickets.end())
        {
            Ticket* ticket = iterator->second;
            ticket->setTicketStatus(static_cast<Enums::TicketStatus>(sharedTicket.status));
        }
        else
        {
            Ticket* ticket =
                Factory::getObject<Ticket>(
                    sharedTicket.ticketId,
                    nullptr,
                    nullptr);
            if (ticket)
            {
                ticket->setTicketStatus(static_cast<Enums::TicketStatus>(sharedTicket.status));
                m_tickets[ticket->getTicketId()] = ticket;
            }
        }
    }
}

/*
 * Function: DataStore::refreshRefunds
 * Description: Synchronizes all Refund objects in the in-memory cache with the
 *              latest SharedRefund records from the mapped file. Existing
 *              refunds are updated and missing refunds are reconstructed and
 *              inserted into the internal refunds map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshRefunds()
{
    int count = 0;
    SharedRefund* sharedRefunds = m_registry.getRefunds()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedRefund& sharedRefund = sharedRefunds[index];
        std::map<std::string, Refund*>::iterator iterator = m_refunds.find(sharedRefund.refundId);
        if (iterator != m_refunds.end())
        {
            Refund* refund = iterator->second;
            refund->setRefundAmount(sharedRefund.amount);
            refund->setStatus(static_cast<Enums::RefundStatus>(sharedRefund.status));
            refund->setTime(util::deserializeTime(sharedRefund.time));
        }
        else
        {
            Refund* refund =
                Factory::getObject<Refund>(
                    sharedRefund.refundId,
                    nullptr,
                    sharedRefund.amount,
                    util::deserializeTime(sharedRefund.time));
            if (refund)
            {
                refund->setStatus(static_cast<Enums::RefundStatus>(sharedRefund.status));
                m_refunds[refund->getRefundId()] = refund;
            }
        }
    }
}

/*
 * Function: DataStore::refreshNotifications
 * Description: Synchronizes all Notification objects in the in-memory cache
 *              with the latest SharedNotification records from the mapped file.
 *              Existing objects are updated and missing objects are
 *              reconstructed and inserted into the internal notifications map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshNotifications()
{
    int count = 0;
    SharedNotification* sharedNotifications = m_registry.getNotifications()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedNotification& sharedNotification = sharedNotifications[index];
        std::map<std::string, Notification*>::iterator iterator = m_notifications.find(sharedNotification.notificationId);
        if (iterator != m_notifications.end())
        {
            Notification* notification = iterator->second;
            notification->setMessage(sharedNotification.message);
            notification->setStatus(static_cast<Enums::NotificationStatus>(sharedNotification.status));
            notification->setTime(util::deserializeTime(sharedNotification.time));
        }
        else
        {
            Notification* notification =
                Factory::getObject<Notification>(
                    sharedNotification.notificationId,
                    nullptr,
                    sharedNotification.message,
                    util::deserializeTime(sharedNotification.time));
            if (notification)
            {
                notification->setStatus(static_cast<Enums::NotificationStatus>(sharedNotification.status));
                m_notifications[notification->getNotificationId()] = notification;
            }
        }
    }
}

/*
 * Function: DataStore::refreshLogs
 * Description: Synchronizes all Log objects in the in-memory cache with the
 *              latest SharedLog records from the mapped file. Existing logs
 *              are updated and missing logs are reconstructed and inserted
 *              into the internal logs map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshLogs()
{
    int count = 0;
    SharedLog* sharedLogs = m_registry.getLogs()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedLog& sharedLog = sharedLogs[index];
        std::map<std::string, Log*>::iterator iterator = m_logs.find(sharedLog.logId);
        if (iterator != m_logs.end())
        {
            Log* log = iterator->second;
            log->setDescription(sharedLog.description);
            log->setLogType(static_cast<Enums::LogType>(sharedLog.type));
            log->setTimestamp(util::Timestamp::fromString(sharedLog.time));
        }
        else
        {
            Log* log =
                Factory::getObject<Log>(
                    sharedLog.logId,
                    sharedLog.description,
                    static_cast<Enums::LogType>(sharedLog.type));
            if (log)
            {
                log->setTimestamp(util::Timestamp::fromString(sharedLog.time));
                m_logs[log->getLogId()] = log;
            }
        }
    }
}

/*
 * Function: DataStore::refreshShowSeatAvailabilitys
 * Description: Synchronizes all ShowSeatAvailability objects in the in-memory
 *              cache with the latest SharedShowSeatAvailability records from
 *              the mapped file. Existing objects are updated and missing
 *              objects are reconstructed and inserted into the internal map.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::refreshShowSeatAvailabilitys()
{
    int count = 0;
    SharedShowSeatAvailability* sharedAvailabilities = m_registry.getAvailability()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        SharedShowSeatAvailability& sharedAvailability = sharedAvailabilities[index];
        std::map<std::string, ShowSeatAvailability*>::iterator iterator = m_showSeatAvailabilitys.find(sharedAvailability.availabilityId);

        std::map<std::string, Enums::BookingStatus> seatMap;
        for (int seatIndex = 0; seatIndex < sharedAvailability.seatCount; ++seatIndex)
        {
            seatMap[sharedAvailability.seatIds[seatIndex]] = static_cast<Enums::BookingStatus>(sharedAvailability.seatStatuses[seatIndex]);
        }
        if (iterator != m_showSeatAvailabilitys.end())
        {
            ShowSeatAvailability* availability = iterator->second;
            availability->setSeatAvailabilityMap(seatMap);
        }
        else
        {
            ShowSeatAvailability* availability =
                Factory::getObject<ShowSeatAvailability>(
                    sharedAvailability.availabilityId,
                    nullptr,
                    seatMap);
            if (availability)
            {
                m_showSeatAvailabilitys[availability->getShowAvailabiltyId()] = availability;
            }
        }
    }
}

/*
 * Function: DataStore::findUserById
 * Description: Retrieves a User object directly from the internal users map
 *              without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    userId - Unique identifier of the user
 * Returns:
 *    User* - Pointer to the User object if found, nullptr otherwise
 */
User* DataStore::findUserById(const std::string& userId)
{
    std::map<std::string, User*>::iterator iterator = m_users.find(userId);
    if (iterator != m_users.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findMovieById
 * Description: Retrieves a Movie object directly from the internal movies map
 *              without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    movieId - Unique identifier of the movie
 * Returns:
 *    Movie* - Pointer to the Movie object if found, nullptr otherwise
 */
Movie* DataStore::findMovieById(const std::string& movieId)
{
    std::map<std::string, Movie*>::iterator iterator = m_movies.find(movieId);
    if (iterator != m_movies.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findTheatreById
 * Description: Retrieves a Theatre object directly from the internal theatres
 *              map without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    theatreId - Unique identifier of the theatre
 * Returns:
 *    Theatre* - Pointer to the Theatre object if found, nullptr otherwise
 */
Theatre* DataStore::findTheatreById(const std::string& theatreId)
{
    std::map<std::string, Theatre*>::iterator iterator = m_theatres.find(theatreId);
    if (iterator != m_theatres.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findScreenById
 * Description: Retrieves a Screen object directly from the internal screens
 *              map without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    screenId - Unique identifier of the screen
 * Returns:
 *    Screen* - Pointer to the Screen object if found, nullptr otherwise
 */
Screen* DataStore::findScreenById(const std::string& screenId)
{
    std::map<std::string, Screen*>::iterator iterator = m_screens.find(screenId);
    if (iterator != m_screens.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findSeatById
 * Description: Retrieves a Seat object directly from the internal seats map
 *              without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    seatId - Unique identifier of the seat
 * Returns:
 *    Seat* - Pointer to the Seat object if found, nullptr otherwise
 */
Seat* DataStore::findSeatById(const std::string& seatId)
{
    std::map<std::string, Seat*>::iterator iterator = m_seats.find(seatId);
    if (iterator != m_seats.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findShowById
 * Description: Retrieves a Show object directly from the internal shows map
 *              without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    showId - Unique identifier of the show
 * Returns:
 *    Show* - Pointer to the Show object if found, nullptr otherwise
 */
Show* DataStore::findShowById(const std::string& showId)
{
    std::map<std::string, Show*>::iterator iterator = m_shows.find(showId);
    if (iterator != m_shows.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findShowSeatAvailabilityById
 * Description: Retrieves a ShowSeatAvailability object directly from the
 *              internal show seat availability map without synchronizing
 *              with shared memory or rebuilding any relationships.
 * Parameters:
 *    availabilityId - Unique identifier of the show seat availability record
 * Returns:
 *    ShowSeatAvailability* - Pointer to the ShowSeatAvailability object if
 *                            found, nullptr otherwise
 */
ShowSeatAvailability* DataStore::findShowSeatAvailabilityById(const std::string& availabilityId)
{
    std::map<std::string, ShowSeatAvailability*>::iterator iterator = m_showSeatAvailabilitys.find(availabilityId);

    if (iterator != m_showSeatAvailabilitys.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findBookingById
 * Description: Retrieves a Booking object directly from the internal bookings
 *              map without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    bookingId - Unique identifier of the booking
 * Returns:
 *    Booking* - Pointer to the Booking object if found, nullptr otherwise
 */
Booking* DataStore::findBookingById(const std::string& bookingId)
{
    std::map<std::string, Booking*>::iterator iterator = m_bookings.find(bookingId);
    if (iterator != m_bookings.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findPaymentById
 * Description: Retrieves a Payment object directly from the internal payments
 *              map without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    paymentId - Unique identifier of the payment
 * Returns:
 *    Payment* - Pointer to the Payment object if found, nullptr otherwise
 */
Payment* DataStore::findPaymentById(const std::string& paymentId)
{
    std::map<std::string, Payment*>::iterator iterator = m_payments.find(paymentId);
    if (iterator != m_payments.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findRefundById
 * Description: Retrieves a Refund object directly from the internal refunds
 *              map without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    refundId - Unique identifier of the refund
 * Returns:
 *    Refund* - Pointer to the Refund object if found, nullptr otherwise
 */
Refund* DataStore::findRefundById(const std::string& refundId)
{
    std::map<std::string, Refund*>::iterator iterator = m_refunds.find(refundId);
    if (iterator != m_refunds.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findTicketById
 * Description: Retrieves a Ticket object directly from the internal tickets
 *              map without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    ticketId - Unique identifier of the ticket
 * Returns:
 *    Ticket* - Pointer to the Ticket object if found, nullptr otherwise
 */
Ticket* DataStore::findTicketById(const std::string& ticketId)
{
    std::map<std::string, Ticket*>::iterator iterator = m_tickets.find(ticketId);
    if (iterator != m_tickets.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findNotificationById
 * Description: Retrieves a Notification object directly from the internal
 *              notifications map without synchronizing with shared memory or
 *              rebuilding any relationships.
 * Parameters:
 *    notificationId - Unique identifier of the notification
 * Returns:
 *    Notification* - Pointer to the Notification object if found,
 *                    nullptr otherwise
 */
Notification* DataStore::findNotificationById(const std::string& notificationId)
{
    std::map<std::string, Notification*>::iterator iterator = m_notifications.find(notificationId);

    if (iterator != m_notifications.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::findLogById
 * Description: Retrieves a Log object directly from the internal logs map
 *              without synchronizing with shared memory or rebuilding any
 *              relationships.
 * Parameters:
 *    logId - Unique identifier of the log
 * Returns:
 *    Log* - Pointer to the Log object if found, nullptr otherwise
 */
Log* DataStore::findLogById(const std::string& logId)
{
    std::map<std::string, Log*>::iterator iterator = m_logs.find(logId);
    if (iterator != m_logs.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: DataStore::rebuildRelationships
 * Description: Reconstructs all inter-object relationships for every entity
 *              stored in memory by restoring references between dependent
 *              objects using the internal datastore maps.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildRelationships()
{
    rebuildTheatreRelationships();
    rebuildScreenRelationships();
    rebuildSeatRelationships();
    rebuildShowSeatAvailabilityRelationships();
    rebuildShowRelationships();
    rebuildBookingRelationships();
    rebuildPaymentRelationships();
    rebuildTicketRelationships();
    rebuildRefundRelationships();
    rebuildNotificationRelationships();
}

/*
 * Function: DataStore::rebuildTheatreRelationships
 * Description: Reconstructs relationships for all Theatre objects by restoring
 *              references to their associated TheatreOwner, Movie, and Screen
 *              objects using the internal datastore maps.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildTheatreRelationships()
{
    int count = 0;
    SharedTheatre* sharedTheatres = m_registry.getTheatres()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildTheatreRelationship(sharedTheatres[index].theatreId);
        SharedTheatre& sharedTheatre = sharedTheatres[index];
        Theatre* theatre = m_theatres[sharedTheatre.theatreId];
        if (!theatre)
        {
            continue;
        }
        theatre->setTheatreOwner(findUserById(sharedTheatre.ownerId));
        theatre->getMoviesForUpdation().clear();
        for (int movieIndex = 0; movieIndex < sharedTheatre.movieCount; ++movieIndex)
        {
            Movie* movie = findMovieById(sharedTheatre.movieIds[movieIndex]);
            if (movie)
            {
                theatre->addMovieToTheatre(movie);
            }
        }
        theatre->getScreensForUpdation().clear();
        for (int screenIndex = 0; screenIndex < sharedTheatre.screenCount; ++screenIndex)
        {
            refreshScreenById(sharedTheatre.screenIds[index]);
            rebuildScreenRelationship(sharedTheatre.screenIds[screenIndex]);
            Screen* screen = findScreenById(sharedTheatre.screenIds[screenIndex]);
            if (screen)
            {
                theatre->setScreen(screen);
            }
        }
    }
}

/*
 * Function: DataStore::rebuildScreenRelationships
 * Description: Reconstructs relationships for all Screen objects by restoring
 *              references to their associated Theatre and Seat objects and
 *              rebuilding their seat grids.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildScreenRelationships()
{
    int count = 0;
    SharedScreen* sharedScreens = m_registry.getScreens()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildScreenRelationship(sharedScreens[index].screenId);
        SharedScreen& sharedScreen = sharedScreens[index];
        Screen* screen = m_screens[sharedScreen.screenId];
        if (!screen)
        {
            continue;
        }
        screen->setTheatre(findTheatreById(sharedScreen.theatreId));
        std::vector<std::vector<Seat*>> seatGrid(screen->getTotalRows());
        for (int seatIndex = 0; seatIndex < sharedScreen.seatCount; ++seatIndex)
        {
            Seat* seat = findSeatById(sharedScreen.seatIds[seatIndex]);
            if (!seat)
            {
                continue;
            }
            int row = seat->getSeatRow() - 'A';
            if (row >= 0 && row < screen->getTotalRows())
            {
                seatGrid[row].push_back(seat);
            }
        }
        screen->setSeatGrid(seatGrid);
    }
}

/*
 * Function: DataStore::rebuildSeatRelationships
 * Description: Reconstructs relationships for all Seat objects by restoring
 *              references to their associated Screen objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildSeatRelationships()
{
    int count = 0;
    SharedSeat* sharedSeats = m_registry.getSeats()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildSeatRelationship(sharedSeats[index].seatId);
        SharedSeat& sharedSeat = sharedSeats[index];
        Seat* seat = m_seats[sharedSeat.seatId];
        if (!seat)
        {
            continue;
        }
        seat->setScreen(findScreenById(sharedSeat.screenId));
    }
}

/*
 * Function: DataStore::rebuildShowSeatAvailabilityRelationships
 * Description: Reconstructs relationships for all ShowSeatAvailability
 *              objects by restoring references to their associated Show
 *              objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildShowSeatAvailabilityRelationships()
{
    int count = 0;
    SharedShowSeatAvailability* sharedAvailabilities = m_registry.getAvailability()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildShowSeatAvailabilityRelationship(sharedAvailabilities[index].availabilityId);
        SharedShowSeatAvailability& sharedAvailability = sharedAvailabilities[index];
        ShowSeatAvailability* availability = m_showSeatAvailabilitys[sharedAvailability.availabilityId];
        if (!availability)
        {
            continue;
        }
        availability->setShow(findShowById(sharedAvailability.showId));
    }
}

/*
 * Function: DataStore::rebuildShowRelationships
 * Description: Reconstructs relationships for all Show objects by restoring
 *              references to their associated Movie, Screen, and
 *              ShowSeatAvailability objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildShowRelationships()
{
    int count = 0;
    SharedShow* sharedShows = m_registry.getShows()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildShowRelationship(sharedShows[index].showId);
        SharedShow& sharedShow = sharedShows[index];
        Show* show = m_shows[sharedShow.showId];
        if (!show)
        {
            continue;
        }
        show->setMovie(findMovieById(sharedShow.movieId));
        show->setScreen(findScreenById(sharedShow.screenId));
        show->setSeatAvailability(findShowSeatAvailabilityById(sharedShow.seatAvailabilityId));
    }
}

/*
 * Function: DataStore::rebuildBookingRelationships
 * Description: Reconstructs relationships for all Booking objects by restoring
 *              references to their associated Customer, Show, and booked Seat
 *              objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildBookingRelationships()
{
    int count = 0;
    SharedBooking* sharedBookings = m_registry.getBookings()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildBookingRelationship(sharedBookings[index].bookingId);
        SharedBooking& sharedBooking = sharedBookings[index];
        Booking* booking = m_bookings[sharedBooking.bookingId];
        if (!booking)
        {
            continue;
        }
        booking->setCustomer(findUserById(sharedBooking.customerId));
        booking->setShow(findShowById(sharedBooking.showId));
        std::vector<Seat*> bookedSeats;
        for (int seatIndex = 0; seatIndex < sharedBooking.seatCount; ++seatIndex)
        {
            Seat* seat = findSeatById(sharedBooking.seatIds[seatIndex]);
            if (seat)
            {
                bookedSeats.push_back(seat);
            }
        }
        booking->setBookedSeats(bookedSeats);
    }
}

/*
 * Function: DataStore::rebuildPaymentRelationships
 * Description: Reconstructs relationships for all Payment objects by restoring
 *              references to their associated Booking objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildPaymentRelationships()
{
    int count = 0;
    SharedPayment* sharedPayments = m_registry.getPayments()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildPaymentRelationship(sharedPayments[index].paymentId);
        SharedPayment& sharedPayment = sharedPayments[index];
        Payment* payment = m_payments[sharedPayment.paymentId];
        if (!payment)
        {
            continue;
        }
        payment->setBooking(findBookingById(sharedPayment.bookingId));
    }
}

/*
 * Function: DataStore::rebuildTicketRelationships
 * Description: Reconstructs relationships for all Ticket objects by restoring
 *              references to their associated Payment and Customer objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildTicketRelationships()
{
    int count = 0;
    SharedTicket* sharedTickets = m_registry.getTickets()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildTicketRelationship(sharedTickets[index].ticketId);
        SharedTicket& sharedTicket = sharedTickets[index];
        Ticket* ticket = m_tickets[sharedTicket.ticketId];
        if (!ticket)
        {
            continue;
        }
        ticket->setPayment(findPaymentById(sharedTicket.paymentId));
        ticket->setCustomer(findUserById(sharedTicket.customerId));
    }
}

/*
 * Function: DataStore::rebuildRefundRelationships
 * Description: Reconstructs relationships for all Refund objects by restoring
 *              references to their associated Ticket objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildRefundRelationships()
{
    int count = 0;
    SharedRefund* sharedRefunds = m_registry.getRefunds()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildRefundRelationship(sharedRefunds[index].refundId);
        SharedRefund& sharedRefund = sharedRefunds[index];
        Refund* refund = m_refunds[sharedRefund.refundId];
        if (!refund)
        {
            continue;
        }
        refund->setBookedTicket(findTicketById(sharedRefund.ticketId));
    }
}

/*
 * Function: DataStore::rebuildNotificationRelationships
 * Description: Reconstructs relationships for all Notification objects by
 *              restoring references to their associated receiver User objects.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void DataStore::rebuildNotificationRelationships()
{
    int count = 0;
    SharedNotification* sharedNotifications = m_registry.getNotifications()->getAllRecords(count);
    for (int index = 0; index < count; ++index)
    {
        rebuildNotificationRelationship(sharedNotifications[index].notificationId);
        SharedNotification& sharedNotification = sharedNotifications[index];
        Notification* notification = m_notifications[sharedNotification.notificationId];
        if (!notification)
        {
            continue;
        }
        notification->setReceiver(findUserById(sharedNotification.userId));
    }
}

/*
 * Function: DataStore::refreshUserById
 * Description: Synchronizes a single User object in the in-memory cache with
 *              the latest SharedUser record stored in the mapped file. Updates
 *              an existing object in place or reconstructs and inserts a new
 *              object if it does not already exist.
 * Parameters:
 *    userId - Unique identifier of the user to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshUserById(const std::string& userId)
{
    SharedUser* sharedUser = m_registry.getUsers()->findById(userId.c_str());
    if (!sharedUser)
    {
        return;
    }
    std::map<std::string, User*>::iterator iterator = m_users.find(userId);
    if (iterator != m_users.end())
    {
        User* user = iterator->second;
        user->setUserName(sharedUser->username);
        user->setEmail(sharedUser->email);
        std::string password = sharedUser->password;
        user->setPassword(User::decryption(password));
        user->setPhoneNumber(sharedUser->phoneNumber);
        user->setStatus(static_cast<Enums::UserStatus>(sharedUser->status));
    }
    else
    {
        std::string password = sharedUser->password;
        User* user = Factory::getObject<User>(
            sharedUser->userId,
            sharedUser->username,
            sharedUser->email,
            User::decryption(password),
            sharedUser->phoneNumber,
            static_cast<Enums::UserType>(sharedUser->userType));
        if (user)
        {
            user->setStatus(static_cast<Enums::UserStatus>(sharedUser->status));
            m_users[user->getUserId()] = user;
        }
    }
}

/*
 * Function: DataStore::refreshMovieById
 * Description: Synchronizes a single Movie object in the in-memory cache with
 *              the latest SharedMovie record stored in the mapped file.
 * Parameters:
 *    movieId - Unique identifier of the movie to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshMovieById(const std::string& movieId)
{
    SharedMovie* sharedMovie = m_registry.getMovies()->findById(movieId.c_str());
    if (!sharedMovie)
    {
        return;
    }
    std::map<std::string, Movie*>::iterator iterator = m_movies.find(movieId);
    if (iterator != m_movies.end())
    {
        Movie* movie = iterator->second;
        movie->setTitle(sharedMovie->title);
        movie->setGenre(sharedMovie->genre);
        movie->setLanguage(sharedMovie->language);
        movie->setDuration(sharedMovie->duration);
        movie->setStatus(static_cast<Enums::MovieStatus>(sharedMovie->status));
    }
    else
    {
        Movie* movie = Factory::getObject<Movie>(
            sharedMovie->movieId,
            sharedMovie->title,
            sharedMovie->language,
            sharedMovie->genre,
            sharedMovie->duration);
        if (movie)
        {
            movie->setStatus(static_cast<Enums::MovieStatus>(sharedMovie->status));
            m_movies[movie->getMovieId()] = movie;
        }
    }
}

/*
 * Function: DataStore::refreshTheatreById
 * Description: Synchronizes a single Theatre object in the in-memory cache
 *              with the latest SharedTheatre record stored in the mapped file.
 * Parameters:
 *    theatreId - Unique identifier of the theatre to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshTheatreById(const std::string& theatreId)
{
    SharedTheatre* sharedTheatre = m_registry.getTheatres()->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return;
    }
    std::map<std::string, Theatre*>::iterator iterator = m_theatres.find(theatreId);
    if (iterator != m_theatres.end())
    {
        Theatre* theatre = iterator->second;
        theatre->setName(sharedTheatre->name);
        theatre->setCity(sharedTheatre->city);
        theatre->setAddress(sharedTheatre->address);
        theatre->setTheatrePhoneNumber(sharedTheatre->phoneNumber);
        theatre->setTheatreEmail(sharedTheatre->email);
        theatre->setStatus(static_cast<Enums::TheatreStatus>(sharedTheatre->status));
    }
    else
    {
        Theatre* theatre = Factory::getObject<Theatre>(
            sharedTheatre->theatreId,
            sharedTheatre->name,
            sharedTheatre->city,
            sharedTheatre->address,
            sharedTheatre->phoneNumber,
            sharedTheatre->email, nullptr);
        if (theatre)
        {
            theatre->setStatus(static_cast<Enums::TheatreStatus>(sharedTheatre->status));
            m_theatres[theatre->getTheatreId()] = theatre;
        }
    }
}

/*
 * Function: DataStore::refreshScreenById
 * Description: Synchronizes a single Screen object in the in-memory cache
 *              with the latest SharedScreen record stored in the mapped file.
 * Parameters:
 *    screenId - Unique identifier of the screen to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshScreenById(const std::string& screenId)
{
    SharedScreen* sharedScreen = m_registry.getScreens()->findById(screenId.c_str());
    if (!sharedScreen)
    {
        return;
    }
    std::map<std::string, Screen*>::iterator iterator = m_screens.find(screenId);
    if (iterator != m_screens.end())
    {
        Screen* screen = iterator->second;
        screen->setName(sharedScreen->name);
        screen->setTotalRows(sharedScreen->totalRows);
        screen->setTotalColumns(sharedScreen->totalColumns);
        screen->setScreenStatus(static_cast<Enums::ScreenStatus>(sharedScreen->status));
    }
    else
    {
        std::vector<std::vector<Seat*>> emptyGrid;
        Screen* screen = Factory::getObject<Screen>(
            sharedScreen->screenId,
            nullptr,
            sharedScreen->name,
            sharedScreen->totalRows,
            sharedScreen->totalColumns,
            emptyGrid);
        if (screen)
        {
            screen->setScreenStatus(static_cast<Enums::ScreenStatus>(sharedScreen->status));
            m_screens[screen->getScreenId()] = screen;
        }
    }
}

/*
 * Function: DataStore::refreshSeatById
 * Description: Synchronizes a single Seat object in the in-memory cache with
 *              the latest SharedSeat record stored in the mapped file.
 * Parameters:
 *    seatId - Unique identifier of the seat to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshSeatById(const std::string& seatId)
{
    SharedSeat* sharedSeat = m_registry.getSeats()->findById(seatId.c_str());
    if (!sharedSeat)
    {
        return;
    }
    std::map<std::string, Seat*>::iterator iterator = m_seats.find(seatId);
    if (iterator != m_seats.end())
    {
        Seat* seat = iterator->second;
        seat->setSeatRow(static_cast<char>(sharedSeat->seatRow));
        seat->setSeatColumn(sharedSeat->seatColumn);
        seat->setSeatAmount(sharedSeat->amount);
        seat->setSeatStatus(static_cast<Enums::SeatStatus>(sharedSeat->status));
    }
    else
    {
        Seat* seat = Factory::getObject<Seat>(
            sharedSeat->seatId,
            nullptr,
            static_cast<char>(sharedSeat->seatRow),
            sharedSeat->seatColumn,
            sharedSeat->amount,
            static_cast<Enums::SeatStatus>(sharedSeat->status));
        if (seat)
        {
            m_seats[seat->getSeatId()] = seat;
        }
    }
}

/*
 * Function: DataStore::refreshShowById
 * Description: Synchronizes a single Show object in the in-memory cache with
 *              the latest SharedShow record stored in the mapped file.
 * Parameters:
 *    showId - Unique identifier of the show to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshShowById(const std::string& showId)
{
    SharedShow* sharedShow = m_registry.getShows()->findById(showId.c_str());
    if (!sharedShow)
    {
        return;
    }
    std::map<std::string, Show*>::iterator iterator = m_shows.find(showId);
    if (iterator != m_shows.end())
    {
        Show* show = iterator->second;
        show->setStartTime(util::deserializeTime(sharedShow->startTime));
        show->setEndTime(util::deserializeTime(sharedShow->endTime));
        show->setShowStatus(static_cast<Enums::ShowStatus>(sharedShow->status));
    }
    else
    {
        Show* show = Factory::getObject<Show>(
            sharedShow->showId,
            nullptr,
            nullptr,
            util::deserializeTime(sharedShow->startTime),
            util::deserializeTime(sharedShow->endTime),
            nullptr);
        if (show)
        {
            show->setShowStatus(static_cast<Enums::ShowStatus>(sharedShow->status));
            m_shows[show->getShowId()] = show;
        }
    }
}

/*
 * Function: DataStore::refreshShowSeatAvailabilityById
 * Description: Synchronizes a single ShowSeatAvailability object in the
 *              in-memory cache with the latest SharedShowSeatAvailability
 *              record stored in the mapped file.
 * Parameters:
 *    availabilityId - Unique identifier of the show seat availability record
 * Returns:
 *    None
 */
void DataStore::refreshShowSeatAvailabilityById(const std::string& availabilityId)
{
    SharedShowSeatAvailability* sharedAvailability = m_registry.getAvailability()->findById(availabilityId.c_str());
    if (!sharedAvailability)
    {
        return;
    }
    std::map<std::string, ShowSeatAvailability*>::iterator iterator = m_showSeatAvailabilitys.find(availabilityId);
    std::map<std::string, Enums::BookingStatus> seatMap;
    for (int index = 0; index < sharedAvailability->seatCount; ++index)
    {
        seatMap[sharedAvailability->seatIds[index]] = static_cast<Enums::BookingStatus>(sharedAvailability->seatStatuses[index]);
    }
    if (iterator != m_showSeatAvailabilitys.end())
    {
        ShowSeatAvailability* availability = iterator->second;
        availability->setSeatAvailabilityMap(seatMap);
    }
    else
    {
        ShowSeatAvailability* availability = Factory::getObject<ShowSeatAvailability>(
            sharedAvailability->availabilityId,
            nullptr,
            seatMap);
        if (availability)
        {
            m_showSeatAvailabilitys[availability->getShowAvailabiltyId()] = availability;
        }
    }
}

/*
 * Function: DataStore::refreshBookingById
 * Description: Synchronizes a single Booking object in the in-memory cache
 *              with the latest SharedBooking record stored in the mapped file.
 * Parameters:
 *    bookingId - Unique identifier of the booking to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshBookingById(const std::string& bookingId)
{
    SharedBooking* sharedBooking = m_registry.getBookings()->findById(bookingId.c_str());
    if (!sharedBooking)
    {
        return;
    }
    std::map<std::string, Booking*>::iterator iterator = m_bookings.find(bookingId);
    if (iterator != m_bookings.end())
    {
        Booking* booking = iterator->second;
        booking->setStatus(static_cast<Enums::BookingStatus>(sharedBooking->status));
        booking->setAmount(sharedBooking->amount);
    }
    else
    {
        std::vector<Seat*> seats;
        Booking* booking = Factory::getObject<Booking>(
            sharedBooking->bookingId,
            nullptr,
            nullptr,
            seats,
            static_cast<Enums::BookingStatus>(sharedBooking->status),
            sharedBooking->amount);

        if (booking)
        {
            m_bookings[booking->getBookingId()] = booking;
        }
    }
}

/*
 * Function: DataStore::refreshPaymentById
 * Description: Synchronizes a single Payment object in the in-memory cache
 *              with the latest SharedPayment record stored in the mapped file.
 * Parameters:
 *    paymentId - Unique identifier of the payment to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshPaymentById(const std::string& paymentId)
{
    SharedPayment* sharedPayment = m_registry.getPayments()->findById(paymentId.c_str());
    if (!sharedPayment)
    {
        return;
    }
    std::map<std::string, Payment*>::iterator iterator = m_payments.find(paymentId);
    if (iterator != m_payments.end())
    {
        Payment* payment = iterator->second;
        payment->setAmount(sharedPayment->amount);
        payment->setPaymentMethod(static_cast<Enums::PaymentMethod>(sharedPayment->paymentMethod));
        payment->setStatus(static_cast<Enums::PaymentStatus>(sharedPayment->status));
        payment->setTimeStamp(util::deserializeTime(sharedPayment->time));
    }
    else
    {
        Payment* payment = Factory::getObject<Payment>(
            sharedPayment->paymentId,
            nullptr,
            sharedPayment->amount,
            static_cast<Enums::PaymentMethod>(sharedPayment->paymentMethod),
            util::deserializeTime(sharedPayment->time));
        if (payment)
        {
            payment->setStatus(static_cast<Enums::PaymentStatus>(sharedPayment->status));
            m_payments[payment->getPaymentId()] = payment;
        }
    }
}

/*
 * Function: DataStore::refreshTicketById
 * Description: Synchronizes a single Ticket object in the in-memory cache
 *              with the latest SharedTicket record stored in the mapped file.
 * Parameters:
 *    ticketId - Unique identifier of the ticket to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshTicketById(const std::string& ticketId)
{
    SharedTicket* sharedTicket = m_registry.getTickets()->findById(ticketId.c_str());
    if (!sharedTicket)
    {
        return;
    }
    std::map<std::string, Ticket*>::iterator iterator = m_tickets.find(ticketId);
    if (iterator != m_tickets.end())
    {
        Ticket* ticket = iterator->second;
        ticket->setTicketStatus(static_cast<Enums::TicketStatus>(sharedTicket->status));
    }
    else
    {
        Ticket* ticket = Factory::getObject<Ticket>(
            sharedTicket->ticketId,
            nullptr,
            nullptr);

        if (ticket)
        {
            ticket->setTicketStatus(static_cast<Enums::TicketStatus>(sharedTicket->status));
            m_tickets[ticket->getTicketId()] = ticket;
        }
    }
}

/*
 * Function: DataStore::refreshRefundById
 * Description: Synchronizes a single Refund object in the in-memory cache
 *              with the latest SharedRefund record stored in the mapped file.
 * Parameters:
 *    refundId - Unique identifier of the refund to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshRefundById(const std::string& refundId)
{
    SharedRefund* sharedRefund = m_registry.getRefunds()->findById(refundId.c_str());
    if (!sharedRefund)
    {
        return;
    }
    std::map<std::string, Refund*>::iterator iterator = m_refunds.find(refundId);
    if (iterator != m_refunds.end())
    {
        Refund* refund = iterator->second;
        refund->setRefundAmount(sharedRefund->amount);
        refund->setStatus(static_cast<Enums::RefundStatus>(sharedRefund->status));
        refund->setTime(util::deserializeTime(sharedRefund->time));
    }
    else
    {
        Refund* refund = Factory::getObject<Refund>(
            sharedRefund->refundId,
            nullptr,
            sharedRefund->amount,
            util::deserializeTime(sharedRefund->time));
        if (refund)
        {
            refund->setStatus(static_cast<Enums::RefundStatus>(sharedRefund->status));
            m_refunds[refund->getRefundId()] = refund;
        }
    }
}

/*
 * Function: DataStore::refreshNotificationById
 * Description: Synchronizes a single Notification object in the in-memory
 *              cache with the latest SharedNotification record stored in the
 *              mapped file.
 * Parameters:
 *    notificationId - Unique identifier of the notification to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshNotificationById(const std::string& notificationId)
{
    SharedNotification* sharedNotification = m_registry.getNotifications()->findById(notificationId.c_str());
    if (!sharedNotification)
    {
        return;
    }
    std::map<std::string, Notification*>::iterator iterator = m_notifications.find(notificationId);
    if (iterator != m_notifications.end())
    {
        Notification* notification = iterator->second;
        notification->setMessage(sharedNotification->message);
        notification->setStatus(static_cast<Enums::NotificationStatus>(sharedNotification->status));
        notification->setTime(util::deserializeTime(sharedNotification->time));
    }
    else
    {
        Notification* notification = Factory::getObject<Notification>(
            sharedNotification->notificationId,
            nullptr,
            sharedNotification->message,
            util::deserializeTime(sharedNotification->time));
        if (notification)
        {
            notification->setStatus(static_cast<Enums::NotificationStatus>(sharedNotification->status));
            m_notifications[notification->getNotificationId()] = notification;
        }
    }
}

/*
 * Function: DataStore::refreshLogById
 * Description: Synchronizes a single Log object in the in-memory cache with
 *              the latest SharedLog record stored in the mapped file.
 * Parameters:
 *    logId - Unique identifier of the log to synchronize
 * Returns:
 *    None
 */
void DataStore::refreshLogById(const std::string& logId)
{
    SharedLog* sharedLog = m_registry.getLogs()->findById(logId.c_str());
    if (!sharedLog)
    {
        return;
    }
    std::map<std::string, Log*>::iterator iterator = m_logs.find(logId);
    if (iterator != m_logs.end())
    {
        Log* log = iterator->second;
        log->setDescription(sharedLog->description);
        log->setLogType(static_cast<Enums::LogType>(sharedLog->type));
        log->setTimestamp(util::Timestamp::fromString(sharedLog->time));
    }
    else
    {
        Log* log = Factory::getObject<Log>(
            sharedLog->logId,
            sharedLog->description,
            static_cast<Enums::LogType>(sharedLog->type));
        if (log)
        {
            log->setTimestamp(util::Timestamp::fromString(sharedLog->time));
            m_logs[log->getLogId()] = log;
        }
    }
}

/*
 * Function: DataStore::rebuildTheatreRelationship
 * Description: Rebuilds relationships for a single Theatre object by restoring
 *              references to its associated owner, movies, and screens using
 *              the latest data available in shared memory and the internal
 *              datastore maps.
 * Parameters:
 *    theatreId - Unique identifier of the theatre whose relationships are to
 *                be rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildTheatreRelationship(const std::string& theatreId)
{
    SharedTheatre* sharedTheatre = m_registry.getTheatres()->findById(theatreId.c_str());
    if (!sharedTheatre)
    {
        return;
    }
    std::map<std::string, Theatre*>::iterator iterator = m_theatres.find(theatreId);
    if (iterator == m_theatres.end())
    {
        return;
    }
    Theatre* theatre = iterator->second;
    refreshUserById(sharedTheatre->ownerId);
    theatre->setTheatreOwner(findUserById(sharedTheatre->ownerId));
    std::vector<Movie*>& movies = theatre->getMoviesForUpdation();
    movies.clear();
    for (int index = 0; index < sharedTheatre->movieCount; ++index)
    {
        refreshMovieById(sharedTheatre->movieIds[index]);
        Movie* movie = findMovieById(sharedTheatre->movieIds[index]);
        if (movie)
        {
            movies.push_back(movie);
        }
    }
    std::vector<Screen*>& screens = theatre->getScreensForUpdation();
    screens.clear();
    for (int index = 0; index < sharedTheatre->screenCount; ++index)
    {
        refreshScreenById(sharedTheatre->screenIds[index]);
        Screen* screen = findScreenById(sharedTheatre->screenIds[index]);
        if (screen)
        {
            screens.push_back(screen);
        }
    }
}

/*
 * Function: DataStore::rebuildScreenRelationship
 * Description: Rebuilds relationships for a single Screen object by restoring
 *              references to its associated Theatre and rebuilding its seat
 *              grid using the latest Seat objects available in shared memory.
 * Parameters:
 *    screenId - Unique identifier of the screen whose relationships are to be
 *               rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildScreenRelationship(const std::string& screenId)
{
    SharedScreen* sharedScreen = m_registry.getScreens()->findById(screenId.c_str());
    if (!sharedScreen)
    {
        return;
    }
    std::map<std::string, Screen*>::iterator iterator = m_screens.find(screenId);
    if (iterator == m_screens.end())
    {
        return;
    }
    Screen* screen = iterator->second;
    refreshTheatreById(sharedScreen->theatreId);
    screen->setTheatre(findTheatreById(sharedScreen->theatreId));
    std::vector<std::vector<Seat*>> seatGrid;
    int currentSeat = 0;
    for (int row = 0; row < screen->getTotalRows(); ++row)
    {
        std::vector<Seat*> seatRow;
        for (int column = 0; column < screen->getTotalColumns(); ++column)
        {
            if (currentSeat >= sharedScreen->seatCount)
            {
                break;
            }
            refreshSeatById(sharedScreen->seatIds[currentSeat]);
            Seat* seat = findSeatById(sharedScreen->seatIds[currentSeat++]);
            if (seat)
            {
                seatRow.push_back(seat);
            }
        }
        seatGrid.push_back(seatRow);
    }
    screen->setSeatGrid(seatGrid);
}

/*
 * Function: DataStore::rebuildSeatRelationship
 * Description: Rebuilds relationships for a single Seat object by restoring
 *              its reference to the associated Screen object using the latest
 *              data available in shared memory.
 * Parameters:
 *    seatId - Unique identifier of the seat whose relationships are to be
 *             rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildSeatRelationship(const std::string& seatId)
{
    SharedSeat* sharedSeat = m_registry.getSeats()->findById(seatId.c_str());
    if (!sharedSeat)
    {
        return;
    }
    std::map<std::string, Seat*>::iterator iterator = m_seats.find(seatId);
    if (iterator == m_seats.end())
    {
        return;
    }
    Seat* seat = iterator->second;
    refreshScreenById(sharedSeat->screenId);
    seat->setScreen(findScreenById(sharedSeat->screenId));
}

/*
 * Function: DataStore::rebuildShowRelationship
 * Description: Rebuilds relationships for a single Show object by restoring
 *              references to its associated Movie, Screen, and
 *              ShowSeatAvailability objects using the latest data available in
 *              shared memory.
 * Parameters:
 *    showId - Unique identifier of the show whose relationships are to be
 *             rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildShowRelationship(const std::string& showId)
{
    SharedShow* sharedShow = m_registry.getShows()->findById(showId.c_str());
    if (!sharedShow)
    {
        return;
    }
    std::map<std::string, Show*>::iterator iterator = m_shows.find(showId);
    if (iterator == m_shows.end())
    {
        return;
    }
    Show* show = iterator->second;
    refreshMovieById(sharedShow->movieId);
    show->setMovie(findMovieById(sharedShow->movieId));
    refreshScreenById(sharedShow->screenId);
    rebuildScreenRelationship(sharedShow->screenId);
    show->setScreen(findScreenById(sharedShow->screenId));
    refreshShowSeatAvailabilityById(sharedShow->seatAvailabilityId);
    show->setSeatAvailability(findShowSeatAvailabilityById(sharedShow->seatAvailabilityId));
}

/*
 * Function: DataStore::rebuildShowSeatAvailabilityRelationship
 * Description: Rebuilds relationships for a single ShowSeatAvailability object
 *              by restoring its reference to the associated Show object using
 *              the latest data available in shared memory.
 * Parameters:
 *    availabilityId - Unique identifier of the show seat availability record
 *                     whose relationships are to be rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildShowSeatAvailabilityRelationship(const std::string& availabilityId)
{
    SharedShowSeatAvailability* sharedAvailability = m_registry.getAvailability()->findById(availabilityId.c_str());
    if (!sharedAvailability)
    {
        return;
    }
    std::map<std::string, ShowSeatAvailability*>::iterator iterator = m_showSeatAvailabilitys.find(availabilityId);
    if (iterator == m_showSeatAvailabilitys.end())
    {
        return;
    }
    ShowSeatAvailability* availability = iterator->second;
    refreshShowById(sharedAvailability->showId);
    rebuildShowRelationship(sharedAvailability->showId);
    availability->setShow(findShowById(sharedAvailability->showId));
}

/*
 * Function: DataStore::rebuildBookingRelationship
 * Description: Rebuilds relationships for a single Booking object by restoring
 *              references to its associated Customer, Show, and booked Seat
 *              objects using the latest data available in shared memory.
 * Parameters:
 *    bookingId - Unique identifier of the booking whose relationships are to
 *                be rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildBookingRelationship(const std::string& bookingId)
{
    SharedBooking* sharedBooking = m_registry.getBookings()->findById(bookingId.c_str());
    if (!sharedBooking)
    {
        return;
    }
    std::map<std::string, Booking*>::iterator iterator = m_bookings.find(bookingId);
    if (iterator == m_bookings.end())
    {
        return;
    }
    Booking* booking = iterator->second;
    refreshUserById(sharedBooking->customerId);
    booking->setCustomer(findUserById(sharedBooking->customerId));
    refreshShowById(sharedBooking->showId);
    rebuildShowRelationship(sharedBooking->showId);
    booking->setShow(findShowById(sharedBooking->showId));
    std::vector<Seat*> seats;
    for (int index = 0; index < sharedBooking->seatCount; ++index)
    {
        refreshSeatById(sharedBooking->seatIds[index]);
        Seat* seat = findSeatById(sharedBooking->seatIds[index]);
        if (seat)
        {
            seats.push_back(seat);
        }
    }
    booking->setBookedSeats(seats);
}

/*
 * Function: DataStore::rebuildPaymentRelationship
 * Description: Rebuilds relationships for a single Payment object by restoring
 *              its reference to the associated Booking object using the latest
 *              data available in shared memory.
 * Parameters:
 *    paymentId - Unique identifier of the payment whose relationships are to
 *                be rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildPaymentRelationship(const std::string& paymentId)
{
    SharedPayment* sharedPayment = m_registry.getPayments()->findById(paymentId.c_str());
    if (!sharedPayment)
    {
        return;
    }
    std::map<std::string, Payment*>::iterator iterator = m_payments.find(paymentId);
    if (iterator == m_payments.end())
    {
        return;
    }
    Payment* payment = iterator->second;
    refreshBookingById(sharedPayment->bookingId);
    rebuildBookingRelationship(sharedPayment->bookingId);
    payment->setBooking(findBookingById(sharedPayment->bookingId));
}

/*
 * Function: DataStore::rebuildTicketRelationship
 * Description: Rebuilds relationships for a single Ticket object by restoring
 *              references to its associated Payment and Customer objects using
 *              the latest data available in shared memory.
 * Parameters:
 *    ticketId - Unique identifier of the ticket whose relationships are to be
 *               rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildTicketRelationship(const std::string& ticketId)
{
    SharedTicket* sharedTicket = m_registry.getTickets()->findById(ticketId.c_str());
    if (!sharedTicket)
    {
        return;
    }
    std::map<std::string, Ticket*>::iterator iterator = m_tickets.find(ticketId);
    if (iterator == m_tickets.end())
    {
        return;
    }
    Ticket* ticket = iterator->second;
    refreshPaymentById(sharedTicket->paymentId);
    rebuildPaymentRelationship(sharedTicket->paymentId);
    ticket->setPayment(findPaymentById(sharedTicket->paymentId));
    refreshUserById(sharedTicket->customerId);
    ticket->setCustomer(findUserById(sharedTicket->customerId));
}

/*
 * Function: DataStore::rebuildRefundRelationship
 * Description: Rebuilds relationships for a single Refund object by restoring
 *              its reference to the associated Ticket object using the latest
 *              data available in shared memory.
 * Parameters:
 *    refundId - Unique identifier of the refund whose relationships are to be
 *               rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildRefundRelationship(const std::string& refundId)
{
    SharedRefund* sharedRefund = m_registry.getRefunds()->findById(refundId.c_str());
    if (!sharedRefund)
    {
        return;
    }
    std::map<std::string, Refund*>::iterator iterator = m_refunds.find(refundId);
    if (iterator == m_refunds.end())
    {
        return;
    }
    Refund* refund = iterator->second;
    refreshTicketById(sharedRefund->ticketId);
    rebuildTicketRelationship(sharedRefund->ticketId);
    refund->setBookedTicket(findTicketById(sharedRefund->ticketId));
}

/*
 * Function: DataStore::rebuildNotificationRelationship
 * Description: Rebuilds relationships for a single Notification object by
 *              restoring its reference to the associated receiver User object
 *              using the latest data available in shared memory.
 * Parameters:
 *    notificationId - Unique identifier of the notification whose
 *                     relationships are to be rebuilt
 * Returns:
 *    None
 */
void DataStore::rebuildNotificationRelationship(const std::string& notificationId)
{
    SharedNotification* sharedNotification = m_registry.getNotifications()->findById(notificationId.c_str());
    if (!sharedNotification)
    {
        return;
    }
    std::map<std::string, Notification*>::iterator iterator = m_notifications.find(notificationId);
    if (iterator == m_notifications.end())
    {
        return;
    }
    Notification* notification = iterator->second;
    refreshUserById(sharedNotification->userId);
    notification->setReceiver(findUserById(sharedNotification->userId));
}

/*
 * Function: DataStore::updateNotificationStatus
 * Description: Updates the status of a notification in the mapped notifications file.
 *              Locates the SharedNotification record by its unique ID and modifies
 *              the status field to reflect the new NotificationStatus value.
 * Parameters:
 *    notificationId - Unique identifier of the notification to update
 *    status         - New notification status to be applied (Enums::NotificationStatus)
 * Returns:
 *    None
 */
void DataStore::updateNotificationStatus(const std::string& notificationId, Enums::NotificationStatus status)
{
    SharedNotification* sharedNotification = m_registry.getNotifications()->findById(notificationId.c_str());
    if (!sharedNotification)
    {
        return;
    }
    sharedNotification->status = static_cast<int>(status);
}

/*
 * Function    : ensureFolder
 * Description : Ensures that the specified folder exists on the file system.
 *               If the folder does not exist, it attempts to create it.
 *               If creation fails for any reason other than the folder
 *               already existing, an error message is displayed.
 * Parameters  : const char* path - The path of the folder to be created
 * Return      : void
 */
void DataStore::ensureFolder(const char* path) {
    if (_mkdir(path) != 0) {
        if (errno != EEXIST) {
            std::cerr << "Failed to create folder!\n";
        }
    }
}

/*
 * Function: DataStore::~DataStore
 * Description: Destructor for the DataStore singleton. Iterates through all
 *              in-memory maps and deletes every heap-allocated object in
 *              dependency order to prevent dangling pointer dereferences.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
DataStore::~DataStore()
{
    clearData();
}