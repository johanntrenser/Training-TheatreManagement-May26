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
#include "InputHelper.h"
 /*
  * Function: MovieManagementService::MovieManagementService
  * Description: Constructs a MovieManagementService object and initializes the DataStore instance.
  * Parameters:
  *    None
  * Returns:
  *    None
  */
MovieManagementService::MovieManagementService() :
	m_dataStore(DataStore::getInstance()), m_mutex(config::MutexMappings::MOVIE_MUTEX_NAME)
{
}

/*
 * Function: MovieManagementService::generateMovieId
 * Description: Generates a unique movie ID based on the current number of movies in the DataStore.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    None
 * Returns:
 *    A string representing the newly generated movie ID
 */
const std::string MovieManagementService::generateMovieId()
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	int idNumber = static_cast<int>(movies.size()) + 1;
	std::ostringstream buffer;
	buffer << "MO" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

/*
 * Function: MovieManagementService::addMovieToSystem
 * Description: Creates and adds a new Movie to the system.
 *              Generates a unique Movie ID, constructs a Movie object using the Factory,
 *              and persists it into the DataStore. Ensures thread safety by acquiring
 *              a scoped lock during the operation. Returns SUCCESS if the movie is
 *              created and stored successfully, otherwise FAILED.
 * Parameters:
 *    title    - The title of the movie.
 *    language - The language of the movie.
 *    genre    - The genre of the movie.
 *    duration - The duration of the movie in minutes.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was added successfully.
 *    Enums::ProcessStatus::FAILED if the movie creation or persistence failed.
 */
Enums::ProcessStatus MovieManagementService::addMovieToSystem(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
	ScopedLock lock(m_mutex);
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
 * Description: Checks whether a movie with the given attributes already exists in the system.
 *              Acquires a scoped lock to ensure thread safety, retrieves all movies from the DataStore,
 *              and iterates through them to compare title, language, genre, and duration.
 *              If a matching movie is found, the function returns FAILED, indicating the movie
 *              is not unique. Otherwise, returns SUCCESS.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    title    - The title of the movie to check.
 *    language - The language of the movie.
 *    genre    - The genre of the movie.
 *    duration - The duration of the movie in minutes.
 * Returns:
 *    Enums::ProcessStatus::FAILED if a duplicate movie exists.
 *    Enums::ProcessStatus::SUCCESS if the movie is unique in the system.
 */
Enums::ProcessStatus MovieManagementService::isMovieUniqueInSystem(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
	ScopedLock lock(m_mutex);
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
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    title - Title of the movie to search for
 * Returns:
 *    A vector of constant Movie pointers representing the matching movies
 */
const std::vector<const Movie*> MovieManagementService::searchMovieByTitle(const std::string& title)
{
	ScopedLock lock(m_mutex);
	std::vector<const Movie*> resultantMovies;
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if (utils::to_lower((iterator->second)->getTitle()) == utils::to_lower(title) && (iterator->second)->getStatus() == Enums::MovieStatus::ACTIVE)
		{
			resultantMovies.push_back(iterator->second);
		}
	}
	return resultantMovies;
}

/*
 * Function: MovieManagementService::setMovieTitleById
 * Description: Updates the title of a movie identified by its unique movie ID.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId - Unique identifier of the movie
 *    title   - New title to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieTitleByID(const std::string& movieId, const std::string& title)
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
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
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    language - New language to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieLanguageByID(const std::string& movieId, const std::string& language)
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
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
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId - Unique identifier of the movie
 *    genre   - New genre to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieGenreByID(const std::string& movieId, const std::string& genre)
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
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
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    duration - New duration (in minutes) to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus MovieManagementService::setMovieDurationByID(const std::string& movieId, const int& duration)
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
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
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    None
 * Returns:
 *    A vector of constant Movie pointers representing all active movies
 */
std::vector<const Movie*> MovieManagementService::getAllActiveMovies()
{
	ScopedLock lock(m_mutex);
	std::vector<const Movie*> allActiveMovies;
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
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
 * Function: MovieManagementService::deactivateMovie
 * Description: Deactivates a movie in the system by updating its status to INACTIVE.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId - Unique identifier of the movie to deactivate
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was successfully deactivated,
 *    Enums::ProcessStatus::FAILED if the movie ID was not found
 */
Enums::ProcessStatus MovieManagementService::deactivateMovie(const std::string& movieId)
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getMovieId() == movieId && isMovieDeactivatable(movieId) == Enums::ProcessStatus::SUCCESS)
		{
			(iterator->second)->setStatus(Enums::MovieStatus::INACTIVE);
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: MovieManagementService::reactivateMovie
 * Description: Activates a movie by searching the datastore for the given movieId
 *              and updating its status to Enums::MovieStatus::ACTIVE if found.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId - Unique identifier of the movie to activate
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was found and activated,
 *    Enums::ProcessStatus::FAILED if the movieId does not exist in the datastore
 */
Enums::ProcessStatus MovieManagementService::reactivateMovie(const std::string& movieId)
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
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
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    title - Title of the movie to search for
 * Returns:
 *    A vector of constant Movie pointers representing the matching deactivated movies
 */
const std::vector<const Movie*> MovieManagementService::searchDeactivatedMovieByTitle(const std::string& title)
{
	ScopedLock lock(m_mutex);
	std::vector<const Movie*> resultantMovies;
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getTitle() == title && (iterator->second)->getStatus() == Enums::MovieStatus::INACTIVE)
		{
			resultantMovies.push_back(iterator->second);
		}
	}
	return resultantMovies;
}

/*
 * Function: MovieManagementService::getAllInactiveMovies
 * Description: Retrieves all movies from the DataStore that are currently marked as INACTIVE.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    None
 * Returns:
 *    A vector of constant Movie pointers representing all inactive movies
 */
std::vector<const Movie*> MovieManagementService::getAllInactiveMovies()
{
	ScopedLock lock(m_mutex);
	std::vector<const Movie*> allInactiveMovies;
	const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
	for (std::map<std::string, Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((iterator->second)->getStatus() == Enums::MovieStatus::INACTIVE)
		{
			allInactiveMovies.push_back(iterator->second);
		}
	}
	return allInactiveMovies;
}

/*
 * Function: MovieManagementService::isMovieDeactivatable
 * Description: Checks whether a movie can be deactivated by verifying if it has
 *              any scheduled or running shows. If such shows exist, the movie
 *              cannot be deactivated.
 *              Ensures thread safety by acquiring a scoped lock during the operation.
 * Parameters:
 *    movieId - Unique identifier of the movie to check
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie can be deactivated,
 *    Enums::ProcessStatus::FAILED if the movie has active shows
 */
Enums::ProcessStatus MovieManagementService::isMovieDeactivatable(const std::string& movieId)
{
	ScopedLock lock(m_mutex);
	int showCount = 0;
	const std::map<std::string, Show*>& shows = m_dataStore.getShows();
	for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
	{
		if (iterator->second && iterator->second->getMovie() && iterator->second->getMovie()->getMovieId() == movieId)
		{
			if (iterator->second->getShowStatus() == Enums::ShowStatus::SCHEDULED || iterator->second->getShowStatus() == Enums::ShowStatus::RUNNING)
			{
				++showCount;
			}
		}
	}
	if (showCount > 0)
	{
		return Enums::ProcessStatus::FAILED;
	}
	return Enums::ProcessStatus::SUCCESS;
}