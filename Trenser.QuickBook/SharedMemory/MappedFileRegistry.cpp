/*
* File: MappedFileRegistry.cpp
* Description: Implements the MappedFileRegistry class by creating,
*              opening, providing access to, and releasing all memory-
*              mapped files used by the QuickBook application for
*              inter-process shared data storage.
* Author: Trenser
* Created: 14 June 2026
*/
#include "MappedFileRegistry.h"

/*
* Function: MappedFileRegistry::openAll
* Description: Creates and opens all memory-mapped files required by
*              the QuickBook application. If any mapped file fails to
*              open, all previously opened mappings are closed and the
*              operation is aborted.
* Parameters: None
* Returns:
*    true if all mapped files were opened successfully,
*    false otherwise
*/
bool MappedFileRegistry::openAll()
{
    m_users = new MappedFile<SharedUser>(config::File::USER_FILEPATH, config::FileMappings::USER_MAPPING_NAME);
    m_movies = new MappedFile<SharedMovie>(config::File::MOVIE_FILEPATH, config::FileMappings::MOVIE_MAPPING_NAME);
    m_theatres = new MappedFile<SharedTheatre>(config::File::THEATRE_FILEPATH, config::FileMappings::THEATRE_MAPPING_NAME);
    m_screens = new MappedFile<SharedScreen>(config::File::SCREEN_FILEPATH, config::FileMappings::SCREEN_MAPPING_NAME);
    m_seats = new MappedFile<SharedSeat>(config::File::SEAT_FILEPATH, config::FileMappings::SEAT_MAPPING_NAME);
    m_shows = new MappedFile<SharedShow>(config::File::SHOW_FILEPATH, config::FileMappings::SHOW_MAPPING_NAME);
    m_bookings = new MappedFile<SharedBooking>(config::File::BOOKING_FILEPATH, config::FileMappings::BOOKING_MAPPING_NAME);
    m_payments = new MappedFile<SharedPayment>(config::File::PAYMENT_FILEPATH, config::FileMappings::PAYMENT_MAPPING_NAME);
    m_refunds = new MappedFile<SharedRefund>(config::File::REFUND_FILEPATH, config::FileMappings::REFUND_MAPPING_NAME);
    m_tickets = new MappedFile<SharedTicket>(config::File::TICKET_FILEPATH, config::FileMappings::TICKET_MAPPING_NAME);
    m_notifications = new MappedFile<SharedNotification>(config::File::NOTIFICATION_FILEPATH, config::FileMappings::NOTIFICATION_MAPPING_NAME);
    m_availability = new MappedFile<SharedShowSeatAvailability>(config::File::SHOW_SEAT_FILEPATH, config::FileMappings::SHOW_SEAT_MAPPING_NAME);
    m_logs = new MappedFile<SharedLog>(config::File::LOG_FILEPATH, config::FileMappings::LOG_MAPPING_NAME);
    if (!(m_users->open() &&
        m_movies->open() &&
        m_theatres->open() &&
        m_screens->open() &&
        m_seats->open() &&
        m_shows->open() &&
        m_bookings->open() &&
        m_payments->open() &&
        m_refunds->open() &&
        m_tickets->open() &&
        m_notifications->open() &&
        m_availability->open() &&
        m_logs->open()))
    {
        closeAll();
        return false;
    }
    return true;
}

/*
* Function: MappedFileRegistry::getUsers
* Description: Retrieves the mapped file that stores shared user data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedUser records
*/
MappedFile<SharedUser>* MappedFileRegistry::getUsers()
{
    return m_users;
}

/*
* Function: MappedFileRegistry::getMovies
* Description: Retrieves the mapped file that stores shared movie data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedMovie records
*/
MappedFile<SharedMovie>* MappedFileRegistry::getMovies()
{
    return m_movies;
}

