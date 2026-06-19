#include <sstream>
#include <iomanip>
#include "BookingManagementService.h"
#include "Factory.h"
#include "TicketManagementService.h"
#include "ApplicationConfig.h"

/*
 * Function: BookingManagementService
 * Description: Default constructor that initializes the booking management service
 *              with a reference to the shared DataStore instance.
 * Parameters: None
 * Returns: None
 */
BookingManagementService::BookingManagementService() :
    m_dataStore(DataStore::getInstance()),
    m_mutex(config::MutexMappings::BOOKING_MUTEX_NAME)
{
}

/*
 * Function: BookingManagementService::getAllBookings
 * Description: Retrieves bookings based on authenticated user type (customer or theatre owner).
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<const Booking*> - List of bookings
 */
const std::vector<const Booking*> BookingManagementService::getAllBookings()
{
    ScopedLock lock(m_mutex);
    std::vector<const Booking*> bookings;
    const User* user = m_dataStore.getAuthenticatedUser();
    Enums::UserType userType = Enums::UserType::USER_NOT_FOUND;
    if (user != nullptr)
    {
        userType = user->getUserType();
    }
    switch (userType)
    {
    case Enums::UserType::CUSTOMER:
        bookings = getCustomerBookings();
        break;
    case Enums::UserType::THEATRE_OWNER:
        bookings = getTheatreBookings();
        break;
    default:
        break;
    }
    return bookings;
}

/*
 * Function: BookingManagementService::getTheatreBookings
 * Description: Retrieves bookings associated with theatres owned by the authenticated user.
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<const Booking*> - List of theatre bookings
 */
const std::vector<const Booking*> BookingManagementService::getTheatreBookings()
{
    ScopedLock lock(m_mutex);
    std::vector<const Booking*> filteredBookings;
    const std::map<std::string, Booking*> bookings = m_dataStore.getBookings();
    const User* user = m_dataStore.getAuthenticatedUser();
    std::string userId;
    if (user != nullptr)
    {
        userId = user->getUserId();
    }
    for (std::map<std::string, Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getShow() && iterator->second->getShow()->getScreen() && iterator->second->getShow()->getScreen()->getTheatre())
        {
            const Theatre* theatre = iterator->second->getShow()->getScreen()->getTheatre();
            if (theatre->getTheatreOwner())
            {
                const std::string& theatreOwnerId = theatre->getTheatreOwner()->getUserId();
                if (userId == theatreOwnerId)
                {
                    filteredBookings.push_back(iterator->second);
                }
            }
        }
    }
    return filteredBookings;
}

/*
 * Function: BookingManagementService::getCustomerBookings
 * Description: Retrieves bookings associated with the authenticated customer.
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<const Booking*> - List of customer bookings
 */
const std::vector<const Booking*> BookingManagementService::getCustomerBookings()
{
    ScopedLock lock(m_mutex);
    std::vector<const Booking*> filteredBookings;
    const std::map<std::string, Booking*> bookings = m_dataStore.getBookings();
    const User* user = m_dataStore.getAuthenticatedUser();
    std::string userId;
    if (user != nullptr)
    {
        userId = user->getUserId();
    }
    for (std::map<std::string, Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getCustomer())
        {
            const User* customer = iterator->second->getCustomer();
            const std::string& bookingCustomerId = customer->getUserId();
            if (userId == bookingCustomerId)
            {
                filteredBookings.push_back(iterator->second);
            }
        }
    }
    return filteredBookings;
}

/*
 * Function: BookingManagementService::getAllBookingIds
 * Description: Retrieves all booking IDs for the authenticated user.
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<std::string> - List of booking IDs
 */
const std::vector<std::string> BookingManagementService::getAllBookingIds()
{
    ScopedLock lock(m_mutex);
    const std::vector<const Booking*> bookings = getAllBookings();
    std::vector<std::string> bookingIds;
    for (std::vector<const Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
    {
        if ((*iterator) != nullptr)
        {
            bookingIds.push_back((*iterator)->getBookingId());
        }
    }
    return bookingIds;
}

/*
 * Function: BookingManagementService::getBookingById
 * Description: Retrieves a booking object by its unique ID via DataStore.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    const Booking* - Pointer to the booking if found, nullptr otherwise
 */
const Booking* BookingManagementService::getBookingById(const std::string& bookingId)
{
    ScopedLock lock(m_mutex);
    return m_dataStore.getBookingById(bookingId);
}

/*
 * Function: BookingManagementService::getCancellableCustomerBookings
 * Description: Retrieves cancellable bookings for the authenticated customer.
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<const Booking*> - List of cancellable bookings
 */
const std::vector<const Booking*> BookingManagementService::getCancellableCustomerBookings()
{
    ScopedLock lock(m_mutex);
    std::vector<const Booking*> filteredBookings;
    const std::map<std::string, Booking*> bookings = m_dataStore.getBookings();
    const User* user = m_dataStore.getAuthenticatedUser();
    std::string userId;
    if (user != nullptr)
    {
        userId = user->getUserId();
    }
    for (std::map<std::string, Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getCustomer() && iterator->second->getShow() && iterator->second->getStatus() == Enums::BookingStatus::CONFIRMED)
        {
            const User* customer = iterator->second->getCustomer();
            const std::string& bookingCustomerId = customer->getUserId();
            const Show* show = iterator->second->getShow();
            time_t showTime = show->getStartTime();
            time_t currentTime = std::time(nullptr);
            if (userId == bookingCustomerId && difftime(showTime, currentTime) > 0)
            {
                filteredBookings.push_back(iterator->second);
            }
        }
    }
    return filteredBookings;
}

