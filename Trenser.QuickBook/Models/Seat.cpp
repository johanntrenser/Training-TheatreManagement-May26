/*
 * File: Seat.cpp
 * Description: Implements the Seat class, which represents a seat in a screen.
 *              Stores details such as seat ID, associated screen, row, column,
 *              type, status, and booking status.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#include "Seat.h"
#include "Screen.h"

 /*
  * Function: Seat::Seat
  * Description: Default constructor initializing seat with default values.
  * Returns:
  *    Seat object
  */
Seat::Seat()
    : m_seatId(""),
    m_screen(nullptr),
    m_seatRow('A'),
    m_seatColumn(0),
    m_seatType(0),
    m_seatStatus(0),
    m_bookingStatus(0)
{}

/*
 * Function: Seat::Seat
 * Description: Parameterized constructor initializing seat details.
 * Parameters:
 *    const std::string& id - Unique seat identifier
 *    Screen* screen - Associated screen object
 *    char seatRow - Seat row character
 *    int seatColumn - Seat column number
 *    int seatType - Seat type code
 *    int seatStatus - Seat status code
 *    int bookingStatus - Booking status code
 * Returns:
 *    Seat object
 */
Seat::Seat(const std::string& id,
    Screen* screen,
    char seatRow,
    int seatColumn,
    int seatType,
    int seatStatus,
    int bookingStatus)
    : m_seatId(id),
    m_screen(screen),
    m_seatRow(seatRow),
    m_seatColumn(seatColumn),
    m_seatType(seatType),
    m_seatStatus(seatStatus),
    m_bookingStatus(bookingStatus)
{}

/*
 * Function: Seat::getSeatId
 * Description: Retrieves the unique seat ID.
 * Returns:
 *    const std::string& - Seat ID
 */
const std::string& Seat::getSeatId() const
{
    return m_seatId;
}

/*
 * Function: Seat::getScreen
 * Description: Retrieves the associated screen object.
 * Returns:
 *    Screen* - Pointer to the screen
 */
Screen* Seat::getScreen() const
{
    return m_screen;
}

/*
 * Function: Seat::getSeatRow
 * Description: Retrieves the seat row character.
 * Returns:
 *    char - Seat row
 */
char Seat::getSeatRow() const
{
    return m_seatRow;
}

/*
 * Function: Seat::getSeatColumn
 * Description: Retrieves the seat column number.
 * Returns:
 *    int - Seat column
 */
int Seat::getSeatColumn() const
{
    return m_seatColumn;
}

/*
 * Function: Seat::getSeatType
 * Description: Retrieves the seat type code.
 * Returns:
 *    int - Seat type
 */
int Seat::getSeatType() const
{
    return m_seatType;
}

/*
 * Function: Seat::getSeatStatus
 * Description: Retrieves the seat status code.
 * Returns:
 *    int - Seat status
 */
int Seat::getSeatStatus() const
{
    return m_seatStatus;
}

/*
 * Function: Seat::getBookingStatus
 * Description: Retrieves the booking status code.
 * Returns:
 *    int - Booking status
 */
int Seat::getBookingStatus() const
{
    return m_bookingStatus;
}

/*
 * Function: Seat::setSeatId
 * Description: Sets the unique seat ID.
 * Parameters:
 *    const std::string& id - New seat ID
 * Returns:
 *    void
 */
void Seat::setSeatId(const std::string& id)
{
    m_seatId = id;
}

/*
 * Function: Seat::setScreen
 * Description: Sets the associated screen object.
 * Parameters:
 *    Screen* screen - Pointer to the screen
 * Returns:
 *    void
 */
void Seat::setScreen(Screen* screen)
{
    m_screen = screen;
}

/*
 * Function: Seat::setSeatRow
 * Description: Sets the seat row character.
 * Parameters:
 *    char seatRow - New seat row
 * Returns:
 *    void
 */
void Seat::setSeatRow(char seatRow)
{
    m_seatRow = seatRow;
}

/*
 * Function: Seat::setSeatColumn
 * Description: Sets the seat column number.
 * Parameters:
 *    int seatColumn - New seat column
 * Returns:
 *    void
 */
void Seat::setSeatColumn(int seatColumn)
{
    m_seatColumn = seatColumn;
}

/*
 * Function: Seat::setSeatType
 * Description: Sets the seat type code.
 * Parameters:
 *    int seatType - New seat type
 * Returns:
 *    void
 */
void Seat::setSeatType(int seatType)
{
    m_seatType = seatType;
}

/*
 * Function: Seat::setSeatStatus
 * Description: Sets the seat status code.
 * Parameters:
 *    int seatStatus - New seat status
 * Returns:
 *    void
 */
void Seat::setSeatStatus(int seatStatus)
{
    m_seatStatus = seatStatus;
}

/*
 * Function: Seat::setBookingStatus
 * Description: Sets the booking status code.
 * Parameters:
 *    int bookingStatus - New booking status
 * Returns:
 *    void
 */
void Seat::setBookingStatus(int bookingStatus)
{
    m_bookingStatus = bookingStatus;
}
