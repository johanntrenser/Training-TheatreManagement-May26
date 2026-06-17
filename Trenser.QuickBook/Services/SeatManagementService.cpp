/*
 * File: SeatManagementService.cpp
 * Description: Declares the SeatManagementService class, which provides
 *              functionality for managing seats within the system.
 *              This service will handle operations such as adding,
 *              updating, retrieving, and deleting seat records, as well
 *              as managing seat availability for shows.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <vector>
#include <iomanip>
#include "SeatManagementService.h"
#include "Factory.h"
#include "ApplicationConfig.h"

 /*
 * Function: SeatManagementService
 * Description: Constructs a SeatManagementService instance. Initializes the
 *              reference to the singleton DataStore and sets up a mutex for
 *              thread-safe seat operations using the configured mutex name.
 * Parameters: None
 * Returns: None
 */
SeatManagementService::SeatManagementService()
    : m_dataStore(DataStore::getInstance()),
    m_mutex(config::MutexMappings::SEAT_MUTEX_NAME)
{
}

/*
Function Name : generateSeatId
Description   : Generates a unique ID for a new seat.
Parameters    : None
Return Type   : std::string
*/
std::string SeatManagementService::generateSeatId()
{
    const int count = m_dataStore.getSeatCount();
    int idNumber = count + 1;
    std::ostringstream buffer;
    buffer << "ST" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: updateSeatLayout
 * Description: Rebuilds the seating grid for a screen with given rows and columns.
 * Parameters:
 *    screen  - Target screen
 *    rows    - Number of rows
 *    columns - Number of columns
 * Returns:
 *    ProcessStatus::SUCCESS if layout updated, FAILED otherwise
 */
Enums::ProcessStatus SeatManagementService::updateSeatLayout(const std::string& selectedScreenId, int rows, int columns, double amount)
{
    ScopedLock lock(m_mutex);
    Screen* screen = m_dataStore.getScreenById(selectedScreenId);
    if (!screen)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::vector<std::vector<Seat*>>& seatGrid = screen->getSeatGridForUpdation();
    clearSeatGrid(seatGrid);
    std::vector<std::vector<Seat*>> newSeatGrid;
    for (int row = 0; row < rows; ++row)
    {
        char rowCharacter = 'A' + row;
        std::vector<Seat*> seatRow;
        for (int column = 0; column < columns; ++column)
        {
            std::string seatId = generateSeatId();
            Seat* seat = Factory::getObject<Seat>(seatId, screen, rowCharacter, column, amount, Enums::SeatStatus::AVAILABLE);
            if (!seat)
            {
                return Enums::ProcessStatus::FAILED;
            }
            m_dataStore.addSeat(seat);
            seatRow.push_back(seat);
        }
        if (seatRow.empty())
        {
            return Enums::ProcessStatus::FAILED;
        }
        newSeatGrid.push_back(seatRow);
    }
    if (!newSeatGrid.empty())
    {
        seatGrid.clear();
        screen->setSeatGrid(newSeatGrid);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
Function Name : clearSeatGrid
Description   : Deletes all seats in the seat grid and clears memory.
Parameters    :
                 seatGrid - Seat grid to be cleared
Return Type   : void
*/
void SeatManagementService::clearSeatGrid(std::vector<std::vector<Seat*>>& seatGrid)
{
    for (std::vector<std::vector<Seat*>>::iterator rowIterator = seatGrid.begin(); rowIterator != seatGrid.end(); ++rowIterator)
    {
        for (std::vector<Seat*>::iterator seatIterator = (*rowIterator).begin(); seatIterator != (*rowIterator).end(); ++seatIterator)
        {
            (*seatIterator)->setSeatStatus(Enums::SeatStatus::BLOCKED);
        }
    }
    seatGrid.clear();
}

/*
 * Function: getSeatLayout
 * Description: Returns the seating grid of a given screen.
 * Parameters:
 *    Screen* - Target screen
 * Returns:
 *    Const reference to 2D vector of Seat pointers
 */
const std::vector<std::vector<Seat*>>& SeatManagementService::getSeatLayout(const std::string& selectedScreenId)
{
    ScopedLock lock(m_mutex);
    Screen* screen = m_dataStore.getScreenById(selectedScreenId);
    return  screen->getSeatGrid();
}

/*
 * Function: SeatManagementService::deactivateSeat
 * Description: Attempts to block a seat in the given screen if conditions allow.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    ProcessStatus::SUCCESS if seat blocked, FAILED otherwise
 */
Enums::ProcessStatus SeatManagementService::deactivateSeat(const std::string& selectedScreenId, const std::string& seatId)
{
    ScopedLock lock(m_mutex);
    Screen* screen = m_dataStore.getScreenById(selectedScreenId);
    if (!screen)
    {
        return Enums::ProcessStatus::FAILED;
    }
    Enums::ProcessStatus status = hasActiveSeatBooking(screen, seatId);
    if (status == Enums::ProcessStatus::FAILED)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::vector<std::vector<Seat*>>& seatGrid = screen->getSeatGridForUpdation();
    for (std::vector<std::vector<Seat*>>::iterator rowIterator = seatGrid.begin(); rowIterator != seatGrid.end(); ++rowIterator)
    {
        for (std::vector<Seat*>::iterator seatIterator = rowIterator->begin(); seatIterator != rowIterator->end(); ++seatIterator)
        {
            if ((*seatIterator)->getSeatId() == seatId)
            {
                if ((*seatIterator)->getSeatStatus() == Enums::SeatStatus::BLOCKED)
                {
                    return Enums::ProcessStatus::FAILED;
                }
                m_dataStore.updateSeatStatus(seatId, Enums::SeatStatus::BLOCKED);
                (*seatIterator)->setSeatStatus(Enums::SeatStatus::BLOCKED);
                return Enums::ProcessStatus::SUCCESS;
            }
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: SeatManagementService::deactivateSeats
 * Description: Deactivates all seats associated with a given screen by setting
 *              their status to BLOCKED.
 * Parameters:
 *    selectedScreenId - Unique identifier of the screen whose seats should be deactivated
 * Returns:
 *    Enums::ProcessStatus::SUCCESS after all seats are blocked
 */
Enums::ProcessStatus SeatManagementService::deactivateSeats(const std::string& selectedScreenId)
{
    ScopedLock lock(m_mutex);
    const std::map<std::string, Seat*>& seats = m_dataStore.getSeats();
    for (std::map<std::string, Seat*>::const_iterator iterator = seats.begin(); iterator != seats.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getScreen() && iterator->second->getScreen()->getScreenId() == selectedScreenId)
        {
            Seat* seat = m_dataStore.getSeatById(iterator->second->getSeatId());
            m_dataStore.updateSeatStatus(seat->getSeatId(), Enums::SeatStatus::BLOCKED);
            seat->setSeatStatus(Enums::SeatStatus::BLOCKED);
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: SeatManagementService::hasActiveSeatBooking
 * Description: Checks whether the seat has confirmed bookings in active shows.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    ProcessStatus::FAILED if active booking exists, SUCCESS otherwise
 */
Enums::ProcessStatus SeatManagementService::hasActiveSeatBooking(Screen* screen, const std::string& seatId)
{
    std::map<std::string, Show*>& shows = m_dataStore.getShowsForUpdation();
    for (std::map<std::string, Show*>::iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
    {
        Show* show = iterator->second;
        if (show->getScreen()->getScreenId() == screen->getScreenId())
        {
            if (show->getShowStatus() == Enums::ShowStatus::RUNNING || show->getShowStatus() == Enums::ShowStatus::SCHEDULED)
            {
                ShowSeatAvailability* availability = show->getSeatAvailability();
                const std::map<std::string, Enums::BookingStatus>& seatAvailabilityMap = availability->getSeatAvailabilityMap();
                std::map<std::string, Enums::BookingStatus>::const_iterator seatIterator = seatAvailabilityMap.find(seatId);
                if (seatIterator != seatAvailabilityMap.end())
                {
                    if (seatIterator->second == Enums::BookingStatus::CONFIRMED)
                    {
                        return Enums::ProcessStatus::FAILED;
                    }
                }
            }
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: reactivateSeat
 * Description: Changes a blocked seat back to available in the given screen.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    ProcessStatus::SUCCESS if seat reactivated, FAILED otherwise
 */
Enums::ProcessStatus SeatManagementService::reactivateSeat(const std::string& selectedScreenId, const std::string& seatId)
{
    ScopedLock lock(m_mutex);
    Screen* screen = m_dataStore.getScreenById(selectedScreenId);
    if (!screen)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::vector<std::vector<Seat*>>& seatGrid = screen->getSeatGridForUpdation();
    for (std::vector<std::vector<Seat*>>::iterator rowIterator = seatGrid.begin(); rowIterator != seatGrid.end(); ++rowIterator)
    {
        for (std::vector<Seat*>::iterator seatIterator = (*rowIterator).begin(); seatIterator != (*rowIterator).end(); ++seatIterator)
        {
            if ((*seatIterator)->getSeatId() == seatId)
            {
                if ((*seatIterator)->getSeatStatus() == Enums::SeatStatus::BLOCKED)
                {
                    m_dataStore.updateSeatStatus(seatId, Enums::SeatStatus::AVAILABLE);
                    (*seatIterator)->setSeatStatus(Enums::SeatStatus::AVAILABLE);
                    return Enums::ProcessStatus::SUCCESS;
                }
            }
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: SeatManagementService::reactivateSeats
 * Description: Reactivates all seats associated with a given screen by setting
 *              their status to AVAILABLE.
 * Parameters:
 *    selectedScreenId - Unique identifier of the screen whose seats should be reactivated
 * Returns:
 *    Enums::ProcessStatus::SUCCESS after all seats are made available
 */
Enums::ProcessStatus SeatManagementService::reactivateSeats(const std::string& selectedScreenId)
{
    ScopedLock lock(m_mutex);
    const std::map<std::string, Seat*>& seats = m_dataStore.getSeats();
    for (std::map<std::string, Seat*>::const_iterator iterator = seats.begin(); iterator != seats.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getScreen() && iterator->second->getScreen()->getScreenId() == selectedScreenId)
        {
            Seat* seat = m_dataStore.getSeatById(iterator->second->getSeatId());
            m_dataStore.updateSeatStatus(seat->getSeatId(), Enums::SeatStatus::AVAILABLE);
            seat->setSeatStatus(Enums::SeatStatus::AVAILABLE);
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
Function Name : getShowsSeatLayout
Description   : Builds and returns a 2D display layout for the given show’s seats.
                Delegates seat status evaluation to helper functions.
Parameters    :
                 show - Pointer to the Show object whose seat layout is to be generated
Return Type   : const std::vector<std::vector<std::string>>
*/
const std::vector<std::vector<std::string>> SeatManagementService::getShowsSeatLayout(const Show* show)
{
    std::vector<std::vector<std::string>> displayLayout;
    if (!show)
    {
        return displayLayout;
    }
    const Screen* screen = show->getScreen();
    if (!screen)
    {
        return displayLayout;
    }
    const std::vector<std::vector<Seat*>>& seatGrid = screen->getSeatGrid();
    const std::map<std::string, Enums::BookingStatus>& availabilityMap = show->getSeatAvailability()->getSeatAvailabilityMap();
    for (auto rowIterator = seatGrid.begin(); rowIterator != seatGrid.end(); ++rowIterator)
    {
        std::vector<std::string> rowDisplay;
        for (auto seatIterator = rowIterator->begin(); seatIterator != rowIterator->end(); ++seatIterator)
        {
            rowDisplay.push_back(formatSeatDisplay(*seatIterator, availabilityMap));
        }
        displayLayout.push_back(rowDisplay);
    }
    return displayLayout;
}

/*
Function Name : formatSeatDisplay
Description   : Formats the display string for a single seat based on its status
                and booking availability.
Parameters    :
                 seat            - Pointer to the Seat object
                 availabilityMap - Map of seat IDs to booking statuses
Return Type   : std::string
*/
std::string SeatManagementService::formatSeatDisplay(const Seat* seat, const std::map<std::string, Enums::BookingStatus>& availabilityMap)
{
    if (!seat)
    {
        return " -[NA] ";
    }
    std::string seatId = seat->getSeatId();
    if (seat->getSeatStatus() == Enums::SeatStatus::BLOCKED)
    {
        return seatId + " -[D] ";
    }
    std::map<std::string, Enums::BookingStatus>::const_iterator availabilityIterator = availabilityMap.find(seatId);
    if (availabilityIterator != availabilityMap.end())
    {
        if (availabilityIterator->second == Enums::BookingStatus::CONFIRMED)
        {
            return seatId + " -[B] ";
        }
        else
        {
            return seatId + " -[A] ";
        }
    }
    return seatId + " -[A] ";
}
