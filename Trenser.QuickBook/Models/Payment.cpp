/*
 * File: Payment.cpp
 * Description: Implements the Payment class, which represents a payment
 *              transaction for a booking. Stores details such as payment ID,
 *              associated booking, amount, method, status, and timestamp.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#include "Payment.h"

 /*
  * Function: Payment::Payment
  * Description: Default constructor initializing payment with default values.
  * Returns:
  *    Payment object
  */
Payment::Payment()
    : m_paymentId(""),
    m_booking(nullptr),
    m_amount(0.0),
    m_paymentMethod(Enums::PaymentMethod::UPI),
    m_status(Enums::PaymentStatus::PENDING),
    m_timeStamp("")
{}

/*
 * Function: Payment::Payment
 * Description: Parameterized constructor initializing payment details.
 * Parameters:
 *    const std::string& paymentId - Unique payment identifier
 *    Booking* booking - Associated booking object
 *    double amount - Payment amount
 *    Enums::PaymentMethod paymentMethod - Method of payment
 *    const std::string& timeStamp - Payment timestamp
 * Returns:
 *    Payment object
 */
Payment::Payment(const std::string& paymentId,
    Booking* booking,
    double amount,
    Enums::PaymentMethod paymentMethod,
    const std::string& timeStamp)
    : m_paymentId(paymentId),
    m_booking(booking),
    m_amount(amount),
    m_paymentMethod(paymentMethod),
    m_status(Enums::PaymentStatus::PENDING),
    m_timeStamp(timeStamp)
{}

/*
 * Function: Payment::getPaymentId
 * Description: Retrieves the unique payment ID.
 * Returns:
 *    const std::string& - Payment ID
 */
const std::string& Payment::getPaymentId() const
{
    return m_paymentId;
}

/*
 * Function: Payment::getBooking
 * Description: Retrieves the associated booking object.
 * Returns:
 *    Booking* - Pointer to the booking
 */
Booking* Payment::getBooking() const
{
    return m_booking;
}

/*
 * Function: Payment::getAmount
 * Description: Retrieves the payment amount.
 * Returns:
 *    double - Payment amount
 */
double Payment::getAmount() const
{
    return m_amount;
}

/*
 * Function: Payment::getPaymentMethod
 * Description: Retrieves the method of payment.
 * Returns:
 *    Enums::PaymentMethod - Payment method
 */
Enums::PaymentMethod Payment::getPaymentMethod() const
{
    return m_paymentMethod;
}

/*
 * Function: Payment::getStatus
 * Description: Retrieves the current payment status.
 * Returns:
 *    Enums::PaymentStatus - Payment status
 */
Enums::PaymentStatus Payment::getStatus() const
{
    return m_status;
}

/*
 * Function: Payment::getTimeStamp
 * Description: Retrieves the timestamp of the payment.
 * Returns:
 *    const std::string& - Payment timestamp
 */
const std::string& Payment::getTimeStamp() const
{
    return m_timeStamp;
}

/*
 * Function: Payment::setPaymentId
 * Description: Sets the unique payment ID.
 * Parameters:
 *    const std::string& paymentId - New payment ID
 * Returns:
 *    void
 */
void Payment::setPaymentId(const std::string& paymentId)
{
    m_paymentId = paymentId;
}

/*
 * Function: Payment::setBooking
 * Description: Sets the associated booking object.
 * Parameters:
 *    Booking* booking - Pointer to the booking
 * Returns:
 *    void
 */
void Payment::setBooking(Booking* booking)
{
    m_booking = booking;
}

/*
 * Function: Payment::setAmount
 * Description: Sets the payment amount.
 * Parameters:
 *    double amount - New payment amount
 * Returns:
 *    void
 */
void Payment::setAmount(double amount)
{
    m_amount = amount;
}

/*
 * Function: Payment::setPaymentMethod
 * Description: Sets the method of payment.
 * Parameters:
 *    Enums::PaymentMethod paymentMethod - New payment method
 * Returns:
 *    void
 */
void Payment::setPaymentMethod(Enums::PaymentMethod paymentMethod)
{
    m_paymentMethod = paymentMethod;
}

/*
 * Function: Payment::setStatus
 * Description: Sets the current payment status.
 * Parameters:
 *    Enums::PaymentStatus status - New payment status
 * Returns:
 *    void
 */
void Payment::setStatus(Enums::PaymentStatus status)
{
    m_status = status;
}

/*
 * Function: Payment::setTimeStamp
 * Description: Sets the timestamp of the payment.
 * Parameters:
 *    const std::string& timeStamp - New payment timestamp
 * Returns:
 *    void
 */
void Payment::setTimeStamp(const std::string& timeStamp)
{
    m_timeStamp = timeStamp;
}

/*
 * Function: serialize
 * Description: Converts Payment object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string Payment::serialize()
{
    std::string result = m_paymentId + config::delimeter::comma;
    if (m_booking)
    {
        result += m_booking->getBookingId()+config::delimeter::comma;
    }
    result += std::to_string(m_amount) + config::delimeter::comma +
        Enums::getPaymentMethodString(m_paymentMethod) + config::delimeter::comma +
        Enums::getPaymentStatusString(m_status) + config::delimeter::comma +
        m_timeStamp;
    return result;
}