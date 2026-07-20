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
#include "NamedMutex.h"
#include "ScopedLock.h"

class ShowManagementService
{
private:
	DataStore& m_dataStore;
	NamedMutex m_showMutex;
	NamedMutex m_screenMutex;
	NamedMutex m_theatreMutex;
	NamedMutex m_movieMutex;
public:
	ShowManagementService() : m_dataStore(DataStore::getInstance()),
		m_showMutex(config::MutexMappings::SHOW_MUTEX_NAME),
		m_screenMutex(config::MutexMappings::SCREEN_MUTEX_NAME),
		m_theatreMutex(config::MutexMappings::THEATRE_MUTEX_NAME),
		m_movieMutex(config::MutexMappings::MOVIE_MUTEX_NAME)
	{}
	const std::string generateShowId();
	const std::string generateShowSeatAvailabilityId();
	Enums::ProcessStatus isMovieInTheatre(const std::string& movieId, const std::string& theatreId);
	const std::vector<const Screen*> getScreensFromTheatre(const std::string& theatreId);
	Enums::ProcessStatus isShowTimeConflicting(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute);
	Enums::ProcessStatus isNewShowTimeConflicting(const std::string& showId, const time_t& newStartTime);
	Enums::ProcessStatus addShow(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute);
	Enums::ProcessStatus updateShow(const time_t& startTime, const std::string& showId);
	const std::vector<const Show*> getActiveShows();
	const std::vector<std::string> getActiveShowIds();
	const std::vector<const Show*> getAllShows();
	const std::vector<std::string> getAllShowIds();
	Enums::ShowStatus getShowStatus(const std::string& showId);
	const std::vector<const Show*> getShowsForMovie(const std::string& movieId);
	Enums::ProcessStatus isShowChangable(const std::string& showId);
	Enums::ProcessStatus setShowStatusById(const std::string& showId, Enums::ShowStatus status);
	const Show* getShowById(const std::string& showId);
	void updateShowStatuses();
	void updateTicketStatusesForCompletedShows();
};