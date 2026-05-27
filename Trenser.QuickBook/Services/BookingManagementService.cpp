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

