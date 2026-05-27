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
 * Function: updateSeatLayout
 * Description: Rebuilds the seating grid for a screen with given rows and columns.
 * Parameters:
 *    screen  - Target screen
 *    rows    - Number of rows
 *    columns - Number of columns
 * Returns:
 *    ProcessStatus::SUCCESS if layout updated, FAILED otherwise
 */
Enums::ProcessStatus SeatManagementService::updateSeatLayout(Screen* screen, int rows, int columns, double amount)
{
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
			std::string seatId = std::string(1, rowCharacter) + std::to_string(column);
			Seat* seat = Factory::getObject<Seat>(seatId, screen, rowCharacter, column, amount, Enums::SeatStatus::AVAILABLE);
			if (!seat)
			{
				return Enums::ProcessStatus::FAILED;
			}
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
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : clearSeatGrid
* Description   : Deletes all seats in the seat grid and clears memory.
* Parameters    :
*                  seatGrid - Seat grid to be cleared
* Return Type   : void
*/
void SeatManagementService::clearSeatGrid(std::vector<std::vector<Seat*>>& seatGrid)
{
	for (std::vector<std::vector<Seat*>>::iterator rowIterator = seatGrid.begin(); rowIterator != seatGrid.end(); ++rowIterator)
	{
		for (std::vector<Seat*>::iterator seatIterator = (*rowIterator).begin(); seatIterator != (*rowIterator).end(); ++seatIterator)
		{
			delete (*seatIterator);
			(*seatIterator) = nullptr;
		}
	}
}

/*
 * Function: getSeatLayout
 * Description: Returns the seating grid of a given screen.
 * Parameters:
 *    Screen* - Target screen
 * Returns:
 *    Const reference to 2D vector of Seat pointers
 */
const std::vector<std::vector<Seat*>>& SeatManagementService::getSeatLayout(const Screen* screen) const
{
	return screen->getSeatGrid();
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
Enums::ProcessStatus SeatManagementService::deactivateSeat(Screen* screen, const std::string& seatId)
{
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
Enums::ProcessStatus SeatManagementService::reactivateSeat(Screen* screen, const std::string& seatId)
{
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
* Function Name : getShowsSeatLayout
* Description   : Builds and returns a 2D display layout for the given show’s seats.
*                 Delegates seat status evaluation to helper functions.
* Parameters    :
*                  show - Pointer to the Show object whose seat layout is to be generated
* Return Type   : const std::vector<std::vector<std::string>>
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
* Function Name : formatSeatDisplay
* Description   : Formats the display string for a single seat based on its status
*                 and booking availability.
* Parameters    :
*                  seat            - Pointer to the Seat object
*                  availabilityMap - Map of seat IDs to booking statuses
* Return Type   : std::string
*/
std::string SeatManagementService::formatSeatDisplay(const Seat* seat, const std::map<std::string, Enums::BookingStatus>& availabilityMap)
{
	if (!seat)
	{
		return "-[NA] ";
	}
	std::string seatId = seat->getSeatId();
	if (seat->getSeatStatus() == Enums::SeatStatus::BLOCKED)
	{
		return seatId + "-[D] ";
	}
	if (seat->getSeatStatus() == Enums::SeatStatus::RESERVED)
	{
		return seatId + "-[R] ";
	}
	std::map<std::string, Enums::BookingStatus>::const_iterator availabilityIterator = availabilityMap.find(seatId);
	if (availabilityIterator != availabilityMap.end())
	{
		if (availabilityIterator->second == Enums::BookingStatus::CONFIRMED)
		{
			return seatId + "-[B] ";
		}
		else
		{
			return seatId + "-[A] ";
		}	
	}
	return seatId + "-[A] ";
}