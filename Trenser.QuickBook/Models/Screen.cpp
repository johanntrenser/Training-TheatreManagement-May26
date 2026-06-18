/*
 * File: Screen.cpp
 * Description: Implements the Screen class, which represents a theatre screen.
 *              Stores details such as screen ID, associated theatre, name,
 *              seating grid, dimensions, and screen status.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#include "Screen.h"
#include "Theatre.h"
#include "Seat.h"

 /*
  * Function: Screen::Screen
  * Description: Default constructor initializing screen with default values.
  * Returns:
  *    Screen object
  */
Screen::Screen()
    : m_screenId(""),
    m_theatre(nullptr),
    m_name(""),
    m_totalRows(0),
    m_totalColumns(0),
    m_seatGrid(),
    m_screenStatus(Enums::ScreenStatus::AVAILABLE)
{
}

/*
 * Function: Screen::Screen
 * Description: Parameterized constructor initializing screen details.
 * Parameters:
 *    const std::string& screenId - Unique screen identifier
 *    Theatre* theatre - Associated theatre object
 *    const std::string& name - Screen name
 *    int totalRows - Total number of seat rows
 *    int totalColumns - Total number of seat columns
 *    const std::vector<std::vector<Seat*>>& seatGrid - Seating grid
 *    int screenStatus - Screen status code
 * Returns:
 *    Screen object
 */
Screen::Screen(const std::string& screenId,
    Theatre* theatre,
    const std::string& name,
    int totalRows,
    int totalColumns,
    const std::vector<std::vector<Seat*>>& seatGrid)
    : m_screenId(screenId),
    m_theatre(theatre),
    m_name(name),
    m_totalRows(totalRows),
    m_totalColumns(totalColumns),
    m_seatGrid(seatGrid),
    m_screenStatus(Enums::ScreenStatus::AVAILABLE)
{
}

/*
 * Function: Screen::getScreenId
 * Description: Retrieves the unique screen ID.
 * Returns:
 *    const std::string& - Screen ID
 */
const std::string& Screen::getScreenId() const
{
    return m_screenId;
}

/*
 * Function: Screen::getTheatre
 * Description: Retrieves the associated theatre object.
 * Returns:
 *    Theatre* - Pointer to the theatre
 */
const Theatre* Screen::getTheatre() const
{
    return m_theatre;
}

/*
 * Function: Screen::getName
 * Description: Retrieves the screen name.
 * Returns:
 *    const std::string& - Screen name
 */
const std::string& Screen::getName() const
{
    return m_name;
}

/*
 * Function: Screen::getTotalRows
 * Description: Retrieves the total number of seat rows.
 * Returns:
 *    int - Total rows
 */
int Screen::getTotalRows() const
{
    return m_totalRows;
}

/*
 * Function: Screen::getTotalColumns
 * Description: Retrieves the total number of seat columns.
 * Returns:
 *    int - Total columns
 */
int Screen::getTotalColumns() const
{
    return m_totalColumns;
}

/*
 * Function: Screen::getSeatGrid
 * Description: Retrieves the seating grid.
 * Returns:
 *    const std::vector<std::vector<Seat*>>& - Seating grid
 */
const std::vector<std::vector<Seat*>>& Screen::getSeatGrid() const
{
    return m_seatGrid;
}

/*
 * Function: Screen::getScreenStatus
 * Description: Retrieves the screen status code.
 * Returns:
 *    int - Screen status
 */
Enums::ScreenStatus Screen::getScreenStatus() const
{
    return m_screenStatus;
}

/*
 * Function: Screen::setScreenId
 * Description: Sets the unique screen ID.
 * Parameters:
 *    const std::string& screenId - New screen ID
 * Returns:
 *    void
 */
void Screen::setScreenId(const std::string& screenId)
{
    m_screenId = screenId;
}

/*
 * Function: Screen::setTheatre
 * Description: Sets the associated theatre object.
 * Parameters:
 *    Theatre* theatre - Pointer to the theatre
 * Returns:
 *    void
 */
void Screen::setTheatre(Theatre* theatre)
{
    m_theatre = theatre;
}

/*
 * Function: Screen::setName
 * Description: Sets the screen name.
 * Parameters:
 *    const std::string& name - New screen name
 * Returns:
 *    void
 */
