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
 *              MappedFileRegistry.
 * Parameters: None
 * Returns:
 *    true if all mapped files were initialized successfully,
 *    false otherwise
 */
bool DataStore::initialize()
{
    return m_registry.openAll() && m_sessionManager.open();

}

/*
 * Function: DataStore::getUsers
 * Description: Retrieves the collection of users stored in the DataStore.
 * Returns:
 *    A constant reference to the map of user IDs to User pointers.
 */
const std::map<string, User*>& DataStore::getUsers()
{
    clearData();
    MappedFile<SharedUser>* userFile = m_registry.getUsers();
    if (userFile != nullptr)
    {
        int recordCount = 0;
        SharedUser* users = userFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            m_users[users[index].userId] = User::deserialize(&users[index]);
        }
    }
    return m_users;
}

/*
 * Function: DataStore::addUser
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
    delete user;
}

/*
 * Function: DataStore::getLogs
 * Description: Retrieves the collection of logs stored in the DataStore.
 * Returns:
 *    A constant reference to the map of log IDs to Log pointers.
 */
const std::map<std::string, Log*>& DataStore::getLogs()
{
    clearData();
    MappedFile<SharedLog>* logsFile = m_registry.getLogs();
    if (logsFile != nullptr)
    {
        int recordCount = 0;
        SharedLog* logs = logsFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            m_logs[logs[index].logId] = Log::deserialize(&logs[index]);
        }
    }
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
    delete log;
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
    delete m_currentUser;
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
    clearData();
    MappedFile<SharedMovie>* movieFile = m_registry.getMovies();
    if (movieFile != nullptr)
    {
        int recordCount = 0;
        SharedMovie* movie = movieFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            m_movies[movie[index].movieId] = Movie::deserialize(&movie[index]);
        }
    }
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
    delete movie;
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
    clearData();
    MappedFile<SharedScreen>* screensFile = m_registry.getScreens();
    if (screensFile)
    {
        int recordCount = 0;
        SharedScreen* screens = screensFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            Screen* screen = Screen::deserialize(&screens[index]);
            Theatre* theatre = getTheatreById(screens[index].theatreId);
            if (theatre && screen)
            {
                screen->setTheatre(theatre);
                m_screens[screen->getScreenId()] = screen;
            }
        }
    }
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
* Description   : Returns all tickets stored in the mapped file of ticket records.
* Parameters    : None
* Return Type   : std::map<std::string, Ticket*>&
*/
std::map<std::string, Ticket*>& DataStore::getTickets()
{
    clearData();
    MappedFile<SharedTicket>* ticketFile = m_registry.getTickets();
    if (ticketFile)
    {
        int recordCount = 0;
        SharedTicket* tickets = ticketFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            Ticket* ticket = Ticket::deserialize(&tickets[index]);
            if (ticket)
            {
                Payment* payment = getPaymentById(tickets[index].paymentId);
                User* customer = getUserById(tickets[index].customerId);
                if (payment)
                {
                    ticket->setPayment(payment);
                }
                if (customer)
                {
                    ticket->setCustomer(customer);
                }
                m_tickets[ticket->getTicketId()] = ticket;
            }
        }
    }
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
    MappedFile<SharedRefund>* refundFile = m_registry.getRefunds();
    if (refundFile)
    {
        int recordCount = 0;
        SharedRefund* refunds = refundFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            Refund* refund = Refund::deserialize(&refunds[index]);
            if (refund)
            {
                Ticket* ticket = getTicketById(refunds[index].paymentId);
                if (ticket)
                {
                    refund->setBookedTicket(ticket);
                }
                m_refunds[refund->getRefundId()] = refund;
            }
        }
    }
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
    clearData();
    MappedFile<SharedPayment>* paymentFile = m_registry.getPayments();
    if (paymentFile)
    {
        int recordCount = 0;
        SharedPayment* payments = paymentFile->getAllRecords(recordCount);
        for (int index = 0; index < recordCount; ++index)
        {
            Payment* payment = Payment::deserialize(&payments[index]);
            if (payment)
            {
                Booking* booking = getBookingById(payments[index].bookingId);
                if (booking)
                {
                    payment->setBooking(booking);
                }
                m_payments[payment->getPaymentId()] = payment;
            }
        }
    }
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
Booking* DataStore::getBookingById(const std::string& bookingId)
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

/*
* Function Name : getSeatById
* Description   : Retrieves a seat using the provided seat ID.
* Parameters    :
*                  seatId - Unique identifier of the seat
* Return Type   : Seat*
*/
Seat* DataStore::getSeatById(const std::string seatId)
{
    std::map<std::string, Seat*>::const_iterator iterator = m_seats.find(seatId);
    if (iterator == m_seats.end())
    {
        return nullptr;
    }
    return iterator->second;
}

