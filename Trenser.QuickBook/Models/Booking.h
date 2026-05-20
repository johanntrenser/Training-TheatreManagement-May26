/*
 * File: Booking.h
 * Description: Implements the Booking class, providing constructors, getters,
 *              and setters for booking attributes such as booking ID, customer,
 *              show, booked seats, status, and amount.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "Show.h"
#include "Seat.h"

class Booking
{
private:
    std::string m_bookingId;
    User* m_customer;
    Show* m_show;
    std::vector<Seat*> m_bookedSeats;
    int m_status; // enum placeholder
    int m_amount;
public:
    Booking();
    Booking(const std::string& bookingId,
        User* customer,
        Show* show,
        const std::vector<Seat*>& bookedSeats,
        int status,
        int amount);
    const std::string& getBookingId() const;
    User* getCustomer() const;
    Show* getShow() const;
    const std::vector<Seat*>& getBookedSeats() const;
    int getStatus() const;
    int getAmount() const;
    void setBookingId(const std::string& id);
    void setCustomer(User* customer);
    void setShow(Show* show);
    void setBookedSeats(const std::vector<Seat*>& bookedSeats);
    void setStatus(int status);
    void setAmount(int amount);
};



