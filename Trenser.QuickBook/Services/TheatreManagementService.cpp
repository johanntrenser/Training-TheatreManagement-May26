#include "TheatreManagementService.h"

/*
     * Function: TheatreManagementService
     * Description: Default constructor that initializes the theatre management
     *              service with a reference to the shared DataStore instance.
     * Parameters: None
     * Returns: None
     */
TheatreManagementService::TheatreManagementService() :
    m_dataStore(DataStore::getInstance())
{
}

/*
     * Function: addTheatre
     * Description: Adds a new theatre to the system with the provided details.
     * Parameters:
     *   - name: Name of the theatre.
     *   - city: City where the theatre is located.
     *   - address: Address of the theatre.
     *   - phone: Contact phone number of the theatre.
     *   - email: Contact email of the theatre.
     * Returns: True if the theatre is successfully added, false otherwise.
     */
bool TheatreManagementService::addTheatre(const std::string& name,
    const std::string& city,
    const std::string& address,
    const std::string& phone,
    const std::string& email)
{
    return true;
}

/*
     * Function: updateTheatreDetails
     * Description: Updates the details of an existing theatre.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     *   - name: Updated name of the theatre.
     *   - address: Updated address of the theatre.
     *   - phone: Updated contact phone number.
     *   - email: Updated contact email.
     * Returns: True if the details are successfully updated, false otherwise.
     */
bool TheatreManagementService::updateTheatreDetails(const std::string& theatreId,
    const std::string& name,
    const std::string& address,
    const std::string& phone,
    const std::string& email)
{
    return true;
}

/*
     * Function: reactivateTheatre
     * Description: Reactivates a previously deactivated theatre.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     * Returns: True if the theatre is successfully reactivated, false otherwise.
     */
bool TheatreManagementService::reactivateTheatre(const std::string& theatreId)
{
    return true;
}

/*
     * Function: deactivateTheatre
     * Description: Deactivates an active theatre, making it unavailable for bookings.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     * Returns: True if the theatre is successfully deactivated, false otherwise.
     */
bool TheatreManagementService::deactivateTheatre(const std::string& theatreId)
{
    return true;
}

/*
     * Function: addMovieToTheatre
     * Description: Associates a movie with a specific theatre.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     *   - movieId: Unique identifier of the movie.
     * Returns: True if the movie is successfully added to the theatre, false otherwise.
     */
bool TheatreManagementService::addMovieToTheatre(const std::string& theatreId,
    const std::string& movieId)
{
    return true;
}

/*
     * Function: viewTheatreDetails
     * Description: Retrieves detailed information about a specific theatre.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     * Returns: Pointer to the Theatre object containing details, or nullptr if not found.
     */
Theatre* TheatreManagementService::viewTheatreDetails(const std::string& theatreId) const
{
    return nullptr;
}

/*
     * Function: viewTheatreStatus
     * Description: Retrieves the current status of a theatre (active/inactive).
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     * Returns: Integer representing the theatre status (enum placeholder).
     */
int TheatreManagementService::viewTheatreStatus(const std::string& theatreId) const
{
    return 0; // placeholder enum value
}

/*
     * Function: listAllTheatres
     * Description: Retrieves a list of all theatres in the system.
     * Parameters: None
     * Returns: Vector of Theatre pointers representing all theatres.
     */
std::vector<Theatre*> TheatreManagementService::listAllTheatres() const
{
    return {};
}

/*
     * Function: listTheatresByCity
     * Description: Retrieves a list of theatres located in a specific city.
     * Parameters:
     *   - city: Name of the city.
     * Returns: Vector of Theatre pointers representing theatres in the given city.
     */
std::vector<Theatre*> TheatreManagementService::listTheatresByCity(const std::string& city) const
{
    return {};
}

/*
     * Function: searchByTheatreName
     * Description: Searches for a theatre by its name.
     * Parameters:
     *   - name: Name of the theatre to search for.
     * Returns: Pointer to the Theatre object if found, or nullptr otherwise.
     */
Theatre* TheatreManagementService::searchByTheatreName(const std::string& name) const
{
    return nullptr;
}

/*
 * Function: TheatreManagementService::saveTheatreData
 * Description: Saves all theatre data from the DataStore into a CSV file.
 *              Includes theatre details, status, associated screens, and movies.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void TheatreManagementService::saveTheatreData()
{
    const std::map<std::string, Theatre*> theatres = m_dataStore.getTheatres();
    std::ofstream theatreFile(PATH, std::ios::trunc);
    if (!theatreFile.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    theatreFile << "THEATRE ID,THEATRE NAME,CITY,ADDRESS,PHONE NUMBER,EMAIL,THEATRE OWNER, STATUS, SCREENS NAME, MOVIES TITLE\n";
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        theatreFile << (iterator->second)->getTheatreId() << ","
            << (iterator->second)->getName() << ","
            << (iterator->second)->getCity() << ","
            << (iterator->second)->getAddress() << ","
            << (iterator->second)->getTheatrePhoneNumber() << ","
            << (iterator->second)->getTheatreEmail() << ","
            << (iterator->second)->getTheatreOwner() << ","
            << Enums::getTheatreStatusString((iterator->second)->getStatus()) << ",";
        for (std::vector<Screen*>::const_iterator screenItertor = (iterator->second)->getScreens().begin(); screenItertor != (iterator->second)->getScreens().end(); ++screenItertor)
        {
            theatreFile << (*screenItertor)->getName();
            if (screenItertor + 1 != (iterator->second)->getScreens().end())
            {
                theatreFile << "|";
            }
        }
        theatreFile << ",";
        for (std::vector<Movie*>::const_iterator movieItertor = (iterator->second)->getMovies().begin(); movieItertor != (iterator->second)->getMovies().end(); ++movieItertor)
        {
            theatreFile << (*movieItertor)->getTitle();
            if (movieItertor + 1 != (iterator->second)->getMovies().end())
            {
                theatreFile << "|";
            }
        }
        theatreFile << ",";
    }
    theatreFile.close();
}