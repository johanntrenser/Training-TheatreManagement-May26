#include "BookingManagementService.h"

/*
 * Function: BookingManagementService
 * Description: Default constructor that initializes the booking management service
 *              with a reference to the shared DataStore instance.
 * Parameters: None
 * Returns: None
 */
BookingManagementService::BookingManagementService() :
    m_dataStore(DataStore::getInstance())
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
const std::vector<const Booking*> BookingManagementService::getAllBookings() const
{
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
const std::vector<const Booking*> BookingManagementService::getTheatreBookings() const
{
    std::vector<const Booking*> filteredBookings;
    const std::map<std::string, Booking*> bookings = m_dataStore.getBookings();
    const User* user = m_dataStore.getAuthenticatedUser();
    std::string userId;;
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
const std::vector<const Booking*> BookingManagementService::getCustomerBookings() const
{
    std::vector<const Booking*> filteredBookings;
    const std::map<std::string, Booking*> bookings = m_dataStore.getBookings();
    const User* user = m_dataStore.getAuthenticatedUser();
    std::string userId;;
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
const std::vector<std::string> BookingManagementService::getAllBookingIds() const
{
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
const Booking* BookingManagementService::getBookingById(const std::string& bookingId) const
{
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
const std::vector<const Booking*> BookingManagementService::getCancellableCustomerBookings() const
{
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
    int numberOfBookedSeats = bookedSeats.size();
    int numberOfSeatsCancelled = 0;
    for (std::vector<Seat*>::const_iterator iterator = bookedSeats.begin(); iterator != bookedSeats.end(); ++iterator)
    {
        if ((*iterator) != nullptr)
        {
            seatMap[(*iterator)->getSeatId()] = Enums::BookingStatus::PENDING;
            ++numberOfSeatsCancelled;
        }
    }
    if (numberOfBookedSeats == numberOfSeatsCancelled)
    {
        showSeatAvailability->setSeatAvailabilityMap(seatMap);
        booking->setStatus(Enums::BookingStatus::CANCELLED);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

