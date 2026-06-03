#include <vector>
#include <iomanip>
#include <sstream>
#include "ScreenManagementService.h"
#include "Factory.h"

/*
* Function Name : ScreenManagementService
* Description   : Initializes the screen management service and connects it with the datastore.
* Parameters    : None
* Return Type   : Constructor
*/
ScreenManagementService::ScreenManagementService()
    : m_dataStore(DataStore::getInstance())
{
}

/*
* Function Name : generateScreenId
* Description   : Generates a unique ID for a new screen.
* Parameters    : None
* Return Type   : std::string
*/
std::string ScreenManagementService::generateScreenId()
{
    const std::map<std::string, Screen*>& screens = m_dataStore.getScreens();
    int idNumber = static_cast<int>(screens.size()) + 1;
    std::ostringstream buffer;
    buffer << "SC" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
* Function Name : generateScreenId
* Description   : Generates a unique ID for a new screen.
* Parameters    : None
* Return Type   : std::string
*/
std::string ScreenManagementService::generateSeatId()
{
    const std::map<std::string, Seat*>& seats = m_dataStore.getSeats();
    int idNumber = static_cast<int>(seats.size()) + 1;
    std::ostringstream buffer;
    buffer << "ST" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
* Function Name : addScreen
* Description   : Adds a new screen to the theatre and creates its seat grid.
* Parameters    :
*                  theatre     - Theatre where the screen is added
*                  name        - Name of the screen
*                  seatRows    - Number of seat rows
*                  seatColumns - Number of seat columns
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus ScreenManagementService::addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount)
{
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (!theatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    const std::vector<Screen*>& currentTheatreScreens = theatre->getScreens();
    for (std::vector<Screen*>::const_iterator iterator = currentTheatreScreens.begin(); iterator != currentTheatreScreens.end(); ++iterator)
    {
        if ((*iterator)->getName() == name)
        {
            return Enums::ProcessStatus::ALREADY_EXISTS;
        }
    }
    Screen* screen = Factory::getObject<Screen>(generateScreenId(), theatre, name, seatRows, seatColumns, std::vector<std::vector<Seat*>>());
    if (screen)
    {
        std::vector<std::vector<Seat*>> seatGrid = generateSeatGrid(screen, seatRows, seatColumns, seatAmount);
        if (!seatGrid.empty())
        {
            screen->setSeatGrid(seatGrid);
            m_dataStore.addScreen(screen);
            theatre->setScreen(screen);
            return Enums::ProcessStatus::SUCCESS;
        }
        else
        {
            delete screen;
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : generateSeatGrid
* Description   : Creates and initializes seats for a screen.
* Parameters    :
*                  screen       - Screen for which seats are created
*                  seatRows     - Number of seat rows
*                  seatColumns  - Number of seat columns
* Return Type   : std::vector<std::vector<Seat*>>
*/
std::vector<std::vector<Seat*>> ScreenManagementService::generateSeatGrid(Screen* screen, int seatRows, int seatColumns, double seatAmount)
{
    std::vector<std::vector<Seat*>> seatGrid;
    for (int row = 0; row < seatRows; ++row)
    {
        char rowCharacter = 'A' + row;
        std::vector<Seat*> seatRow;
        for (int column = 0; column < seatColumns; ++column)
        {
            std::string seatId = generateSeatId();
            Seat* seat = Factory::getObject<Seat>(seatId, screen, rowCharacter, column, seatAmount, Enums::SeatStatus::AVAILABLE);
            if (seat != nullptr)
            {
                seatRow.push_back(seat);
                m_dataStore.addSeat(seat);
            }
            else
            {
                cleanupSeatGrid(seatGrid, seatRow);
                return {};
            }
        }
        seatGrid.push_back(seatRow);
    }
    return seatGrid;
}

/*
* Function Name : cleanupSeatGrid
* Description   : Deletes created seats when seat grid creation fails.
* Parameters    :
*                  seatGrid - Existing seat grid
*                  seatRow  - Current seat row
* Return Type   : void
*/
void ScreenManagementService::cleanupSeatGrid(std::vector<std::vector<Seat*>>& seatGrid, std::vector<Seat*>& seatRow)
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
* Function Name : updateScreenName
* Description   : Updates the name of a screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
*                  name     - New screen name
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus ScreenManagementService::updateScreenName(const std::string& theatreId, const std::string& screenId, const std::string& name)
{
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (!theatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::vector<Screen*>& screens = theatre->getScreensForUpdation();
    for (std::vector<Screen*>::iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        if ((*iterator)->getName() == name)
        {
            return Enums::ProcessStatus::ALREADY_EXISTS;
        }
        if ((*iterator)->getScreenId() == screenId)
        {
            (*iterator)->setName(name);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::NOT_FOUND;
}

/*
* Function Name : deactivateScreen
* Description   : Changes an active screen to unavailable status if no active shows exist.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus ScreenManagementService::deactivateScreen(const std::string& theatreId, const std::string& screenId)
{
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (!theatre)
    {
        return Enums::ProcessStatus::FAILED;
    }
    Enums::ProcessStatus status = hasActiveShows(theatreId, screenId);
    if (status == Enums::ProcessStatus::SUCCESS)
    {
        std::vector<Screen*>& screens = theatre->getScreensForUpdation();
        for (std::vector<Screen*>::iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
        {
            if ((*iterator)->getScreenId() == screenId)
            {
                if ((*iterator)->getScreenStatus() == Enums::ScreenStatus::UNAVAILABLE)
                {
                    return Enums::ProcessStatus::ALREADY_EXISTS;
                }
                (*iterator)->setScreenStatus(Enums::ScreenStatus::UNAVAILABLE);
                return Enums::ProcessStatus::SUCCESS;
            }
        }
    }
    return Enums::ProcessStatus::NOT_FOUND;
}

/*
* Function Name : ScreenManagementService::hasActiveShows
* Description   : Checks if a given screen in a theatre has any active shows.
*                 Active shows are defined as those with status RUNNING or SCHEDULED.
*                 Returns FAILED if active shows exist, otherwise SUCCESS.
* Parameters    :
*                  theatreId - The unique identifier of the theatre
*                  screenId  - The unique identifier of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus ScreenManagementService::hasActiveShows(const std::string& theatreId, const std::string& screenId)
{
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    Enums::ProcessStatus status = Enums::ProcessStatus::SUCCESS;
    for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
    {
        if (iterator->second->getScreen()->getScreenId() == screenId)
        {
            if (iterator->second->getShowStatus() == Enums::ShowStatus::RUNNING
                || iterator->second->getShowStatus() == Enums::ShowStatus::SCHEDULED)
            {
                status = Enums::ProcessStatus::FAILED;
                return status;
            }
        }
    }
    return status;
}

/*
* Function Name : reactivateScreen
* Description   : Changes an inactive screen to available status.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus ScreenManagementService::reactivateScreen(const std::string& theatreId, const std::string& screenId)
{
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    std::vector<Screen*>& screens = theatre->getScreensForUpdation();
    for (std::vector<Screen*>::iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        if ((*iterator)->getScreenId() == screenId)
        {
            if ((*iterator)->getScreenStatus() == Enums::ScreenStatus::AVAILABLE)
            {
                return Enums::ProcessStatus::ALREADY_EXISTS;
            }
            (*iterator)->setScreenStatus(Enums::ScreenStatus::AVAILABLE);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::NOT_FOUND;
}

/*
* Function Name : ScreenManagementService::viewTheatreScreens
* Description   : Retrieves screens for a given theatre based on user type.
*                 - Admin and Theatre Owner: all screens (active + inactive)
*                 - Customer: only active (AVAILABLE) screens
*                 Returns an empty list if theatre is not found.
* Parameters    :
*                  theatreId - The unique identifier of the theatre
* Return Type   : const std::vector<const Screen*>
*/
const std::vector<const Screen*> ScreenManagementService::viewTheatreScreens(const std::string& theatreId)
{
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (!theatre)
    {
        return {};
    }
    const std::vector<Screen*>& screens = theatre->getScreens();
    std::vector<const Screen*> result;
    Enums::UserType userType = m_dataStore.getAuthenticatedUserType();
    if (userType == Enums::UserType::ADMIN || userType == Enums::UserType::THEATRE_OWNER)
    {
        for (auto screen : screens)
        {
            result.push_back(screen);
        }
    }
    else if (userType == Enums::UserType::CUSTOMER)
    {
        for (auto screen : screens)
        {
            if (screen->getScreenStatus() == Enums::ScreenStatus::AVAILABLE)
            {
                result.push_back(screen);
            }
        }
    }
    return result;
}

/*
* Function Name : getAuthenticatedUserType
* Description   : Retrieves the Authenticated User Type.
* Parameters    :
*                  None
* Return Type   : Enums::UserType
*/
Enums::UserType ScreenManagementService::getAuthenticatedUserType()
{
    return m_dataStore.getAuthenticatedUserType();
}

/*
 * Function: ScreenManagementService::saveScreenData
 * Description: Saves all screen data from the DataStore into a CSV file.
 *              Includes screen details such as Screen ID, Theatre ID, name, total rows, total columns,
 *              seat layout (seat IDs written row by row, separated by '|'), and screen status.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void ScreenManagementService::saveScreenData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::SCREEN_HEADER);
    const std::map<std::string, Screen*>& screens = m_dataStore.getScreens();
    for (std::map<std::string, Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        if (iterator->second)
        {
            lines.push_back((iterator->second)->serialize());
        }
    }
    FileManagement::writeLines(std::string(config::File::SCREEN_FILEPATH), lines);
}

/*
 * name        : loadScreenData
 * description : Loads screen data from the CSV file, deserializes each line into a Screen object,
 *               resolves references to Theatre, sets status, and adds the Screen to datastore.
 * parameter   : None
 * return type : void
 */
void ScreenManagementService::loadScreenData()
{
    std::string screenId, theatreId, name, totalRows, totalColumns, status;
    std::vector<std::string> lines = FileManagement::readlines(PATH);
    for (int index = 1; index < lines.size(); index++)
    {
        Screen* screen = Screen::deserialize(lines[index]);
        std::stringstream lineStream(lines[index]);
        getline(lineStream, screenId, ',');
        getline(lineStream, theatreId, ',');
        getline(lineStream, name, ',');
        getline(lineStream, totalRows, ',');
        getline(lineStream, totalColumns, ',');
        getline(lineStream, status, ',');
        if (!theatreId.empty())
        {
            Theatre* theatre = m_dataStore.getTheatreById(theatreId);
            screen->setTheatre(theatre);
        }
        Enums::ScreenStatus screenStatus = Enums::getScreenStatus(status);
        screen->setScreenStatus(screenStatus);
        m_dataStore.addScreen(screen);
    }
}