/*
 * Function: BookingManagementService::cancelBooking
 * Description: Cancels a booking by updating seat availability to PENDING and marking booking status as CANCELLED.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if cancelled, FAILED otherwise
 */
Enums::ProcessStatus BookingManagementService::cancelBooking(const std::string& bookingId)
{
    ScopedLock lock(m_mutex);
    Booking* booking = m_dataStore.getBookingByIdForUpdation(bookingId);
    if (booking == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    Show* show = booking->getShow();
    if (show == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    ShowSeatAvailability* showSeatAvailability = show->getSeatAvailability();
    if (showSeatAvailability == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::map<std::string, Enums::BookingStatus> seatMap = showSeatAvailability->getSeatAvailabilityMap();
    const std::vector<Seat*>& bookedSeats = booking->getBookedSeats();
    int numberOfBookedSeats = int(bookedSeats.size());
    int numberOfSeatsCancelled = 0;
    for (std::vector<Seat*>::const_iterator iterator = bookedSeats.begin(); iterator != bookedSeats.end(); ++iterator)
    {
        if ((*iterator) != nullptr)
        {
            seatMap[(*iterator)->getSeatId()] = Enums::BookingStatus::NOT_BOOKED;
            ++numberOfSeatsCancelled;
        }
    }
    Enums::ProcessStatus ticketCancellationAndRefundStatus = Enums::ProcessStatus::FAILED;
    ticketCancellationAndRefundStatus = cancelTicketAndProcessRefund(booking);
    if (numberOfBookedSeats == numberOfSeatsCancelled && ticketCancellationAndRefundStatus == Enums::ProcessStatus::SUCCESS)
    {
        showSeatAvailability->setSeatAvailabilityMap(seatMap);
        m_dataStore.updateBookingStatus(bookingId, Enums::BookingStatus::CANCELLED);
        booking->setStatus(Enums::BookingStatus::CANCELLED);
        std::string message = "Booking with ID : " + booking->getBookingId() + " has been cancelled.";
        m_event.notify("", m_dataStore.getAuthenticatedUser()->getUserId(), message);
        logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
        return Enums::ProcessStatus::SUCCESS;
    }
    std::string message = "Booking with ID : " + booking->getBookingId() + " could not been cancelled.";
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : bookSelectedSeats
* Description   : Creates a booking for the selected seats of a show.
*                 Marks seats as booked, calculates the booking amount,
*                 creates a Booking object, and stores it in the datastore.
* Parameters    :
*                  showId          - Unique identifier of the show
*                  selectedSeatIds - List of selected seat identifiers
* Return Type   : const Booking*
*/
const Booking* BookingManagementService::bookSelectedSeats(const std::string& showId, const std::vector<std::string>& selectedSeatIds)
{
    ScopedLock lock(m_mutex);
    Show* show = m_dataStore.getShowByIdForUpdation(showId);
    if (show == nullptr)
    {
        return nullptr;
    }
    ShowSeatAvailability* showSeatAvailability = show->getSeatAvailability();
    if (showSeatAvailability == nullptr)
    {
        return nullptr;
    }
    std::map<std::string, Enums::BookingStatus> seatMap = showSeatAvailability->getSeatAvailabilityMap();
    for (std::vector<std::string>::const_iterator iterator = selectedSeatIds.begin(); iterator != selectedSeatIds.end(); ++iterator)
    {
        seatMap[(*iterator)] = Enums::BookingStatus::CONFIRMED;
    }
    std::vector<Seat*> seats;
    getSeatsFromSeatIds(seats, selectedSeatIds);
    User* customer = m_dataStore.getAuthenticatedUser();
    double amount = getBookingAmount(seats);
    Booking* booking = Factory::getObject<Booking>(generateBookingId(), customer, show, seats, Enums::BookingStatus::PENDING, amount);
    std::string message = "Booking with ID : " + booking->getBookingId() + " has been created.";
    std::string notificationMessage = "Your booking was successful.";
    m_event.notify("", m_dataStore.getAuthenticatedUser()->getUserId(), notificationMessage);
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    m_notificationManagementService.sendNotification(m_dataStore.getAuthenticatedUser(), message);
    if (booking != nullptr)
    {
        m_dataStore.addBooking(booking);
        showSeatAvailability->setSeatAvailabilityMap(seatMap);
        return booking;
    }
    message = "Error in Booking creation";
    logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
    return nullptr;
}

/*
* Function Name : generateBookingId
* Description   : Generates a unique booking ID for a new booking.
* Parameters    : None
* Return Type   : const std::string
*/
const std::string BookingManagementService::generateBookingId()
{
    const int bookingsCount = m_dataStore.getBookingCount();
    int idNumber = bookingsCount + 1;
    std::ostringstream buffer;
    buffer << "BKG" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
* Function Name : getSeatsFromSeatIds
* Description   : Retrieves Seat objects corresponding to the provided seat IDs.
* Parameters    :
*                  seats   - Output vector to store retrieved Seat pointers
*                  seatIds - List of seat identifiers
* Return Type   : void
*/
void BookingManagementService::getSeatsFromSeatIds(std::vector<Seat*>& seats, const std::vector<std::string>& seatIds)
{
    for (std::vector<std::string>::const_iterator iterator = seatIds.begin(); iterator != seatIds.end(); ++iterator)
    {
        Seat* seat = m_dataStore.getSeatById(*iterator);
        if (seat != nullptr)
        {
            seats.push_back(seat);
        }
    }
}

/*
* Function Name : getBookingAmount
* Description   : Calculates the total booking amount for the provided seats.
* Parameters    :
*                  seats - Collection of seats included in the booking
* Return Type   : double
*/
double BookingManagementService::getBookingAmount(const std::vector<Seat*>& seats)
{
    double amount = 0.0;
    for (std::vector<Seat*>::const_iterator iterator = seats.begin(); iterator != seats.end(); ++iterator)
    {
        if ((*iterator) != nullptr)
        {
            amount += (*iterator)->getSeatAmount();
        }
    }
    return amount;
}

/*
* Function Name : cancelTicketAndProcessRefund
* Description   : Cancels the ticket associated with the provided booking
*                 and processes the corresponding refund.
* Parameters    :
*                  booking - Booking whose ticket must be cancelled
*                            and refunded
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus BookingManagementService::cancelTicketAndProcessRefund(const Booking* booking)
{
    TicketManagementService ticketManagementService;
    const Ticket* ticket = m_dataStore.getTicketForBooking(booking);
    if (ticket == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::string ticketId = ticket->getTicketId();
    return ticketManagementService.cancelTicket(ticketId);
}

/*
* Function Name : cancelBookingForFailedPayment
* Description   : Cancels a booking created during the booking workflow
*                 when payment processing or ticket generation fails.
*                 Restores seat availability and updates the booking status.
* Parameters    :
*                  bookingId - Unique identifier of the booking
* Return Type   : void
*/
void BookingManagementService::cancelBookingForFailedPayment(const std::string& bookingId)
{
    ScopedLock lock(m_mutex);
    Booking* booking = m_dataStore.getBookingByIdForUpdation(bookingId);
    if (booking == nullptr)
    {
        return;
    }
    Show* show = booking->getShow();
    if (show == nullptr)
    {
        return;
    }
    ShowSeatAvailability* showSeatAvailability = show->getSeatAvailability();
    if (showSeatAvailability == nullptr)
    {
        return;
    }
    std::map<std::string, Enums::BookingStatus> seatMap = showSeatAvailability->getSeatAvailabilityMap();
    const std::vector<Seat*>& bookedSeats = booking->getBookedSeats();
    int numberOfBookedSeats = int(bookedSeats.size());
    int numberOfSeatsCancelled = 0;
    for (std::vector<Seat*>::const_iterator iterator = bookedSeats.begin(); iterator != bookedSeats.end(); ++iterator)
    {
        if ((*iterator) != nullptr)
        {
            seatMap[(*iterator)->getSeatId()] = Enums::BookingStatus::NOT_BOOKED;
            ++numberOfSeatsCancelled;
        }
    }
    if (numberOfBookedSeats == numberOfSeatsCancelled)
    {
        showSeatAvailability->setSeatAvailabilityMap(seatMap);
        m_dataStore.updateBookingStatus(bookingId, Enums::BookingStatus::CANCELLED);
        booking->setStatus(Enums::BookingStatus::CANCELLED);
    }
}

/*
 * Function: BookingManagementService::getSeatIdsFromBooking
 * Description: Retrieves the seat IDs associated with a given Booking object.
 *              Iterates through the booked seats and collects their identifiers
 *              into a vector of strings for further processing or display.
 * Parameters:
 *    booking - Pointer to the Booking object whose seat IDs are to be extracted
 * Returns:
 *    const std::vector<const std::string> - A vector containing the seat IDs
 */
const std::vector<std::string> BookingManagementService::getSeatIdsFromBooking(const Booking* booking)
{
    const std::vector<Seat*>& seats = booking->getBookedSeats();
    std::vector<std::string> seatIds;
    for (std::vector<Seat*>::const_iterator iterator = seats.begin(); iterator != seats.end(); ++iterator)
    {
        seatIds.push_back((*iterator)->getSeatId());
    }
    return seatIds;
}