/*
* Function: MappedFileRegistry::getTheatres
* Description: Retrieves the mapped file that stores shared theatre data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedTheatre records
*/
MappedFile<SharedTheatre>* MappedFileRegistry::getTheatres()
{
    return m_theatres;
}

/*
* Function: MappedFileRegistry::getScreens
* Description: Retrieves the mapped file that stores shared screen data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedScreen records
*/
MappedFile<SharedScreen>* MappedFileRegistry::getScreens()
{
    return m_screens;
}

/*
* Function: MappedFileRegistry::getSeats
* Description: Retrieves the mapped file that stores shared seat data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedSeat records
*/
MappedFile<SharedSeat>* MappedFileRegistry::getSeats()
{
    return m_seats;
}

/*
* Function: MappedFileRegistry::getShows
* Description: Retrieves the mapped file that stores shared show data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedShow records
*/
MappedFile<SharedShow>* MappedFileRegistry::getShows()
{
    return m_shows;
}

/*
* Function: MappedFileRegistry::getBookings
* Description: Retrieves the mapped file that stores shared booking data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedBooking records
*/
MappedFile<SharedBooking>* MappedFileRegistry::getBookings()
{
    return m_bookings;
}

/*
* Function: MappedFileRegistry::getPayments
* Description: Retrieves the mapped file that stores shared payment data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedPayment records
*/
MappedFile<SharedPayment>* MappedFileRegistry::getPayments()
{
    return m_payments;
}

/*
* Function: MappedFileRegistry::getRefunds
* Description: Retrieves the mapped file that stores shared refund data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedRefund records
*/
MappedFile<SharedRefund>* MappedFileRegistry::getRefunds()
{
    return m_refunds;
}

/*
* Function: MappedFileRegistry::getTickets
* Description: Retrieves the mapped file that stores shared ticket data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedTicket records
*/
MappedFile<SharedTicket>* MappedFileRegistry::getTickets()
{
    return m_tickets;
}

/*
* Function: MappedFileRegistry::getNotifications
* Description: Retrieves the mapped file that stores shared notification data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedNotification records
*/
MappedFile<SharedNotification>* MappedFileRegistry::getNotifications()
{
    return m_notifications;
}

/*
* Function: MappedFileRegistry::getAvailability
* Description: Retrieves the mapped file that stores shared show seat
*              availability data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedShowSeatAvailability records
*/
MappedFile<SharedShowSeatAvailability>* MappedFileRegistry::getAvailability()
{
    return m_availability;
}

/*
* Function: MappedFileRegistry::getLogs
* Description: Retrieves the mapped file that stores shared log data.
* Parameters: None
* Returns:
*    Pointer to the mapped file containing SharedLog records
*/
MappedFile<SharedLog>* MappedFileRegistry::getLogs()
{
    return m_logs;
}

/*
 * Function: getUsersCount
 * Description: Retrieves the total number of users from the mapped file registry.
 * Parameters:
 *    None
 * Returns:
 *    Integer count of users
 */
int MappedFileRegistry::getUsersCount() const
{
    return m_users->getRecordCount();
}

/*
* Function: MappedFileRegistry::closeAll
* Description: Releases all mapped file objects managed by the registry
*              and resets their pointers to nullptr.
* Parameters: None
* Returns: None
*/
void MappedFileRegistry::closeAll()
{
    delete m_users;
    m_users = nullptr;
    delete m_movies;
    m_movies = nullptr;
    delete m_theatres;
    m_theatres = nullptr;
    delete m_screens;
    m_screens = nullptr;
    delete m_seats;
    m_seats = nullptr;
    delete m_shows;
    m_shows = nullptr;
    delete m_bookings;
    m_bookings = nullptr;
    delete m_payments;
    m_payments = nullptr;
    delete m_refunds;
    m_refunds = nullptr;
    delete m_tickets;
    m_tickets = nullptr;
    delete m_notifications;
    m_notifications = nullptr;
    delete m_availability;
    m_availability = nullptr;
    delete m_logs;
    m_logs = nullptr;
}
