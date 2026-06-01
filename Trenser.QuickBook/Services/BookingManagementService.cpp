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

/*
 * Function: BookingManagementService::loadBookingData
 * Description: Loads all booking data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Booking object via Booking::deserialize,
 *              and restores associations with Customer, Show, and Booked Seats if their IDs
 *              are present and found in the DataStore.
 *              Finally, adds the reconstructed Booking to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void BookingManagementService::loadBookingData()
{
    std::string bookingId, customerId, showId, bookedSeat, status, amount;
    std::vector<std::string> lines = FileManagement::readlines(PATH);
    for (int index = 0; index < lines.size(); index++)
    {
        Booking* booking = Booking::deserialize(lines[index]);
        std::stringstream lineStream(lines[index]);
        getline(lineStream, bookingId, ',');
        getline(lineStream, customerId, ',');
        getline(lineStream, showId, ',');
        getline(lineStream, bookedSeat, ',');
        getline(lineStream, status, ',');
        getline(lineStream, amount, ',');
        if (!customerId.empty())
        {
            User* customer = m_dataStore.getUserById(customerId);
            booking->setCustomer(customer);
        }
        if (!showId.empty())
        {
            Show* show = m_dataStore.getShowById(showId);
            booking->setShow(show);
        }
        if (!bookedSeat.empty())
        {
            std::vector<Seat*> seats;
            std::stringstream seatStream(bookedSeat);
            std::string seatId;
            while (getline(seatStream, seatId, '|'))
            {
                Seat* seat = m_dataStore.getSeatById(seatId);
                if (seat != nullptr)
                {
                    seats.push_back(seat);
                }
            }
            booking->setBookedSeats(seats);
        }
        m_dataStore.addBooking(booking);
    }
}