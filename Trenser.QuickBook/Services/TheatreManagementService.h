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
#include <fstream>
#include <sstream>
#include "FileManagement.h"
#include "Theatre.h"
#include "DataStore.h"
#include "ApplicationConfig.h"

class TheatreManagementService
{
    DataStore& m_dataStore;
    const std::string& PATH = config::File::THEATRE_FILEPATH;
public:
    TheatreManagementService();
    bool addTheatre(const std::string& name, const std::string& city, const std::string& address, const std::string& phone, const std::string& email);
    bool updateTheatreDetails(const std::string& theatreId, const std::string& name, const std::string& address, const std::string& phone, const std::string& email);
    bool reactivateTheatre(const std::string& theatreId);
    bool deactivateTheatre(const std::string& theatreId);
    bool addMovieToTheatre(const std::string& theatreId, const std::string& movieId);
    Theatre* viewTheatreDetails(const std::string& theatreId) const;
    int viewTheatreStatus(const std::string& theatreId) const; // enum placeholder
    std::vector<Theatre*> listAllTheatres() const;
    std::vector<Theatre*> listTheatresByCity(const std::string& city) const;
    Theatre* searchByTheatreName(const std::string& name) const;
    void saveTheatreData();
    void loadTheatreData();
};