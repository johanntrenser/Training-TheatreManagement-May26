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

class Screen;

class Seat
{
private:
    std::string m_seatId;
    Screen* m_screen;
    char m_seatRow;
    int m_seatColumn;
    int m_seatType;       // enum placeholder
    int m_seatStatus;     // enum placeholder
    int m_bookingStatus;  // enum placeholder
public:
    Seat();
    Seat(const std::string& seatId,
        Screen* screen,
        char seatRow,
        int seatColumn,
        int seatType,
        int seatStatus,
        int bookingStatus);
    const std::string& getSeatId() const;
    Screen* getScreen() const;
    char getSeatRow() const;
    int getSeatColumn() const;
    int getSeatType() const;
    int getSeatStatus() const;
    int getBookingStatus() const;
    void setSeatId(const std::string& id);
    void setScreen(Screen* screen);
    void setSeatRow(char seatRow);
    void setSeatColumn(int seatColumn);
    void setSeatType(int seatType);
    void setSeatStatus(int seatStatus);
    void setBookingStatus(int bookingStatus);
};