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
Enums::ProcessStatus SeatManagementService::updateSeatLayout(Screen* screen, const int rows, const int columns)
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
			delete (*seatIterator);
			(*seatIterator) = nullptr;
		}
	}
	std::vector<std::vector<Seat*>> newSeatGrid;
	for (int row = 0; row < rows; ++row)
	{
		char rowCharacter = 'A' + row;
		std::vector<Seat*> seatRow;
		for (int column = 0; column < columns; ++column)
		{
			std::string seatId = std::string(1, rowCharacter) + std::to_string(column);
			Seat* seat = Factory::getObject<Seat>(seatId, screen, rowCharacter, column, Enums::SeatStatus::AVAILABLE,Enums::BookingStatus::PENDING);
			if (seat != nullptr)
			{
				seatRow.push_back(seat);
			}
			else
			{
				return Enums::ProcessStatus::FAILED;
			}
		}
		if (!seatRow.empty())
		{
			newSeatGrid.push_back(seatRow);
		}
		else
		{
			return Enums::ProcessStatus::FAILED;
		}
	}
	if (!newSeatGrid.empty())
	{
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
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
	std::map<std::string, Show*>& shows = m_dataStore.getShowsForUpdation();
	bool status = true;
	for (std::map<std::string, Show*>::iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
	{
		if (iterator->second->getScreen()->getScreenId() == screen->getScreenId())
		{
			if (iterator->second->getShowStatus() != Enums::ShowStatus::RUNNING
				&& iterator->second->getShowStatus() != Enums::ShowStatus::SCHEDULED)
			{
				ShowSeatAvailability* showSeatAvailability = iterator->second->getSeatAvailability();
				const std::map<std::string, Seat*>& seatAvailabilityMap = showSeatAvailability->getSeatAvailabilityMap();
				for (std::map<std::string, Seat*>::const_iterator seatIterator = seatAvailabilityMap.begin(); seatIterator != seatAvailabilityMap.end(); ++seatIterator)
				{
					if (seatIterator->second->getSeatId() == seatId
						&& seatIterator->second->getSeatStatus() != Enums::SeatStatus::AVAILABLE)
					{
						status = false;
					}
				}
			}
		}
	}
	if (status == false)
	{
		std::vector<std::vector<Seat*>>& seatGrid = screen->getSeatGridForUpdation();
		for (std::vector<std::vector<Seat*>>::iterator rowIterator = seatGrid.begin(); rowIterator != seatGrid.end(); ++rowIterator)
		{
			for (std::vector<Seat*>::iterator seatIterator = (*rowIterator).begin(); seatIterator != (*rowIterator).end(); ++seatIterator)
			{
				if ((*seatIterator)->getSeatId() == seatId)
				{
					if ((*seatIterator)->getSeatStatus() == Enums::SeatStatus::AVAILABLE)
					{
						(*seatIterator)->setSeatStatus(Enums::SeatStatus::BLOCKED);
						return Enums::ProcessStatus::SUCCESS;
					}
				}
			}
		}
	}
	return Enums::ProcessStatus::FAILED;
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
