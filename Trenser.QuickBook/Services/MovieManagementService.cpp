/*
 * File: MovieManagementService.cpp
 * Description: Implements movie management operations including creation,
 *              retrieval, and formatting of movie-related data. Utilizes
 *              Factory pattern for object instantiation and supports
 *              formatted output for reporting.
 * Author: Trenser
 * Created: 22 May 2026
 */
#include <sstream>
#include <iomanip>
#include <map>
#include "MovieManagementService.h"
#include "Factory.h"

/*
 * Function: MovieManagementService::MovieManagementService
 * Description: Constructs a MovieManagementService object and initializes the DataStore instance.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
MovieManagementService::MovieManagementService() :
	m_dataStore(DataStore::getInstance())
{
}

/*
 * Function: MovieManagementService::generateMovieId
 * Description: Generates a unique movie ID based on the current number of movies in the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    A string representing the newly generated movie ID
 */
const std::string MovieManagementService::generateMovieId()
{
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	int idNumber = static_cast<int>(movies.size()) + 1;
	std::ostringstream buffer;
	buffer << "MO" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

/*
 * Function: MovieManagementService::addMovieToSystem
 * Description: Creates a new Movie object and adds it to the DataStore.
 * Parameters:
 *    title    - Title of the movie
 *    language - Language of the movie
 *    genre    - Genre of the movie
 *    duration - Duration of the movie in minutes
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was added successfully,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::addMovieToSystem(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
	Movie* movie = Factory::getObject<Movie>(generateMovieId(), title, language, genre, duration);
	if (movie != nullptr)
	{
		m_dataStore.addMovieToSystem(movie);
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::isMovieUniqueInSystem
 * Description: Checks if a movie with the given attributes already exists in the DataStore.
 * Parameters:
 *    title    - Title of the movie
 *    language - Language of the movie
 *    genre    - Genre of the movie
 *    duration - Duration of the movie in minutes
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie is unique,
 *    Enums::ProcessStatus::FAILED if a duplicate exists
 */
Enums::ProcessStatus MovieManagementService::isMovieUniqueInSystem(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getTitle() == title && (iterator->second)->getLanguage() == language && (iterator->second)->getGenre() == genre && (iterator->second)->getDuration() == duration)
		{
			return Enums::ProcessStatus::FAILED;
		}
	}
	return Enums::ProcessStatus::SUCCESS;
}