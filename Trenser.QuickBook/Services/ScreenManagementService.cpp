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
			std::string seatId = std::string(1, rowCharacter) + std::to_string(column);
			Seat* seat = Factory::getObject<Seat>(seatId, screen, rowCharacter, column, seatAmount, Enums::SeatStatus::AVAILABLE, Enums::BookingStatus::PENDING);
			if (seat != nullptr)
			{
				seatRow.push_back(seat);
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
	for (std::vector<Seat*>& rowSeats : seatGrid)
	{
		for (Seat* existingSeat : rowSeats)
		{
			delete existingSeat;
		}
	}
	for (Seat* existingSeat : seatRow)
	{
		delete existingSeat;
	}
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
		if ((*iterator)->getScreenId() == screenId)
		{
			(*iterator)->setName(name);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
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
	const std::map<std::string, Show*>& shows = m_dataStore.getShows();
	bool hasActiveShows = false;
	for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
	{
		if (iterator->second->getScreen()->getScreenId() == screenId)
		{
			if (iterator->second->getShowStatus() == Enums::ShowStatus::RUNNING
				|| iterator->second->getShowStatus() == Enums::ShowStatus::SCHEDULED)
			{
				hasActiveShows = true;
				break;
			}
		}
	}
	if (!hasActiveShows)
	{
		std::vector<Screen*>& screens = theatre->getScreensForUpdation();
		for (std::vector<Screen*>::iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
		{
			if ((*iterator)->getScreenId() == screenId)
			{
				if ((*iterator)->getScreenStatus() == Enums::ScreenStatus::UNAVAILABLE)
				{
					return Enums::ProcessStatus::FAILED;
				}
				(*iterator)->setScreenStatus(Enums::ScreenStatus::UNAVAILABLE);
				return Enums::ProcessStatus::SUCCESS;
			}
		}
	}
	return Enums::ProcessStatus::NOT_FOUND;
}