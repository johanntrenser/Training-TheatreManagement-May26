/*
 * File: Booking.h
 * Description: Implements the Booking class, providing constructors, getters,
 *              and setters for booking attributes such as booking ID, customer,
 *              show, booked seats, status, and amount.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "ApplicationConfig.h"
#include "User.h"
#include "Show.h"
#include "Seat.h"
#include "Factory.h"
#include "InputHelper.h"

class Booking
{
private:
    std::string m_bookingId;
    User* m_customer;
    Show* m_show;
    std::vector<Seat*> m_bookedSeats;
    Enums::BookingStatus m_status; 
    double m_amount;
public:
    Booking();
    Booking(const std::string& bookingId,
        User* customer,
        Show* show,
        const std::vector<Seat*>& bookedSeats,
        Enums::BookingStatus status,
        double amount);
    const std::string& getBookingId() const;
    User* getCustomer() const;
    Show* getShow() const;
    const std::vector<Seat*>& getBookedSeats() const;
    Enums::BookingStatus getStatus() const;
    double getAmount() const;
    void setBookingId(const std::string& id);
    void setCustomer(User* customer);
    void setShow(Show* show);
    void setBookedSeats(const std::vector<Seat*>& bookedSeats);
    void setStatus(Enums::BookingStatus status);
    void setAmount(double amount);
    std::string serialize();
    static Booking* deserialize(const std::string& lines);
};



