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
{
}

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
{
}

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
 * Description: Converts Bookig object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string Booking::serialize()
{
    std::string result = m_bookingId + config::delimeter::comma;
    if (m_customer)
    {
        result += m_customer->getUserId() + config::delimeter::comma;
    }
    if (m_show)
    {
        result += m_show->getShowId() + config::delimeter::comma;
    }
    if (!m_bookedSeats.empty())
    {
        for (std::vector<Seat*>::const_iterator iterator = m_bookedSeats.begin(); iterator != m_bookedSeats.end(); ++iterator)
        {
            result += (*iterator)->getSeatId() + config::delimeter::verticalBar;
        }
    }
    result += Enums::getBookingStatusString(m_status) + config::delimeter::comma +
        std::to_string(m_amount);
    return result;
}

/*
 * Function: Booking::deserialize
 * Description: Deserializes a single line of CSV-formatted booking data into a Booking object. 
 *              Extracts fields such as Booking ID, Customer ID, Show ID, Booked Seat, Status, and Amount. 
 *              Converts string values into appropriate types (double for amount, enum for status). 
 *              The Customer and Show pointers are set to nullptr initially and can be linked later 
 *              when those objects are available in the DataStore. The booked seats are initialized 
 *              as an empty container and can be populated afterward.
 * Parameters:
 *    lines - A reference to a string containing one line of CSV booking data.
 * Returns:
 *    A pointer to a newly created Booking object populated with the deserialized data.
 */
Booking* Booking::deserialize(std::string& lines)
{
    std::string bookingId, customerId, showId, bookedSeat, status,amount;
    std::stringstream lineStream(lines);
    getline(lineStream, bookingId, ',');
    getline(lineStream, customerId, ',');
    getline(lineStream, showId, ',');
    getline(lineStream, bookedSeat, ',');
    getline(lineStream, status, ',');
    getline(lineStream, amount, ',');
    return new Booking(bookingId, nullptr, nullptr, {}, Enums::getBookingStatus(status), stod(amount));
}