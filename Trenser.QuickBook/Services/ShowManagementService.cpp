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
void ShowManagementService::loadShowData()
{
	std::string showId, movieId, screenId, startTime, endTime, seatAvailabilityId, status, year, dash, space, month, day, hour, colon, minute;
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 0; index < lines.size(); index++)
	{
		Show* show = Show::deserialize(lines[index]);
		std::stringstream lineStream(lines[index]);
		getline(lineStream, showId, ',');
		getline(lineStream, movieId, ',');
		getline(lineStream, screenId, ',');
		getline(lineStream, startTime, ',');
		getline(lineStream, endTime, ',');
		getline(lineStream, seatAvailabilityId, ',');
		getline(lineStream, status, ',');
		if (!movieId.empty())
		{
			Movie* movie=m_dataStore.getMovieById(movieId);
			show->setMovie(movie);
		}
		if (!screenId.empty())
		{
			Screen* screen = m_dataStore.getScreenById(screenId);
			show->setScreen(screen);
		}
		Enums::ShowStatus showStatus = Enums::getShowStatus(status);
		show->setShowStatus(showStatus);
		m_dataStore.addShow(show);
	}
}