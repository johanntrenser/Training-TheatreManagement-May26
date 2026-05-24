/*
 * File: Seat.h
 * Description: Defines the Seat class, which represents an individual seat
 *              in a cinema screen. Stores details such as seat ID, screen,
 *              row, column, type, status, and booking state.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include <string>
#include "Enums.h"

class Screen;

class Seat
{
private:
    std::string m_seatId;
    Screen* m_screen;
    char m_seatRow;
    int m_seatColumn;
    Enums::SeatStatus m_seatStatus; 
    Enums::BookingStatus m_bookingStatus;
public:
    Seat();
    Seat(const std::string& seatId,
        Screen* screen,
        char seatRow,
        int seatColumn,
        Enums::SeatStatus seatStatus,
        Enums::BookingStatus bookingStatus);
    const std::string& getSeatId() const;
    Screen* getScreen() const;
    char getSeatRow() const;
    int getSeatColumn() const;
    Enums::SeatStatus getSeatStatus() const;
    Enums::BookingStatus getBookingStatus() const;
    void setSeatId(const std::string& id);
    void setScreen(Screen* screen);
    void setSeatRow(char seatRow);
    void setSeatColumn(int seatColumn);
    void setSeatStatus(Enums::SeatStatus seatStatus);
    void setBookingStatus(Enums::BookingStatus bookingStatus);
};