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
    m_amount(0),
    m_seatStatus(Enums::SeatStatus::AVAILABLE)
{
}

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
    double m_amount,
    Enums::SeatStatus seatStatus)
    : m_seatId(id),
    m_screen(screen),
    m_seatRow(seatRow),
    m_seatColumn(seatColumn),
    m_amount(0),
    m_seatStatus(seatStatus)
{
}

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
 * Function: getSeatAmount
 * Description: Retrieves the seat amount.
 * Returns:
 *    double - Seat amount
 */
double Seat::getSeatAmount()
{
    return m_amount;
}

/*
 * Function: Seat::getSeatStatus
 * Description: Retrieves the seat status code.
 * Returns:
 *    enum - Seat status
 */
Enums::SeatStatus Seat::getSeatStatus() const
{
    return m_seatStatus;
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
 * Function: Seat::setSeatStatus
 * Description: Sets the seat status code.
 * Parameters:
 *    int seatStatus - New seat status
 * Returns:
 *    void
 */
void Seat::setSeatStatus(Enums::SeatStatus seatStatus)
{
    m_seatStatus = seatStatus;
}

/*
 * Function: setSeatAmount
 * Description: Sets the seat amount.
 * Parameters:
 *    int amount - New booking amount
 * Returns:
 *    void
 */
void Seat::setSeatAmount(double amount)
{
    m_amount = amount;
}
