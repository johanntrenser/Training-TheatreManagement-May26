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
#include "ScopedLock.h"
#include "NotificationEvent.h"

class MovieManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::MOVIE_FILEPATH;
	NamedMutex m_mutex;
	NotificationEvent m_event;
public:
	MovieManagementService();
	Enums::ProcessStatus addMovieToSystem(const std::string& title, const std::string& language, const std::string& genre, const int duration);
	const std::string generateMovieId();
	Enums::ProcessStatus isMovieUniqueInSystem(const std::string& title, const std::string& language, const std::string& genre, const int duration);
	const std::vector<const Movie*> searchMovieByTitle(const std::string& title);
	Enums::ProcessStatus setMovieTitleByID(const std::string& movieId, const std::string& title);
	Enums::ProcessStatus setMovieLanguageByID(const std::string& movieId, const std::string& language);
	Enums::ProcessStatus setMovieGenreByID(const std::string& movieId, const std::string& genre);
	Enums::ProcessStatus setMovieDurationByID(const std::string& movieId, const int& duration);
	std::vector<const Movie*> getAllActiveMovies();
	Enums::ProcessStatus deactivateMovie(const std::string& movieId);
	Enums::ProcessStatus reactivateMovie(const std::string& movieId);
	const std::vector<const Movie*> searchDeactivatedMovieByTitle(const std::string& title);
	std::vector<const Movie*> getAllInactiveMovies();
	Enums::ProcessStatus isMovieDeactivatable(const std::string& movieId);
	std::vector<std::string> getAllTheatreOwnersId();
};