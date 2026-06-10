/*
 * File: TheatreManagementService.h
 * Description: Declares the TheatreManagementService class, which provides
 *              functionality for managing theatres within the system.
 *              This service handles operations such as adding, updating,
 *              activating/deactivating theatres, associating movies, and
 *              retrieving theatre details and lists.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <vector>
#include "FileManagement.h"
#include "Theatre.h"
#include "DataStore.h"

class TheatreManagementService
{
    DataStore& m_dataStore;
    const std::string& PATH = config::File::THEATRE_FILEPATH;
public:
    TheatreManagementService();
    bool updateTheatreDetails(const std::string& theatreId, const std::string& name, const std::string& address, const std::string& phone, const std::string& email);
    bool reactivateTheatre(const std::string& theatreId);
    Enums::ProcessStatus deactivateTheatre(const std::string& theatreId);
    Theatre* viewTheatreDetails(const std::string& theatreId) const;
    int viewTheatreStatus(const std::string& theatreId) const;
    std::vector<const Theatre*> listAllTheatres() const;
    std::vector<Theatre*> listTheatresByCity(const std::string& city) const;
    const std::vector<const Theatre*> searchByTheatreName(const std::string& theatreName) const;
    const std::vector<const Theatre*> getCurrentOwnerTheatres();
    const std::vector<const Theatre*> getCurrentOwnerInactiveTheatres();
    const std::vector<std::string> getCurrentOwnerTheatreIds();
    const std::vector<const Movie*> getMoviesFromTheatre(const std::string& theatreId);
    const User* getAuthenticatedUser() const;
    const std::vector<const Theatre*> getAllTheatres();
    Enums::ProcessStatus isTheatrePhoneNumberUnique(const std::string& phoneNumber);
    Enums::ProcessStatus isTheatreEmailUnique(const std::string& email);
    Enums::ProcessStatus addTheatre(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
    const std::string generateTheatreId();
    Enums::ProcessStatus isTheatreUniqueInSystem(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
    Enums::ProcessStatus setTheatreNameById(const std::string& theatreId, const std::string& name);
    Enums::ProcessStatus setTheatreCityById(const std::string& theatreId, const std::string& city);
    Enums::ProcessStatus setTheatreAddressById(const std::string& theatreId, const std::string& address);
    Enums::ProcessStatus setTheatrePhoneNumberById(const std::string& theatreId, const std::string& phoneNumber);
    Enums::ProcessStatus setTheatreEmailById(const std::string& theatreId, const std::string& email);
    const std::vector<const Theatre*> getPendingTheatres();
    Enums::ProcessStatus setTheatreStatusById(const std::string& theatreId, Enums::TheatreStatus& theatreStatus);
    Enums::ProcessStatus addMovieToTheatre(const std::string& theatreId, const std::string& movieId);
    Theatre* getTheatreById(const std::string& theatreId);
    Movie* getMovieById(const std::string& movieId);
    bool isMovieAlreadyExistsInTheatre(Theatre* theatre, const std::string& movieId);
    Enums::ProcessStatus removeMovieFromTheatre(const std::string& theatreId, const std::string& movieId);
    Enums::ProcessStatus isScreenDeactivatable(Screen* screen);
    void saveTheatreData();
    void loadTheatreData();
};