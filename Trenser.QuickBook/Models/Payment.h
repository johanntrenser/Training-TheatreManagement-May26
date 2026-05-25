/*
 * File: Payment.h
 * Description: Defines the Payment class, which represents a payment
 *              transaction for a booking. Stores details such as payment ID,
 *              associated booking, amount, payment method, status, and timestamp.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include <string>
#include "Booking.h"

class Payment
{
private:
    std::string m_paymentId;
    Booking* m_booking;
    double m_amount;
    Enums::PaymentMethod m_paymentMethod; 
    Enums::PaymentStatus m_status;
    std::string m_timeStamp;
public:
    Payment();
    Payment(const std::string& paymentId,
        Booking* booking,
        double amount,
        Enums::PaymentMethod paymentMethod,
        const std::string& timeStamp);
    const std::string& getPaymentId() const;
    Booking* getBooking() const;
    double getAmount() const;
    Enums::PaymentMethod getPaymentMethod() const;
    Enums::PaymentStatus getStatus() const;
    const std::string& getTimeStamp() const;
    void setPaymentId(const std::string& paymentId);
    void setBooking(Booking* booking);
    void setAmount(double amount);
    void setPaymentMethod(Enums::PaymentMethod paymentMethod);
    void setStatus(Enums::PaymentStatus status);
    void setTimeStamp(const std::string& timeStamp);
};

