/*
 * File: Booking.cpp
 * Description: Implements the Booking class, providing constructors, getters,
 *              and setters for booking attributes such as booking ID, customer,
 *              show, booked seats, status, and amount.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "Booking.h"

 /*
  * Function: Booking::Booking
  * Description: Default constructor. Initializes booking attributes to empty
  *              or zero values, with pointers set to nullptr.
  * Returns: None
  */
Booking::Booking()
    : m_bookingId(""),
    m_customer(nullptr),
    m_show(nullptr),
    m_bookedSeats(),
    m_status(Enums::BookingStatus::PENDING),
    m_amount(0)
{}

/*
 * Function: Booking::Booking
 * Description: Parameterized constructor. Initializes booking attributes with
 *              provided values.
 * Parameters:
 *    const std::string& id - Unique booking identifier
 *    User* customer - Pointer to the customer making the booking
 *    Show* show - Pointer to the show being booked
 *    const std::vector<Seat*>& bookedSeats - List of booked seats
 *    int status - Current booking status
 *    int amount - Total booking amount
 * Returns: None
 */
Booking::Booking(const std::string& id,
    User* customer,
    Show* show,
    const std::vector<Seat*>& bookedSeats,
    Enums::BookingStatus status,
    double amount)
    : m_bookingId(id),
    m_customer(customer),
    m_show(show),
    m_bookedSeats(bookedSeats),
    m_status(status),
    m_amount(amount)
{}

/*
 * Function: Booking::getBookingId
 * Description: Retrieves the booking ID.
 * Returns:
 *    const std::string& - Booking ID
 */
const std::string& Booking::getBookingId() const
{
    return m_bookingId;
}

/*
 * Function: Booking::getCustomer
 * Description: Retrieves the customer associated with the booking.
 * Returns:
 *    User* - Pointer to the customer
 */
User* Booking::getCustomer() const
{
    return m_customer;
}

/*
 * Function: Booking::getShow
 * Description: Retrieves the show associated with the booking.
 * Returns:
 *    Show* - Pointer to the show
 */
Show* Booking::getShow() const
{
    return m_show;
}

/*
 * Function: Booking::getBookedSeats
 * Description: Retrieves the list of booked seats.
 * Returns:
 *    const std::vector<Seat*>& - List of booked seats
 */
const std::vector<Seat*>& Booking::getBookedSeats() const
{
    return m_bookedSeats;
}

/*
 * Function: Booking::getStatus
 * Description: Retrieves the booking status.
 * Returns:
 *    Enums::BookingStatus - Booking status
 */
Enums::BookingStatus Booking::getStatus() const
{
    return m_status;
}

/*
 * Function: Booking::getAmount
 * Description: Retrieves the booking amount.
 * Returns:
 *    double - Booking amount
 */
double Booking::getAmount() const
{
    return m_amount;
}

/*
 * Function: Booking::setBookingId
 * Description: Updates the booking ID.
 * Parameters:
 *    const std::string& id - New booking ID
 * Returns: None
 */
void Booking::setBookingId(const std::string& id)
{
    m_bookingId = id;
}

/*
 * Function: Booking::setCustomer
 * Description: Updates the customer associated with the booking.
 * Parameters:
 *    User* customer - New customer pointer
 * Returns: None
 */
void Booking::setCustomer(User* customer)
{
    m_customer = customer;
}

/*
 * Function: Booking::setShow
 * Description: Updates the show associated with the booking.
 * Parameters:
 *    Show* show - New show pointer
 * Returns: None
 */
void Booking::setShow(Show* show)
{
    m_show = show;
}

/*
 * Function: Booking::setBookedSeats
 * Description: Updates the list of booked seats.
 * Parameters:
 *    const std::vector<Seat*>& bookedSeats - New list of booked seats
 * Returns: None
 */
void Booking::setBookedSeats(const std::vector<Seat*>& bookedSeats)
{
    m_bookedSeats = bookedSeats;
}

/*
 * Function: Booking::setStatus
 * Description: Updates the booking status.
 * Parameters:
 *    Enums::BookingStatus status - New booking status
 * Returns: None
 */
void Booking::setStatus(Enums::BookingStatus status)
{
    m_status = status;
}

/*
 * Function: Booking::setAmount
 * Description: Updates the booking amount.
 * Parameters:
 *    double amount - New booking amount
 * Returns: None
 */
void Booking::setAmount(double amount)
{
    m_amount = amount;
}

/*
 * Function: serialize
 * Description: Converts a Booking object into a SharedBooking struct suitable
 *              for storage in a memory-mapped file. Copies booking metadata
 *              including booking ID, customer ID, show ID, status, amount,
 *              and seat identifiers up to SEAT_MAX_COUNT into fixed-size arrays.
 * Parameters:
 *    sharedBooking - Reference to a SharedBooking struct to populate with serialized data
 * Returns:
 *    None
 */
void Booking::serialize(SharedBooking& sharedBooking) const
{
    sharedBooking = {};
    strncpy_s(sharedBooking.bookingId, m_bookingId.c_str(), sizeof(sharedBooking.bookingId));
    strncpy_s(sharedBooking.customerId, 
        (m_customer ? m_customer->getUserId().c_str() : ""), 
        sizeof(sharedBooking.customerId));
    strncpy_s(sharedBooking.showId, 
        (m_show ? m_show->getShowId().c_str() : ""), 
        sizeof(sharedBooking.showId));
    sharedBooking.status = static_cast<int>(m_status);
    sharedBooking.amount = m_amount;
    sharedBooking.seatCount = 0;
    for (const Seat* seat : m_bookedSeats)
    {
        if (seat && sharedBooking.seatCount < config::Limit::SEAT_MAX_COUNT)
        {
            strncpy_s(sharedBooking.seatIds[sharedBooking.seatCount],
                seat->getSeatId().c_str(), 
                sizeof(sharedBooking.seatIds[0]));
            sharedBooking.seatCount++;
        }
    }
}

/*
 * Function: deserialize
 * Description: Reconstructs a Booking object from a SharedBooking struct.
 *              Initializes booking attributes including ID, status, and amount.
 *              Associated Customer, Show, and Seat pointers are set to nullptr
 *              initially and can be linked later when those objects are available
 *              in the DataStore.
 * Parameters:
 *    sharedBooking - Pointer to a SharedBooking struct containing serialized booking data
 * Returns:
 *    Pointer to a newly created Booking object, or nullptr if input is invalid
 */
Booking* Booking::deserialize(const SharedBooking* sharedBooking)
{
    if (sharedBooking == nullptr)
    {
        return nullptr;
    }
    std::vector<Seat*> emptySeats;
    Booking* booking = Factory::getObject<Booking>(
        sharedBooking->bookingId,
        nullptr,
        nullptr,
        emptySeats,
        static_cast<Enums::BookingStatus>(sharedBooking->status),
        sharedBooking->amount);
    return booking;
}