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
void SeatManagementService::saveSeatData()
{
	const std::map<std::string, Seat*>& seats = m_dataStore.getSeats();
	std::ofstream seatFile(PATH, std::ios::trunc);
	if (!seatFile.is_open())
	{
		throw std::runtime_error("Cannot open file: " + PATH);
	}
	seatFile << "SEAT ID,SCREEN ID,SEAT ROW,SEAT COLUMN,STATUS\n";
	for (std::map<std::string, Seat*>::const_iterator iterator = seats.begin(); iterator != seats.end(); ++iterator)
	{
		seatFile << (iterator->second)->getSeatId() << ","
			<< (iterator->second)->getScreen()->getScreenId() << ","
			<< (iterator->second)->getSeatRow() << ","
			<< (iterator->second)->getSeatColumn() << ","
			<< (iterator->second)->getSeatAmount() << ","
			<< Enums::getSeatStatusString((iterator->second)->getSeatStatus()) << "\n";
	}
	seatFile.close();
}