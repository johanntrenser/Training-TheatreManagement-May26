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

 /*
  * Function: SeatManagementService
  * Description: Initializes service with singleton DataStore instance.
  * Parameters:
  *    None
  * Returns:
  *    None
  */
SeatManagementService::SeatManagementService()
    : m_dataStore(DataStore::getInstance())
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
    const std::map<std::string, Seat*>& seats = m_dataStore.getSeats();
    int idNumber = static_cast<int>(seats.size()) + 1;
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
const std::vector<std::vector<Seat*>>& SeatManagementService::getSeatLayout(const std::string& selectedScreenId) const
{
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
                (*seatIterator)->setSeatStatus(Enums::SeatStatus::BLOCKED);
                return Enums::ProcessStatus::SUCCESS;
            }
        }
    }
    return Enums::ProcessStatus::FAILED;
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
                    (*seatIterator)->setSeatStatus(Enums::SeatStatus::AVAILABLE);
                    return Enums::ProcessStatus::SUCCESS;
                }
            }
        }
    }
    return Enums::ProcessStatus::FAILED;
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

/*
 * Function: SeatManagementService::saveSeatData
 * Description: Saves all seat data from the DataStore into a CSV file.
 *              Includes seat details such as Seat ID, Screen ID, row, column,
 *              seat amount, and seat status.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void SeatManagementService::saveSeatData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::SEAT_HEADER);
    const std::map<std::string, Seat*>& seats = m_dataStore.getSeats();
    for (std::map<std::string, Seat*>::const_iterator iterator = seats.begin(); iterator != seats.end(); ++iterator)
    {
        lines.push_back((iterator->second)->serialize());
    }
    FileManagement::writeLines(std::string(config::File::SEAT_FILEPATH), lines);
}

/*
 * Function: SeatManagementService::loadSeatData
 * Description: Loads all seat data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Seat object via Seat::deserialize,
 *              and restores the association with its Screen if the Screen ID
 *              is present and found in the DataStore.
 *              Finally, adds the reconstructed Seat to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void SeatManagementService::loadSeatData()
{
    std::vector<std::string> lines = FileManagement::readlines(PATH_SEAT);
    for (int index = 1; index < lines.size(); ++index)
    {
        Seat* seat = Seat::deserialize(lines[index]);
        std::string seatId;
        std::string screenId;
        std::string seatRow;
        std::string seatColumn;
        std::string amount;
        std::string seatStatus;
        std::stringstream lineStream(lines[index]);
        getline(lineStream, seatId, ',');
        getline(lineStream, screenId, ',');
        getline(lineStream, seatRow, ',');
        getline(lineStream, seatColumn, ',');
        getline(lineStream, amount, ',');
        getline(lineStream, seatStatus, ',');
        if (!screenId.empty())
        {
            Screen* screen = m_dataStore.getScreenById(screenId);
            if (screen != nullptr)
            {
                seat->setScreen(screen);
                std::vector<std::vector<Seat*> >& seatGrid = screen->getSeatGridForUpdation();
                if (seatGrid.empty())
                {
                    seatGrid.resize(
                        screen->getTotalRows(),
                        std::vector<Seat*>(screen->getTotalColumns(), nullptr));
                }
                int rowIndex = seat->getSeatRow() - 'A';
                int columnIndex = seat->getSeatColumn();
                if (rowIndex >= 0 && rowIndex < screen->getTotalRows() && columnIndex >= 0 && columnIndex < screen->getTotalColumns())
                {
                    seatGrid[rowIndex][columnIndex] = seat;
                }
            }
        }
        m_dataStore.addSeat(seat);
    }
}

/*
 * Function: SeatManagementService::saveShowSeatAvailabilityData
 * Description: Persists all ShowSeatAvailability records from the DataStore into a CSV file.
 *              Builds a list of serialized strings for each ShowSeatAvailability object,
 *              prepends the configurable header, and writes the entire collection to the
 *              designated file path. This ensures that seat availability information for
 *              shows is saved and can be reloaded later.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or written)
 */
void SeatManagementService::saveShowSeatAvailabilityData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::SHOW_SEAT_HEADER);
    const std::map<std::string, ShowSeatAvailability*>& showSeats = m_dataStore.getShowSeatAvailabilityList();
    for (std::map<std::string, ShowSeatAvailability*>::const_iterator iterator = showSeats.begin(); iterator != showSeats.end(); ++iterator)
    {
        lines.push_back((iterator->second)->serialize());
    }
    FileManagement::writeLines(std::string(config::File::SHOW_SEAT_FILEPATH), lines);
}

/*
 * Function: SeatManagementService::loadShowSeatAvailabilityData
 * Description: Loads all ShowSeatAvailability records from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines, skipping the header.
 *              Deserializes each line into a ShowSeatAvailability object, restores its association
 *              with the corresponding Show (if available), and reconstructs the seat availability map
 *              by parsing seat entries in the format "SE001:AVAILABLE|SE002:BOOKED".
 *              Finally, adds the reconstructed ShowSeatAvailability object to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void SeatManagementService::loadShowSeatAvailabilityData()
{
    std::vector<std::string> lines = FileManagement::readlines(config::File::SHOW_SEAT_FILEPATH);
    std::string showSeatAvailabilityId, showId, seatBlock;
    for (int index = 1; index < lines.size(); ++index)
    {
        ShowSeatAvailability* showSeatAvailability = ShowSeatAvailability::deserialize(lines[index]);
        std::stringstream lineStream(lines[index]);
        getline(lineStream, showSeatAvailabilityId, ',');
        getline(lineStream, showId, ',');
        getline(lineStream, seatBlock, ',');
        if (!showId.empty())
        {
            Show* show = m_dataStore.getShowDetailsById(showId);
            showSeatAvailability->setShow(show);
        }
        if (!seatBlock.empty())
        {
            std::stringstream seatStream(seatBlock);
            std::string seatEntry;
            std::map<std::string, Enums::BookingStatus> seatAvailabilityMap;
            while (getline(seatStream, seatEntry, config::delimeter::verticalBar[0]))
            {
                int position = int(seatEntry.find(config::delimeter::colon));
                if (position != std::string::npos)
                {
                    std::string seatId = seatEntry.substr(0, position);
                    std::string statusString = seatEntry.substr(position + 1);
                    util::trimWhitespace(statusString);
                    seatAvailabilityMap[seatId] = Enums::getBookingStatus(statusString);
                }
            }
            showSeatAvailability->setSeatAvailabilityMap(seatAvailabilityMap);
        }
        m_dataStore.addShowSeatAvailabilityList(showSeatAvailability);
    }
}