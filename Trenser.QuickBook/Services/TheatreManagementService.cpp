#include "TheatreManagementService.h"

TheatreManagementService::TheatreManagementService() :
    m_dataStore(DataStore::getInstance())
{}

bool TheatreManagementService::addTheatre(const std::string& name,
    const std::string& city,
    const std::string& address,
    const std::string& phone,
    const std::string& email)
{
    return true;
}

bool TheatreManagementService::updateTheatreDetails(const std::string& theatreId,
    const std::string& name,
    const std::string& address,
    const std::string& phone,
    const std::string& email)
{
    return true;
}

bool TheatreManagementService::reactivateTheatre(const std::string& theatreId)
{
    return true;
}

bool TheatreManagementService::deactivateTheatre(const std::string& theatreId)
{
    return true;
}

bool TheatreManagementService::addMovieToTheatre(const std::string& theatreId,
    const std::string& movieId)
{
    return true;
}

Theatre* TheatreManagementService::viewTheatreDetails(const std::string& theatreId) const
{
    return nullptr;
}

int TheatreManagementService::viewTheatreStatus(const std::string& theatreId) const
{
    return 0; // placeholder enum value
}

std::vector<Theatre*> TheatreManagementService::listAllTheatres() const
{
    return {};
}

std::vector<Theatre*> TheatreManagementService::listTheatresByCity(const std::string& city) const
{
    return {};
}

Theatre* TheatreManagementService::searchByTheatreName(const std::string& name) const
{
    return nullptr;
}
