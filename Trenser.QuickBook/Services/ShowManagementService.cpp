#include "ShowManagementService.h"

/*
 * Function: ShowManagementService::saveShowData
 * Description: Saves all show data from the DataStore into a CSV file.
 *              Includes show details such as Show ID, Movie ID, Screen ID,
 *              start time, end time, and show status.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void ShowManagementService::saveShowData()
{
	const std::map<std::string, Show*>& show = m_dataStore.getShows();
	std::ofstream showFile(PATH, std::ios::trunc);
	if (!showFile.is_open())
	{
		throw std::runtime_error("Cannot open file: " + PATH);
	}
	showFile << "SHOW ID,MOVIE ID,SCREEN ID,STARTING TIME,ENDING TIME,SEAT AVAILABILITY ID,STATUS\n";
	for (std::map<std::string, Show*>::const_iterator iterator = show.begin(); iterator != show.end(); ++iterator)
	{
		showFile << (iterator->second)->getShowId() << ","
			<< (iterator->second)->getMovie()->getMovieId() << ","
			<< (iterator->second)->getScreen()->getScreenId() << ","
			<< (iterator->second)->getStartTime() << ","
			<< (iterator->second)->getEndTime() << ","
			<< Enums::getShowStatusString((iterator->second)->getShowStatus()) << "\n";
	}
	showFile.close();
}