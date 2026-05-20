#pragma once
#include <string>
#include <vector>
#include "Theatre.h"
#include "DataStore.h"

class TheatreManagementService
{
    DataStore& m_dataStore;
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
};

