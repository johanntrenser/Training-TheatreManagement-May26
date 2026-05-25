/*
 * File: ShowManagementService.h
 * Description: Declares the ShowManagementService class, which provides
 *              functionality for managing shows within the system.
 *              This service will handle operations such as creating,
 *              updating, retrieving, and deleting show records, as well
 *              as managing show schedules and related details.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <ctime>
#include "DataStore.h"

class ShowManagementService
{
private:
	DataStore& m_dataStore;
public:
	ShowManagementService() : m_dataStore(DataStore::getInstance()) {}
	const std::string generateShowId();
	const std::string generateShowSeatAvailabilityId();
	Enums::ProcessStatus isMovieInTheatre(const std::string& movieId, const std::string& theatreId);
	const std::vector<const Screen*> getScreensFromTheatre(const std::string& theatreId);
	Enums::ProcessStatus isShowTimeConflicting(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute);
	Enums::ProcessStatus addShow(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute);
};

