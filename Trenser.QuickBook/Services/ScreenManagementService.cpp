#include <vector>
#include <iomanip>
#include <sstream>
#include "ScreenManagementService.h"
#include "Factory.h"
#include "SeatManagementService.h"
#include "ApplicationConfig.h"

/*
* Function Name : ScreenManagementService
* Description   : Initializes the screen management service and connects it with the datastore.
* Parameters    : None
* Return Type   : Constructor
*/
ScreenManagementService::ScreenManagementService()
    : m_dataStore(DataStore::getInstance()),
      m_screenMutex(config::MutexMappings::SCREEN_MUTEX_NAME),
      m_seatMutex(config::MutexMappings::SEAT_MUTEX_NAME)
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
    const int count = m_dataStore.getScreenCount();
    int idNumber = count + 1;
    std::ostringstream buffer;
    buffer << "SC" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
* Function Name : generateSeatId
* Description   : Generates a unique ID for a new seat.
* Parameters    : None
* Return Type   : std::string
*/
std::string ScreenManagementService::generateSeatId()
{
    const int count = m_dataStore.getSeatCount();
    int idNumber = count + 1;
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
    ScopedLock screenLock(m_screenMutex);
    ScopedLock seatLock(m_seatMutex);
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
            std::string message = "Screen " + screen->getScreenId() + " added to theatre " + theatreId;
            m_logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
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
    ScopedLock lock(m_screenMutex);
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
            m_dataStore.updateScreenName(screenId, name);
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
    ScopedLock screenLock(m_screenMutex);
    ScopedLock seatLock(m_seatMutex);
    SeatManagementService seatManagementService;
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
                m_dataStore.updateScreenStatus(screenId, Enums::ScreenStatus::UNAVAILABLE);
                (*iterator)->setScreenStatus(Enums::ScreenStatus::UNAVAILABLE);
                Enums::ProcessStatus seatDeactivationStatus = seatManagementService.deactivateSeats(screenId);
                return seatDeactivationStatus;
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
            if (iterator->second->getShowStatus() == Enums::ShowStatus::RUNNING || iterator->second->getShowStatus() == Enums::ShowStatus::SCHEDULED)
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
    ScopedLock screenLock(m_screenMutex);
    ScopedLock seatLock(m_seatMutex);
    SeatManagementService seatManagementService;
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
            m_dataStore.updateScreenStatus(screenId, Enums::ScreenStatus::AVAILABLE);
            (*iterator)->setScreenStatus(Enums::ScreenStatus::AVAILABLE);
            Enums::ProcessStatus seatReactivationStatus = seatManagementService.reactivateSeats(screenId);
            return seatReactivationStatus;
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
    ScopedLock lock(m_screenMutex);
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
        for (Screen* screen : screens)
        {
            result.push_back(screen);
        }
    }
    else if (userType == Enums::UserType::CUSTOMER)
    {
        for (Screen* screen : screens)
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