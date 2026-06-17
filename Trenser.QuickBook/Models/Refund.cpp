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
 * Function: Refund::serialize
 * Description: Serializes the Refund object into a SharedRefund structure
 *              for use in shared memory or inter-process communication.
 *              Copies refund details such as refund ID, payment ID, customer ID,
 *              refund amount, status, and timestamp into the provided
 *              SharedRefund reference using safe string operations.
 * Parameters:
 *    sharedRefund - Reference to a SharedRefund structure that will be populated
 *                   with the serialized refund data.
 * Returns: None
 */
void Refund::serialize(SharedRefund& sharedRefund) const
{
    sharedRefund = {};
    strncpy_s(sharedRefund.refundId, m_refundId.c_str(), sizeof(sharedRefund.refundId));
    strncpy_s(sharedRefund.paymentId, 
        (m_bookedTicket && m_bookedTicket->getPayment() ? m_bookedTicket->getPayment()->getPaymentId().c_str() : ""),
        sizeof(sharedRefund.paymentId));
    strncpy_s(sharedRefund.customerId,
        (m_bookedTicket && m_bookedTicket->getCustomer()? m_bookedTicket->getCustomer()->getUserId().c_str() : ""), 
        sizeof(sharedRefund.customerId));
    sharedRefund.amount = m_refundAmount;
    sharedRefund.status = static_cast<int>(m_status);
    strncpy_s(sharedRefund.time, util::serializeTime(m_time).c_str(), sizeof(sharedRefund.time));
}

/*
 * Function: Refund::deserialize
 * Description: Deserializes a SharedRefund structure into a Refund object.
 *              Converts serialized fields such as refund ID, amount, status,
 *              and timestamp back into a Refund instance. Uses the Factory
 *              to create the Refund object and applies the stored status.
 * Parameters:
 *    sharedRefund - Pointer to a SharedRefund structure containing serialized refund data.
 * Returns:
 *    Pointer to a newly created Refund object if deserialization succeeds.
 *    nullptr if the provided SharedRefund pointer is null.
 */
Refund* Refund::deserialize(const SharedRefund* sharedRefund)
{
    if (sharedRefund == nullptr)
    {
        return nullptr;
    }
    time_t convertedTime = util::deserializeTime(sharedRefund->time);
    Refund* refund = Factory::getObject<Refund>(
        sharedRefund->refundId,
        nullptr,
        sharedRefund->amount,
        convertedTime);
    if (refund != nullptr)
    {
        refund->setStatus(static_cast<Enums::RefundStatus>(sharedRefund->status));
    }
    return refund;
}