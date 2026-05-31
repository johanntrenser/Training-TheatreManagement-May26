/*
 * File: Screen.h
 * Description: Defines the Screen class, which represents a cinema screen
 *              inside a theatre. Stores details such as screen ID, theatre,
 *              name, seating grid, and status.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include <string>
#include <vector>
#include "Enums.h"
#include "ApplicationConfig.h"

class Theatre;
class Seat;

class Screen
{
private:
    std::string m_screenId;
    Theatre* m_theatre;
    std::string m_name;
    int m_totalRows;
    int m_totalColumns;
    std::vector<std::vector<Seat*>> m_seatGrid;
    Enums::ScreenStatus m_screenStatus;
public:
    Screen();
    Screen(const std::string& screenId,
        Theatre* theatre,
        const std::string& name,
        int totalRows,
        int totalColumns,
        const std::vector<std::vector<Seat*>>& seatGrid);
    const std::string& getScreenId() const;
    Theatre* getTheatre() const;
    const std::string& getName() const;
    int getTotalRows() const;
    int getTotalColumns() const;
    const std::vector<std::vector<Seat*>>& getSeatGrid() const;
    Enums::ScreenStatus getScreenStatus() const;
    void setScreenId(const std::string& screenId);
    void setTheatre(Theatre* theatre);
    void setName(const std::string& name);
    void setTotalRows(int totalRows);
    void setTotalColumns(int totalColumns);
    void setSeatGrid(const std::vector<std::vector<Seat*>>& seatGrid);
    void setScreenStatus(Enums::ScreenStatus screenStatus);
    std::string serialize() const;
};