#include "SeatManagementService.h"

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
#include <iostream>
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