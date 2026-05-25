#include <sstream>
#include <iomanip>
#include "TheatreManagementService.h"
#include "Factory.h"
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
 * Function: TheatreManagementService::isMovieAlreadyExistsInTheatre
 * Description: Checks whether a given movie already exists in the specified
 *              theatre. Iterates through the theatre's movie list and compares
 *              each movie's ID with the provided movie ID.
 * Parameters:
 *    theatre - Pointer to the Theatre object where the movie existence is to be checked.
 *    movieId - A string representing the unique identifier of the movie to be validated.
 * Returns:
 *    true  - If the movie with the given ID already exists in the theatre.
 *    false - If the movie does not exist in the theatre.
 */
bool TheatreManagementService::isMovieAlreadyExistsInTheatre(Theatre* theatre, const std::string& movieId)
{
    const std::vector<Movie*>& movies = theatre->getMovies();
    for (std::vector<Movie*>::const_iterator iterator =movies.begin(); iterator != movies.end(); ++iterator)
    {
        if ((*iterator)->getMovieId() == movieId)
        {
            return true;
        }
    }
    return false;
}

/*
 * Function: TheatreManagementService::getMovieById
 * Description: Retrieves a movie object from the datastore by its unique identifier.
 *              Iterates through the internal movie map and returns the corresponding
 *              Movie pointer if a match is found.
 * Parameters:
 *    movieId - A string representing the unique identifier of the movie to be retrieved.
 * Returns:
 *    Pointer to the Movie object if found.
 *    nullptr if no movie exists with the given ID.
 */
Movie* TheatreManagementService::getMovieById(const std::string& movieId)
{
    const std::map<std::string, Movie*>& movies = m_dataStore.getMovies();
    for (std::map<std::string, Movie*>::const_iterator iterator =movies.begin(); iterator != movies.end(); ++iterator)
    {
        if ((iterator->second)->getMovieId() == movieId)
        {
            return iterator->second;
        }
    }
    return nullptr;
}

/*
 * Function: TheatreManagementService::getTheatreById
 * Description: Retrieves a theatre object from the datastore by its unique identifier.
 *              Iterates through the internal theatre map and returns the corresponding
 *              Theatre pointer if a match is found.
 * Parameters:
 *    theatreId - A string representing the unique identifier of the theatre to be retrieved.
 * Returns:
 *    Pointer to the Theatre object if found.
 *    nullptr if no theatre exists with the given ID.
 */
Theatre* TheatreManagementService::getTheatreById(const std::string& theatreId)
{
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator =theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            return iterator->second;
        }
    }
    return nullptr;
}

/*
     * Function: addMovieToTheatre
     * Description: Associates a movie with a specific theatre.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     *   - movieId: Unique identifier of the movie.
     * Returns: True if the movie is successfully added to the theatre, false otherwise.
     */
Enums::ProcessStatus TheatreManagementService::addMovieToTheatre(const std::string& theatreId, const std::string& movieId)
{
    Theatre* theatre = getTheatreById(theatreId);
    Movie* movie = getMovieById(movieId);
    if (theatre == nullptr || movie == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (isMovieAlreadyExistsInTheatre(theatre, movieId))
    {
        return Enums::ProcessStatus::FAILED;
    }
    theatre->addMovieToTheatre(movie);
    return Enums::ProcessStatus::SUCCESS;
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

/*
 * Function: TheatreManagementService::generateTheatreId
 * Description: Generates a unique theatre ID based on the current number
 *              of theatres available in the data store.
 * Parameters:
 *    None
 * Returns:
 *    const std::string - Newly generated theatre ID
 */
const std::string TheatreManagementService::generateTheatreId()
{
    const std::map<std::string, Theatre*>& theatre = m_dataStore.getTheatres();
    int idNumber = static_cast<int>(theatre.size()) + 1;
    std::ostringstream buffer;
    buffer << "TH" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: TheatreManagementService::isTheatrePhoneNumberUnique
 * Description: Checks whether the provided theatre phone number is unique
 *              across all theatres and registered users in the system.
 * Parameters:
 *    phoneNumber (const std::string&) - Theatre phone number to validate
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the phone number is unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus TheatreManagementService::isTheatrePhoneNumberUnique(const std::string& phoneNumber)
{
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatrePhoneNumber() == phoneNumber) //function name
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if ((iterator->second)->getPhoneNumber() == phoneNumber)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: TheatreManagementService::isTheatreEmailUnique
 * Description: Checks whether the provided theatre email address is unique
 *              across all theatres and registered users in the system.
 * Parameters:
 *    email (const std::string&) - Theatre email address to validate
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the email address is unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus TheatreManagementService::isTheatreEmailUnique(const std::string& email)
{
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreEmail() == email)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
    {
        if ((iterator->second)->getEmail() == email)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: TheatreManagementService::addTheatre
 * Description: Creates and adds a new theatre to the system using the
 *              provided theatre details and authenticated user information.
 * Parameters:
 *    name (const std::string&) - Name of the theatre
 *    city (const std::string&) - City where the theatre is located
 *    address (const std::string&) - Address of the theatre
 *    phoneNumber (const std::string&) - Contact phone number of the theatre
 *    email (const std::string&) - Contact email address of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the theatre is created and added
 *                           successfully, FAILED otherwise
 */
Enums::ProcessStatus TheatreManagementService::addTheatre(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email)
{
    Theatre* theatre = Factory::getObject<Theatre>(generateTheatreId(), name, city, address, phoneNumber, email, m_dataStore.getAuthenticatedUser());
    if (theatre != nullptr)
    {
        m_dataStore.addTheatre(theatre);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::isTheatreUniqueInSystem
 * Description: Checks whether the provided theatre details already exist
 *              in the system.
 * Parameters:
 *    name (const std::string&) - Name of the theatre
 *    city (const std::string&) - City where the theatre is located
 *    address (const std::string&) - Address of the theatre
 *    phoneNumber (const std::string&) - Contact phone number of the theatre
 *    email (const std::string&) - Contact email address of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the theatre details are unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus TheatreManagementService::isTheatreUniqueInSystem(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email)
{
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getName() == name && (iterator->second)->getCity() == city && (iterator->second)->getAddress() == address && (iterator->second)->getTheatrePhoneNumber() == phoneNumber && (iterator->second)->getTheatreEmail() == email)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}