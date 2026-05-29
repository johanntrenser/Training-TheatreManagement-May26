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
	std::vector<std::string> lines;
	lines.push_back(config::Header::SHOW_HEADER);
	const std::map<std::string, Show*>& show = m_dataStore.getShows();
	for (std::map<std::string, Show*>::const_iterator iterator = show.begin(); iterator != show.end(); ++iterator)
	{
		lines.push_back((iterator->second)->serialize());
	}
	FileManagement::writeLines(std::string(config::File::SHOW_FILEPATH), lines);
}