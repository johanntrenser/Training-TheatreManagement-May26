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
    m_status(0),
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
    int status,
    int amount)
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
 *    int - Booking status
 */
int Booking::getStatus() const
{
    return m_status;
}

/*
 * Function: Booking::getAmount
 * Description: Retrieves the booking amount.
 * Returns:
 *    int - Booking amount
 */
int Booking::getAmount() const
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
 *    int status - New booking status
 * Returns: None
 */
void Booking::setStatus(int status)
{
    m_status = status;
}

/*
 * Function: Booking::setAmount
 * Description: Updates the booking amount.
 * Parameters:
 *    int amount - New booking amount
 * Returns: None
 */
void Booking::setAmount(int amount)
{
    m_amount = amount;
}
