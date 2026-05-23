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
const std::vector<const Theatre*> TheatreManagementService::searchByTheatreName(const std::string& theatreName) const
{
    std::vector<const Theatre*> resultantTheatres;
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getName() == theatreName && (iterator->second)->getStatus() == Enums::TheatreStatus::ACTIVE)
        {
            resultantTheatres.push_back(iterator->second);
        }
    }
    return resultantTheatres;
}


/*
 * Function: TheatreManagementService::getCurrentOwnerTheatres
 * Description: Retrieves all theatres owned by the currently authenticated user.
 *              Iterates through the datastore of theatres and collects those
 *              associated with the authenticated user account.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers representing the theatres owned by the
 *    current authenticated user. Returns an empty vector if no theatres
 *    are found for the user.
 */
const std::vector<const Theatre*> TheatreManagementService::getCurrentOwnerTheatres()
{
    std::vector<const Theatre*> ownerTheatres;
    const User* authenticatedUser = m_dataStore.getAuthenticatedUser();
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second != nullptr) && ((iterator->second)->getTheatreOwner() == authenticatedUser))
        {
            ownerTheatres.push_back(iterator->second);
        }
    }
    return ownerTheatres;
}

/*
 * Function: TheatreManagementService::getAuthenticatedUser
 * Description: Retrieves the currently authenticated user from the datastore
 *              through the TheatreManagementService. Provides access to the
 *              user object representing the active session.
 * Parameters: None
 * Returns:
 *    Pointer to the User object representing the authenticated user, or nullptr
 *    if no user is currently logged in.
 */
const User* TheatreManagementService::getAuthenticatedUser() const
{
    return m_dataStore.getAuthenticatedUser();
}

/*
 * Function: TheatreManagementService::getAllTheatres
 * Description: Retrieves all theatres stored in the datastore. Iterates through
 *              the internal theatre map and collects each theatre into a list
 *              for return.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers representing all theatres in the datastore.
 *    Returns an empty vector if no theatres are available.
 */
const std::vector<const Theatre*> TheatreManagementService::getAllTheatres()
{
    std::vector<const Theatre*> theatresList;
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        theatresList.push_back(iterator->second);
    }
    return theatresList;
}