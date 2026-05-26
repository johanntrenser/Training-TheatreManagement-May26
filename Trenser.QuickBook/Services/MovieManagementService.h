/*
 * File: MovieManagementService.h
 * Description: Declares the MovieManagementService class, which provides
 *              functionality for managing movies within the system. This
 *              service will handle operations such as adding, retrieving,
 *              updating, and deleting movie records.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include "DataStore.h"

class MovieManagementService
{
private:
	DataStore& m_dataStore;
public:
	MovieManagementService();
	std::vector<const Movie*> getAllActiveMovies();
};