void Screen::setName(const std::string& name)
{
    m_name = name;
}

/*
 * Function: Screen::setTotalRows
 * Description: Sets the total number of seat rows.
 * Parameters:
 *    int totalRows - New total rows
 * Returns:
 *    void
 */
void Screen::setTotalRows(int totalRows)
{
    m_totalRows = totalRows;
}

/*
 * Function: Screen::setTotalColumns
 * Description: Sets the total number of seat columns.
 * Parameters:
 *    int totalColumns - New total columns
 * Returns:
 *    void
 */
void Screen::setTotalColumns(int totalColumns)
{
    m_totalColumns = totalColumns;
}

/*
 * Function: Screen::setSeatGrid
 * Description: Sets the seating grid.
 * Parameters:
 *    const std::vector<std::vector<Seat*>>& seatGrid - New seating grid
 * Returns:
 *    void
 */
void Screen::setSeatGrid(const std::vector<std::vector<Seat*>>& seatGrid)
{
    m_seatGrid = seatGrid;
}

/*
 * Function: Screen::setScreenStatus
 * Description: Sets the screen status code.
 * Parameters:
 *    int screenStatus - New screen status
 * Returns:
 *    void
 */
void Screen::setScreenStatus(Enums::ScreenStatus screenStatus)
{
    m_screenStatus = screenStatus;
}

/*
 * Function: getSeatGridForUpdation
 * Description: Provides reference access to the screen’s seat grid for updates.
 * Parameters:
 *    None
 * Returns:
 *    Reference to 2D vector of Seat pointers
 */
std::vector<std::vector<Seat*>>& Screen::getSeatGridForUpdation()
{
    return m_seatGrid;
}

/*
 * Function: serialize
 * Description: Converts a Screen object into a SharedScreen struct suitable
 *              for storage in a memory-mapped file. Includes screen metadata
 *              and seat identifiers up to SEAT_MAX_COUNT.
 * Parameters:
 *    None
 * Returns:
 *    SharedScreen struct representation of this Screen
 */
void Screen::serialize(SharedScreen& sharedScreen) const
{
    strncpy_s(sharedScreen.screenId, sizeof(sharedScreen.screenId), m_screenId.c_str(), _TRUNCATE);
    strncpy_s(sharedScreen.theatreId, sizeof(sharedScreen.theatreId),
        (m_theatre ? m_theatre->getTheatreId().c_str() : ""), _TRUNCATE);
    strncpy_s(sharedScreen.name, sizeof(sharedScreen.name), m_name.c_str(), _TRUNCATE);
    sharedScreen.totalRows = m_totalRows;
    sharedScreen.totalColumns = m_totalColumns;
    sharedScreen.status = static_cast<int>(m_screenStatus);
    sharedScreen.seatCount = 0;
    for (const std::vector<Seat*>& row : m_seatGrid)
    {
        for (const Seat* seat : row)
        {
            if (sharedScreen.seatCount < config::Limit::SEAT_MAX_COUNT)
            {
                strncpy_s(sharedScreen.seatIds[sharedScreen.seatCount],
                    sizeof(sharedScreen.seatIds[0]),
                    seat->getSeatId().c_str(),
                    _TRUNCATE);
                sharedScreen.seatCount++;
            }
        }
    }
}

/*
 * Function: deserialize
 * Description: Reconstructs a Screen object from a SharedScreen struct.
 *              Initializes basic screen attributes and sets status.
 * Parameters:
 *    sharedScreen - Pointer to a SharedScreen struct containing serialized screen data
 * Returns:
 *    Pointer to a newly created Screen object, or nullptr if input is invalid
 */
Screen* Screen::deserialize(const SharedScreen* sharedScreen)
{
    if (sharedScreen == nullptr)
    {
        return nullptr;
    }
    std::vector<std::vector<Seat*>> emptyGrid;
    Screen* screen = Factory::getObject<Screen>(
        sharedScreen->screenId,
        nullptr,
        sharedScreen->name,
        sharedScreen->totalRows,
        sharedScreen->totalColumns,
        emptyGrid);
    if (screen != nullptr)
    {
        screen->setScreenStatus(static_cast<Enums::ScreenStatus>(sharedScreen->status));
    }
    return screen;
}