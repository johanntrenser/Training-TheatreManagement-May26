/*
 * File: Refund.cpp
 * Description: Implements the Refund class, which represents a refund
 *              transaction for a booked ticket. Stores details such as
 *              refund ID, associated ticket, refund amount, time, and status.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#include "Refund.h"

 /*
  * Function: Refund::Refund
  * Description: Default constructor initializing refund with default values.
  * Returns:
  *    Refund object
  */
Refund::Refund()
    : m_refundId(""),
    m_bookedTicket(nullptr),
    m_refundAmount(0.0),
    m_time(""),
    m_status(0)
{}

/*
 * Function: Refund::Refund
 * Description: Parameterized constructor initializing refund details.
 * Parameters:
 *    const std::string& id - Unique refund identifier
 *    Ticket* bookedTicket - Associated ticket object
 *    double refundAmount - Refund amount
 *    const std::string& time - Refund timestamp
 *    int status - Refund status code
 * Returns:
 *    Refund object
 */
Refund::Refund(const std::string& id,
    Ticket* bookedTicket,
    double refundAmount,
    const std::string& time,
    int status)
    : m_refundId(id),
    m_bookedTicket(bookedTicket),
    m_refundAmount(refundAmount),
    m_time(time),
    m_status(status)
{}

/*
 * Function: Refund::getRefundId
 * Description: Retrieves the unique refund ID.
 * Returns:
 *    const std::string& - Refund ID
 */
const std::string& Refund::getRefundId() const
{
    return m_refundId;
}

/*
 * Function: Refund::getBookedTicket
 * Description: Retrieves the associated ticket object.
 * Returns:
 *    Ticket* - Pointer to the booked ticket
 */
Ticket* Refund::getBookedTicket() const
{
    return m_bookedTicket;
}

/*
 * Function: Refund::getRefundAmount
 * Description: Retrieves the refund amount.
 * Returns:
 *    double - Refund amount
 */
double Refund::getRefundAmount() const
{
    return m_refundAmount;
}

/*
 * Function: Refund::getTime
 * Description: Retrieves the refund timestamp.
 * Returns:
 *    const std::string& - Refund timestamp
 */
const std::string& Refund::getTime() const
{
    return m_time;
}

/*
 * Function: Refund::getStatus
 * Description: Retrieves the refund status code.
 * Returns:
 *    int - Refund status
 */
int Refund::getStatus() const
{
    return m_status;
}

/*
 * Function: Refund::setRefundId
 * Description: Sets the unique refund ID.
 * Parameters:
 *    const std::string& id - New refund ID
 * Returns:
 *    void
 */
void Refund::setRefundId(const std::string& id)
{
    m_refundId = id;
}

/*
 * Function: Refund::setBookedTicket
 * Description: Sets the associated ticket object.
 * Parameters:
 *    Ticket* bookedTicket - Pointer to the booked ticket
 * Returns:
 *    void
 */
void Refund::setBookedTicket(Ticket* bookedTicket)
{
    m_bookedTicket = bookedTicket;
}

/*
 * Function: Refund::setRefundAmount
 * Description: Sets the refund amount.
 * Parameters:
 *    double refundAmount - New refund amount
 * Returns:
 *    void
 */
void Refund::setRefundAmount(double refundAmount)
{
    m_refundAmount = refundAmount;
}

/*
 * Function: Refund::setTime
 * Description: Sets the refund timestamp.
 * Parameters:
 *    const std::string& time - New refund timestamp
 * Returns:
 *    void
 */
void Refund::setTime(const std::string& time)
{
    m_time = time;
}

/*
 * Function: Refund::setStatus
 * Description: Sets the refund status code.
 * Parameters:
 *    int status - New refund status
 * Returns:
 *    void
 */
void Refund::setStatus(int status)
{
    m_status = status;
}