/*
* Function Name : addBooking
* Description   : Adds a booking object to the datastore.
* Parameters    :
*                  booking - Pointer to the booking object to be stored
* Return Type   : void
*/
void DataStore::addBooking(Booking* booking)
{
    m_bookings[booking->getBookingId()] = booking;
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
    Payment* payment = nullptr;
    std::string bookingId = booking->getBookingId();
    for (std::map<std::string, Payment*>::iterator iterator = m_payments.begin(); iterator != m_payments.end(); ++iterator)
    {
        if (iterator->second != nullptr)
        {
            Booking* booking = iterator->second->getBooking();
            if (booking != nullptr && booking->getBookingId() == bookingId)
            {
                payment = iterator->second;
            }
        }
    }
    if (payment == nullptr)
    {
        return nullptr;
    }
    for (std::map<std::string, Ticket*>::iterator iterator = m_tickets.begin(); iterator != m_tickets.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getPayment() && iterator->second->getPayment()->getPaymentId() == payment->getPaymentId())
        {
            return iterator->second;
        }
    }
    return nullptr;
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
    MappedFile<SharedUser>* usersFile = m_registry.getUsers();
    if (usersFile)
    {
        SharedUser* sharedUser = usersFile->findById(userId.c_str());
        if (sharedUser != nullptr)
        {
            User* user = User::deserialize(sharedUser);
            if (user)
            {
                m_users[user->getUserId()] = user;
            }
            return m_users[userId];
        }
    }
    return nullptr;
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
}

/*
 * Function: DataStore::addNotification
 * Description: Adds or updates a Notification object in the DataStore.
 *              Uses the Notification’s unique ID as the key in the
 *              internal notifications map. If a Notification with the
 *              same ID already exists, it will be replaced.
 * Parameters:
 *    notification - pointer to the Notification object to be stored
 * Returns:
 *    None
 */
void DataStore::addNotification(Notification* notification)
{
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
    return m_bookings[bookingId];
}

/*
 * Function: DataStore::getShowSeatAvailabilityList
 * Description: Retrieves the entire map of ShowSeatAvailability objects stored in the DataStore.
 *              Returns a copy of the internal map keyed by unique ShowSeatAvailability IDs.
 *              This allows iteration or lookup of all seat availability records associated with shows.
 * Parameters:
 *    None
 * Returns:
 *    A std::map<std::string, ShowSeatAvailability*> containing all ShowSeatAvailability objects.
 */
std::map<std::string, ShowSeatAvailability*> DataStore::getShowSeatAvailabilityList()
{
    return m_showSeatAvailabilitys;
}

void DataStore::addShowSeatAvailabilityList(ShowSeatAvailability* showSeatAvailability)
{
    m_showSeatAvailabilitys[showSeatAvailability->getShowAvailabiltyId()] = showSeatAvailability;
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
    return m_shows[id];
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
    MappedFile<SharedPayment>* paymentsFile = m_registry.getPayments();
    if (paymentsFile)
    {
        SharedPayment* sharedPayment = paymentsFile->findById(paymentId.c_str());
        if (sharedPayment != nullptr)
        {
            Payment* payment = Payment::deserialize(sharedPayment);
            if (payment)
            {
                m_payments[payment->getPaymentId()] = payment;
            }
            return m_payments[paymentId];
        }
    }
    return nullptr;
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
    for (std::map<std::string, Ticket*>::iterator iterator = m_tickets.begin(); iterator != m_tickets.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Refund*>::iterator iterator = m_refunds.begin(); iterator != m_refunds.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Payment*>::iterator iterator = m_payments.begin(); iterator != m_payments.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Booking*>::iterator iterator = m_bookings.begin(); iterator != m_bookings.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, ShowSeatAvailability*>::iterator iterator = m_showSeatAvailabilitys.begin(); iterator != m_showSeatAvailabilitys.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Show*>::iterator iterator = m_shows.begin(); iterator != m_shows.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Seat*>::iterator iterator = m_seats.begin(); iterator != m_seats.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Screen*>::iterator iterator = m_screens.begin(); iterator != m_screens.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Theatre*>::iterator iterator = m_theatres.begin(); iterator != m_theatres.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Movie*>::iterator iterator = m_movies.begin(); iterator != m_movies.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Notification*>::iterator iterator = m_notifications.begin(); iterator != m_notifications.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, Log*>::iterator iterator = m_logs.begin(); iterator != m_logs.end(); ++iterator)
    {
        delete iterator->second;
    }
    for (std::map<std::string, User*>::iterator iterator = m_users.begin(); iterator != m_users.end(); ++iterator)
    {
        delete iterator->second;
    }
}