#include "Screen.h"
#include "Theatre.h"
#include "Seat.h"

Screen::Screen()
    : m_screenId(""),
    m_theatre(nullptr),
    m_name(""),
    m_totalRows(0),
    m_totalColumns(0),
    m_seatGrid(),
    m_screenStatus(0)
{}

Screen::Screen(const std::string& screenId,
    Theatre* theatre,
    const std::string& name,
    int totalRows,
    int totalColumns,
    const std::vector<std::vector<Seat*>>& seatGrid,
    int screenStatus)
    : m_screenId(screenId),
    m_theatre(theatre),
    m_name(name),
    m_totalRows(totalRows),
    m_totalColumns(totalColumns),
    m_seatGrid(seatGrid),
    m_screenStatus(screenStatus)
{}

const std::string& Screen::getScreenId() const
{
    return m_screenId;
}

Theatre* Screen::getTheatre() const
{
    return m_theatre;
}

const std::string& Screen::getName() const
{
    return m_name;
}

int Screen::getTotalRows() const
{
    return m_totalRows;
}

int Screen::getTotalColumns() const
{
    return m_totalColumns;
}

const std::vector<std::vector<Seat*>>& Screen::getSeatGrid() const
{
    return m_seatGrid;
}

int Screen::getScreenStatus() const
{
    return m_screenStatus;
}

void Screen::setScreenId(const std::string& screenId)
{
    m_screenId = screenId;
}

void Screen::setTheatre(Theatre* theatre)
{
    m_theatre = theatre;
}

void Screen::setName(const std::string& name)
{
    m_name = name;
}

void Screen::setTotalRows(int totalRows)
{
    m_totalRows = totalRows;
}

void Screen::setTotalColumns(int totalColumns)
{
    m_totalColumns = totalColumns;
}

void Screen::setSeatGrid(const std::vector<std::vector<Seat*>>& seatGrid)
{
    m_seatGrid = seatGrid;
}

void Screen::setScreenStatus(int screenStatus)
{
    m_screenStatus = screenStatus;
}
