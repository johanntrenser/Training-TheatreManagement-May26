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
 * Function: BookingManagementService::saveBookingData
 * Description: Saves all booking data from the DataStore into a CSV file.
 *              Uses a configurable header (from config::Header::BOOKING_HEADER)
 *              and delegates serialization of each Booking object to its
 *              serialize() method for consistent formatting.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void BookingManagementService::saveBookingData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::BOOKING_HEADER);
    const std::map<std::string, Booking*>& bookings = m_dataStore.getBookings();
    for (std::map<std::string, Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
    {
        lines.push_back((iterator->second)->serialize());
    }
    FileManagement::writeLines(std::string(config::File::BOOKING_FILEPATH), lines);
}