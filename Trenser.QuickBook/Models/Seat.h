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
#include <sstream>
#include "ApplicationConfig.h"
#include "Enums.h"
#include "Factory.h"
#include "InputHelper.h"

class Screen;

class Seat
{
private:
    std::string m_seatId;
    Screen* m_screen;
    char m_seatRow;
    int m_seatColumn;
    double m_amount;
    Enums::SeatStatus m_seatStatus;
public:
    Seat();
    Seat(const std::string& seatId,
        Screen* screen,
        char seatRow,
        int seatColumn,
        double m_amount,
        Enums::SeatStatus seatStatus);
    const std::string& getSeatId() const;
    Screen* getScreen() const;
    char getSeatRow() const;
    int getSeatColumn() const;
    double getSeatAmount();
    Enums::SeatStatus getSeatStatus() const;
    void setSeatId(const std::string& id);
    void setScreen(Screen* screen);
    void setSeatRow(char seatRow);
    void setSeatColumn(int seatColumn);
    void setSeatStatus(Enums::SeatStatus seatStatus);
    void setSeatAmount(double amount);
    std::string serialize();
    static Seat* deserialize(std::string& line);
};