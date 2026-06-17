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
    m_timeStamp(std::time(nullptr))
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
    time_t timeStamp)
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
 *    time_t - Payment timestamp
 */
const time_t Payment::getTimeStamp() const
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
 *    const time_t timeStamp - New payment timestamp
 * Returns:
 *    void
 */
void Payment::setTimeStamp(const time_t timeStamp)
{
    m_timeStamp = timeStamp;
}

/*
 * Function: Payment::serialize
 * Description: Serializes the Payment object into a SharedPayment structure
 *              for use in shared memory or inter-process communication.
 *              Copies payment details such as payment ID, booking ID, customer ID,
 *              amount, payment method, status, and timestamp into the provided
 *              SharedPayment reference using safe string operations.
 * Parameters:
 *    sharedPayment - Reference to a SharedPayment structure that will be populated
 *                    with the serialized payment data.
 * Returns: None
 */
void Payment::serialize(SharedPayment& sharedPayment) const
{
    sharedPayment = {};
    strncpy_s(sharedPayment.paymentId, m_paymentId.c_str(), sizeof(sharedPayment.paymentId));
    strncpy_s(sharedPayment.bookingId, 
        (m_booking ? m_booking->getBookingId().c_str() : ""), sizeof(sharedPayment.bookingId));
    strncpy_s(sharedPayment.customerId, 
        (m_booking && m_booking->getCustomer() ? m_booking->getCustomer()->getUserId().c_str() : ""),
        sizeof(sharedPayment.customerId));
    sharedPayment.amount = m_amount;
    sharedPayment.paymentMethod = static_cast<int>(m_paymentMethod);
    sharedPayment.status = static_cast<int>(m_status);
    strncpy_s(sharedPayment.time, util::serializeTime(m_timeStamp).c_str(), sizeof(sharedPayment.time));
}

/*
 * Function: Payment::deserialize
 * Description: Deserializes a SharedPayment structure into a Payment object.
 *              Converts serialized fields such as payment ID, amount, payment method,
 *              status, and timestamp back into a Payment instance. Uses the Factory
 *              to create the Payment object and applies the stored status.
 * Parameters:
 *    sharedPayment - Pointer to a SharedPayment structure containing serialized payment data.
 * Returns:
 *    Pointer to a newly created Payment object if deserialization succeeds.
 *    nullptr if the provided SharedPayment pointer is null.
 */
Payment* Payment::deserialize(const SharedPayment* sharedPayment)
{
    if (sharedPayment == nullptr)
    {
        return nullptr;
    }
    time_t timeStamp = util::deserializeTime(sharedPayment->time);
    Payment* payment = Factory::getObject<Payment>(
        sharedPayment->paymentId,
        nullptr,
        sharedPayment->amount,
        static_cast<Enums::PaymentMethod>(sharedPayment->paymentMethod),
        timeStamp);
    if (payment != nullptr)
    {
        payment->setStatus(static_cast<Enums::PaymentStatus>(sharedPayment->status));
    }
    return payment;
}