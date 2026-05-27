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
	showFile << config::Header::SHOW_HEADER<<"\n";
	for (std::map<std::string, Show*>::const_iterator iterator = show.begin(); iterator != show.end(); ++iterator)
	{
		showFile << (iterator->second)->serialize()<< "\n";
	}
	showFile.close();
}