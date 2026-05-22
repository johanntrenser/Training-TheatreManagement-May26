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

/*
 * Function: MovieManagementService::searchMovieByTitle
 * Description: Searches for movies in the DataStore that match the given title and are marked as ACTIVE.
 * Parameters:
 *    title - Title of the movie to search for
 * Returns:
 *    A vector of constant Movie pointers representing the matching movies
 */
const std::vector<const Movie*> MovieManagementService::searchMovieByTitle(const std::string& title)
{
	std::vector<const Movie*> resultantMovies;
	const std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getTitle() == title && (iterator->second)->getStatus() == Enums::MovieStatus::ACTIVE)
		{
			resultantMovies.push_back(iterator->second);
		}
	}
	return resultantMovies;
}

/*
 * Function: MovieManagementService::setMovieTitleById
 * Description: Updates the title of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId - Unique identifier of the movie
 *    title   - New title to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieTitleByID(const std::string& movieId, const std::string& title)
{
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId)
		{
			(iterator->second)->setTitle(title);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::setMovieLanguageById
 * Description: Updates the language of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    language - New language to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieLanguageByID(const std::string& movieId, const std::string& language)
{
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId)
		{
			(iterator->second)->setLanguage(language);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::setMovieGenreById
 * Description: Updates the genre of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId - Unique identifier of the movie
 *    genre   - New genre to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieGenreByID(const std::string& movieId, const std::string& genre)
{
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId)
		{
			(iterator->second)->setGenre(genre);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::setMovieDurationById
 * Description: Updates the duration of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    duration - New duration (in minutes) to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieDurationByID(const std::string& movieId, const int& duration)
{
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId)
		{
			(iterator->second)->setDuration(duration);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::getAllActiveMovies
 * Description: Retrieves all movies from the DataStore that are currently marked as ACTIVE.
 * Parameters:
 *    None
 * Returns:
 *    A vector of constant Movie pointers representing all active movies
 */
std::vector<const Movie*> MovieManagementService::getAllActiveMovies()
{
	std::vector<const Movie*> allActiveMovies;
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getStatus() == Enums::MovieStatus::ACTIVE)
		{
			allActiveMovies.push_back(iterator->second);
		}
	}
	return allActiveMovies;
}

/*
 * Function: MovieManagementService::setMovieDeactive
 * Description: Deactivates a movie in the system by updating its status to INACTIVE.
 * Parameters:
 *    movieId - Unique identifier of the movie to deactivate
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was successfully deactivated,
 *    Enums::ProcessStatus::FAILED if the movie ID was not found
 */
Enums::ProcessStatus MovieManagementService::setMovieDeactive(const std::string& movieId)
{
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId)
		{
			(iterator->second)->setStatus(Enums::MovieStatus::INACTIVE);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::setMovieActive
 * Description: Activates a movie by searching the datastore for the given movieId
 *              and updating its status to Enums::MovieStatus::ACTIVE if found.
 * Parameters:
 *    movieId - Unique identifier of the movie to activate
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was found and activated,
 *    Enums::ProcessStatus::FAILED if the movieId does not exist in the datastore
 */
Enums::ProcessStatus MovieManagementService::setMovieActive(const std::string& movieId)
{
	std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId)
		{
			(iterator->second)->setStatus(Enums::MovieStatus::ACTIVE);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::searchDeactivatedMovieByTitle
 * Description: Searches for movies in the DataStore that match the given title and are marked as INACTIVE.
 * Parameters:
 *    title - Title of the movie to search for
 * Returns:
 *    A vector of constant Movie pointers representing the matching deactivated movies
 */
const std::vector<const Movie*> MovieManagementService::searchDeactivatedMovieByTitle(const std::string& title)
{
	std::vector<const Movie*> resultantMovies;
	const std::map<std::string, Movie*> movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getTitle() == title && (iterator->second)->getStatus() == Enums::MovieStatus::INACTIVE)
		{
			resultantMovies.push_back(iterator->second);
		}
	}
	return resultantMovies;
}