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
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 0; index < lines.size(); ++index)
	{
		Seat* seat = Seat::deserialize(lines[index]);
		std::string seatId, screenId, seatRow, seatColumn, amount, seatStatus;
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
			seat->setScreen(screen);
		}
		m_dataStore.addSeat(seat);
	}
}