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
    m_time(-1),
    m_status(Enums::RefundStatus::COMPLETED)
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
    time_t time)
    : m_refundId(id),
    m_bookedTicket(bookedTicket),
    m_refundAmount(refundAmount),
    m_time(time),
    m_status(Enums::RefundStatus::COMPLETED)
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
time_t Refund::getTime() const
{
    return m_time;
}

/*
 * Function: Refund::getStatus
 * Description: Retrieves the refund status code.
 * Returns:
 *    enum - Refund status
 */
Enums::RefundStatus Refund::getStatus() const
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
void Refund::setTime(time_t time)
{
    m_time = time;
}

/*
 * Function: Refund::setStatus
 * Description: Sets the refund status code.
 * Parameters:
 *    enum status - New refund status
 * Returns:
 *    void
 */
void Refund::setStatus(Enums::RefundStatus status)
{
    m_status = status;
}

/*
 * Function: serialize
 * Description: Converts Refund object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string Refund::serialize()
{
    std::string result = m_refundId + config::delimeter::comma;
    if (m_bookedTicket)
    {
        result += m_bookedTicket->getTicketId() + config::delimeter::comma;
    }
    else
    {
        result += config::delimeter::comma;
    }
    result += std::to_string(m_refundAmount) + config::delimeter::comma +
        util::serializeTime(m_time) + config::delimeter::comma +
        Enums::getRefundStatusString(m_status);
    return result;
}

/*
 * Function: Refund::deserialize
 * Description: Deserializes a single line of CSV-formatted refund data into a Refund object.
 *              Extracts fields such as Refund ID, Booked Ticket ID, Refund Amount, Time, and Status.
 *              Parses the time string into its components (year, month, day, hour, minute) and converts
 *              it into a time_t object using util::createTime. The Booked Ticket pointer is set to nullptr
 *              initially and can be linked later when the Ticket object is available in the DataStore.
 * Parameters:
 *    lines - A reference to a string containing one line of CSV refund data.
 * Returns:
 *    A pointer to a newly created Refund object populated with the deserialized data.
 */
Refund* Refund::deserialize(const std::string& lines)
{
    std::string refundId, bookedTicketId, refundAmount, time, status, year, dash, space, month, day, hour, colon, minute;
    std::stringstream lineStream(lines);
    getline(lineStream, refundId, ',');
    getline(lineStream, bookedTicketId, ',');
    getline(lineStream, refundAmount, ',');
    getline(lineStream, time, ',');
    getline(lineStream, status, ',');
    time_t convertedTime = util::deserializeTime(time);
    util::trimWhitespace(refundAmount);
    Refund* refund = Factory::getObject<Refund>(refundId, nullptr, stod(refundAmount), convertedTime);
    return refund;
}