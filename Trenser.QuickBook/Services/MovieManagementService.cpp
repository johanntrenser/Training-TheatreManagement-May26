#include "MovieManagementService.h"

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

