#include <sstream>
#include <iomanip>
#include "TheatreManagementService.h"
#include "ShowManagementService.h"
#include "Factory.h"
#include "ScreenManagementService.h"
/*
     * Function: TheatreManagementService
     * Description: Default constructor that initializes the theatre management
     *              service with a reference to the shared DataStore instance.
     * Parameters: None
     * Returns: None
     */
TheatreManagementService::TheatreManagementService() :
    m_dataStore(DataStore::getInstance()),
    m_theatreMutex(config::MutexMappings::THEATRE_MUTEX_NAME),
    m_showMutex(config::MutexMappings::SHOW_MUTEX_NAME),
    m_movieMutex(config::MutexMappings::MOVIE_MUTEX_NAME),
    m_userMutex(config::MutexMappings::USER_MUTEX_NAME),
    m_screenMutex(config::MutexMappings::SCREEN_MUTEX_NAME)
{
}

/*
     * Function: deactivateTheatre
     * Description: Deactivates an active theatre, making it unavailable for bookings.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     * Returns: True if the theatre is successfully deactivated, false otherwise.
     */
Enums::ProcessStatus TheatreManagementService::deactivateTheatre(const std::string& theatreId)
{
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock showLock(m_showMutex);
    ScreenManagementService screenManagementService;
    const std::map<std::string, Screen*>& screens = m_dataStore.getScreens();
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (theatre == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    Enums::ProcessStatus screenDeactivateStatus = Enums::ProcessStatus::FAILED;
    for (std::map<std::string, Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getTheatre() && iterator->second->getTheatre()->getTheatreId() == theatreId)
        {
            screenDeactivateStatus = screenManagementService.deactivateScreen(theatreId, iterator->second->getScreenId());
        }
    }
    if (screenDeactivateStatus == Enums::ProcessStatus::FAILED)
    {
        return Enums::ProcessStatus::FAILED;
    }
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
    {
        if (iterator->second && iterator->second->getScreen() && iterator->second->getScreen()->getTheatre() && iterator->second->getScreen()->getTheatre()->getTheatreId() == theatreId)
        {
            Show* show = m_dataStore.getShowByIdForUpdation(iterator->second->getShowId());
            if (show)
            {
                m_dataStore.updateShowStatus(show->getShowId(), Enums::ShowStatus::CANCELLED);
                show->setShowStatus(Enums::ShowStatus::CANCELLED);
            }
        }
    }
    return Enums::ProcessStatus::SUCCESS;
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
     * Function: addMovieToTheatre
     * Description: Associates a movie with a specific theatre.
     * Parameters:
     *   - theatreId: Unique identifier of the theatre.
     *   - movieId: Unique identifier of the movie.
     * Returns: True if the movie is successfully added to the theatre, false otherwise.
     */
Enums::ProcessStatus TheatreManagementService::addMovieToTheatre(const std::string& theatreId, const std::string& movieId)
{
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock movieLock(m_movieMutex);
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    Movie* movie = m_dataStore.getMovieById(movieId);
    if (theatre == nullptr || movie == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (isMovieAlreadyExistsInTheatre(theatre, movieId))
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (m_dataStore.addMovieToTheatre(theatreId, movieId) != Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::FAILED;
    }
    theatre->addMovieToTheatre(movie);
    return Enums::ProcessStatus::SUCCESS;
}

/*
     * Function: listAllTheatres
     * Description: Retrieves a list of all theatres in the system.
     * Parameters: None
     * Returns: Vector of Theatre pointers representing all theatres.
     */
std::vector<const Theatre*> TheatreManagementService::listAllTheatres()
{
    ScopedLock lock(m_theatreMutex);
    std::vector<const Theatre*> theatres;
    const std::map<std::string, Theatre*>& allTheatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = allTheatres.begin(); iterator != allTheatres.end(); ++iterator)
    {
        theatres.push_back(iterator->second);
    }
    return theatres;
}

/*
     * Function: searchByTheatreName
     * Description: Searches for a theatre by its name.
     * Parameters:
     *   - name: Name of the theatre to search for.
     * Returns: Pointer to the Theatre object if found, or nullptr otherwise.
     */
const std::vector<const Theatre*> TheatreManagementService::searchByTheatreName(const std::string& theatreName)
{
    ScopedLock lock(m_theatreMutex);
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
    ScopedLock lock(m_theatreMutex);
    std::vector<const Theatre*> ownerTheatres;
    const User* authenticatedUser = m_dataStore.getAuthenticatedUser();
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second != nullptr) && ((iterator->second)->getTheatreOwner()->getUserId() == authenticatedUser->getUserId()) && (iterator->second->getStatus() == Enums::TheatreStatus::ACTIVE))
        {
            ownerTheatres.push_back(iterator->second);
        }
    }
    return ownerTheatres;
}

/*
Function Name : getCurrentOwnerInactiveTheatres
Description   : Retrieves all theatres owned by the currently authenticated user
                that are marked with status INACTIVE. Iterates through the datastore’s
                theatre collection, filters by ownership and inactive status, and
                returns the matching theatres.
Parameters    : None
Return Type   : const std::vector<const Theatre*>
*/
const std::vector<const Theatre*> TheatreManagementService::getCurrentOwnerInactiveTheatres()
{
    ScopedLock lock(m_theatreMutex);
    std::vector<const Theatre*> ownerTheatres;
    const User* authenticatedUser = m_dataStore.getAuthenticatedUser();
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second != nullptr) && ((iterator->second)->getTheatreOwner()->getUserId() == authenticatedUser->getUserId()) && (iterator->second->getStatus() == Enums::TheatreStatus::INACTIVE))
        {
            ownerTheatres.push_back(iterator->second);
        }
    }
    return ownerTheatres;
}

/*
 * Function: TheatreManagementService::getCurrentOwnerTheatreIds
 * Description: Retrieves the unique IDs of theatres owned by the currently
 *              authenticated user. Calls getCurrentOwnerTheatres and extracts
 *              the theatreId from each theatre.
 * Parameters: None
 * Returns:
 *    const std::vector<std::string> - Vector containing theatre IDs owned by
 *                                     the current authenticated user.
 */
const std::vector<std::string> TheatreManagementService::getCurrentOwnerTheatreIds()
{
    const std::vector<const Theatre*> theatres = getCurrentOwnerTheatres();
    std::vector<std::string> theatreIds;
    for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        theatreIds.push_back((*iterator)->getTheatreId());
    }
    return theatreIds;
}

/*
 * Function: TheatreManagementService::getMoviesFromTheatre
 * Description: Retrieves all movies associated with a specific theatre owned
 *              by the currently authenticated user. Iterates through the
 *              owner’s theatres, matches the given theatreId, and collects
 *              all movies linked to that theatre.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    const std::vector<const Movie*> - Vector of Movie pointers representing
 *                                      movies available in the specified theatre.
 */
const std::vector<const Movie*> TheatreManagementService::getMoviesFromTheatre(const std::string& theatreId)
{
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock movieLock(m_movieMutex);
    std::vector<const Theatre*> theatres = getCurrentOwnerTheatres();
    std::vector<const Movie*> theatreMovies;
    for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((*iterator)->getTheatreId() == theatreId)
        {
            const std::vector<Movie*>& movies = (*iterator)->getMovies();
            for (std::vector<Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
            {
                theatreMovies.push_back((*iterator));
            }
        }
    }
    return theatreMovies;
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
    ScopedLock lock(m_theatreMutex);
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
    ScopedLock lock(m_theatreMutex);
    const int theatresCount = m_dataStore.getTheatresCount();
    int idNumber = theatresCount + 1;
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
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock userLock(m_userMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatrePhoneNumber() == phoneNumber) 
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
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
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock userLock(m_userMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreEmail() == email)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    const std::map<std::string, User*>& users = m_dataStore.getUsers();
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
    ScopedLock lock(m_theatreMutex);
    Theatre* theatre = Factory::getObject<Theatre>(generateTheatreId(), name, city, address, phoneNumber, email, m_dataStore.getAuthenticatedUser());
    if (theatre != nullptr)
    {
        m_dataStore.addTheatre(theatre);
        std::string message = "New Theatre has been added : " + name;
        m_event.notify("",getAllAdminsId(), message);
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
    ScopedLock lock(m_theatreMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getName() == name && (iterator->second)->getCity() == city && (iterator->second)->getAddress() == address)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: TheatreManagementService::setTheatreNameById
 * Description: Updates the name of a theatre identified by the given
 *              theatre ID. Searches the datastore for the matching
 *              theatre and updates its name if found.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    name      - New name to be assigned to the theatre.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre name was updated successfully.
 *    ProcessStatus::FAILED if no matching theatre was found.
 */
Enums::ProcessStatus TheatreManagementService::setTheatreNameById(const std::string& theatreId, const std::string& name)
{
    ScopedLock lock(m_theatreMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            if (m_dataStore.updateTheatreName(theatreId, name) != Enums::ProcessStatus::SUCCESS)
            {
                return Enums::ProcessStatus::FAILED;
            }
            (iterator->second)->setName(name);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::setTheatreCityById
 * Description: Updates the city of a theatre identified by the given
 *              theatre ID. Searches the datastore for the matching
 *              theatre and updates its city if found.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    city      - New city to be assigned to the theatre.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre city was updated successfully.
 *    ProcessStatus::FAILED if no matching theatre was found.
 */
Enums::ProcessStatus TheatreManagementService::setTheatreCityById(const std::string& theatreId, const std::string& city)
{
    ScopedLock lock(m_theatreMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            if (m_dataStore.updateTheatreCity(theatreId, city) != Enums::ProcessStatus::SUCCESS)
            {
                return Enums::ProcessStatus::FAILED;
            }
            (iterator->second)->setCity(city);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::setTheatreAddressById
 * Description: Updates the address of a theatre identified by the given
 *              theatre ID. Searches the datastore for the matching
 *              theatre and updates its address if found.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    address   - New address to be assigned to the theatre.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre address was updated successfully.
 *    ProcessStatus::FAILED if no matching theatre was found.
 */
Enums::ProcessStatus TheatreManagementService::setTheatreAddressById(const std::string& theatreId, const std::string& address)
{
    ScopedLock lock(m_theatreMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            if (m_dataStore.updateTheatreAddress(theatreId, address) != Enums::ProcessStatus::SUCCESS)
            {
                return Enums::ProcessStatus::FAILED;
            }
            (iterator->second)->setAddress(address);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::setTheatrePhoneNumberById
 * Description: Updates the phone number of a theatre identified by the
 *              given theatre ID. Searches the datastore for the matching
 *              theatre and updates its phone number if found.
 * Parameters:
 *    theatreId   - Unique identifier of the theatre.
 *    phoneNumber - New phone number to be assigned to the theatre.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre phone number was updated successfully.
 *    ProcessStatus::FAILED if no matching theatre was found.
 */
Enums::ProcessStatus TheatreManagementService::setTheatrePhoneNumberById(const std::string& theatreId, const std::string& phoneNumber)
{
    ScopedLock lock(m_theatreMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            if (m_dataStore.updateTheatrePhoneNumber(theatreId, phoneNumber) != Enums::ProcessStatus::SUCCESS)
            {
                return Enums::ProcessStatus::FAILED;
            }
            (iterator->second)->setTheatrePhoneNumber(phoneNumber);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::setTheatreEmailById
 * Description: Updates the email address of a theatre identified by the
 *              given theatre ID. Searches the datastore for the matching
 *              theatre and updates its email address if found.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    email     - New email address to be assigned to the theatre.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre email was updated successfully.
 *    ProcessStatus::FAILED if no matching theatre was found.
 */
Enums::ProcessStatus TheatreManagementService::setTheatreEmailById(const std::string& theatreId, const std::string& email)
{
    ScopedLock lock(m_theatreMutex);
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            if (m_dataStore.updateTheatreEmail(theatreId, email) != Enums::ProcessStatus::SUCCESS)
            {
                return Enums::ProcessStatus::FAILED;
            }
            (iterator->second)->setTheatreEmail(email);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::getPendingTheatres
 * Description: Retrieves all theatres that are currently marked with
 *              pending status from the datastore.
 *              Iterates through all stored theatres and collects
 *              theatres whose status is PENDING.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers containing all pending theatres.
 *    Returns an empty vector if no pending theatres are found.
 */
const std::vector<const Theatre*> TheatreManagementService::getPendingTheatres()
{
    ScopedLock lock(m_theatreMutex);
    std::vector<const Theatre*> theatresList;
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if((iterator->second)->getStatus()==Enums::TheatreStatus::PENDING)
        {
            theatresList.push_back(iterator->second);
        }
    }
    return theatresList;
}

/*
 * Function: TheatreManagementService::setTheatreStatusById
 * Description: Updates the status of a theatre identified by the given
 *              theatre ID. Searches the datastore for the matching
 *              theatre and updates its status if found.
 * Parameters:
 *    theatreId    - Unique identifier of the theatre.
 *    theatreStatus - New status to be assigned to the theatre.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre status was updated successfully.
 *    ProcessStatus::FAILED if no matching theatre was found.
 */
Enums::ProcessStatus TheatreManagementService::setTheatreStatusById(const std::string& theatreId, Enums::TheatreStatus& theatreStatus)
{
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock screenLock(m_screenMutex);
    bool isTheatreDeactivatable = false;
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    for (std::map<std::string, Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
    {
        if ((iterator->second)->getTheatreId() == theatreId)
        {
            if (theatreStatus == Enums::TheatreStatus::INACTIVE)
            {
                const std::vector<Screen*>& screens = (iterator->second)->getScreens();
                if (screens.empty())
                {
                    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
                    if (theatre && (m_dataStore.updateTheatreStatus(theatreId, Enums::TheatreStatus::INACTIVE) == Enums::ProcessStatus::SUCCESS)
                        && (m_dataStore.clearMoviesFromTheatre(theatreId) == Enums::ProcessStatus::SUCCESS))
                    {
                        theatre->setMovies({});
                        theatre->setStatus(Enums::TheatreStatus::INACTIVE);
                        return Enums::ProcessStatus::SUCCESS;
                    }
                    return Enums::ProcessStatus::FAILED;
                }
                for (std::vector<Screen*>::const_iterator screenIterator = screens.begin(); screenIterator != screens.end(); ++screenIterator)
                {
                    if (isScreenDeactivatable((*screenIterator)) == Enums::ProcessStatus::SUCCESS)
                    {
                        isTheatreDeactivatable = true;
                    }
                    else
                    {
                        isTheatreDeactivatable = false;
                    }
                }
                if (!isTheatreDeactivatable)
                {
                    return Enums::ProcessStatus::FAILED;
                }
            }
            else if (m_dataStore.getAuthenticatedUser()->getUserType() == Enums::UserType::ADMIN && theatreStatus == Enums::TheatreStatus::ACTIVE)
            {
                if (m_dataStore.updateTheatreStatus(iterator->second->getTheatreId(), Enums::TheatreStatus::ACTIVE) == Enums::ProcessStatus::SUCCESS)
                {
                    if (iterator->second == nullptr)
                    {
                        return Enums::ProcessStatus::FAILED;
                    }
                    (iterator->second)->setStatus(Enums::TheatreStatus::ACTIVE);
                    std::string message = "Your theatre " + (iterator->second)->getName() + " has been approved!";
                    m_event.notify("", (iterator->second)->getTheatreOwner()->getUserId(), message);
                    m_notificationManagementService.sendNotification((iterator->second)->getTheatreOwner(), message);
                    return Enums::ProcessStatus::SUCCESS;
                }
            }
            else if (m_dataStore.getAuthenticatedUser()->getUserType() == Enums::UserType::ADMIN && theatreStatus == Enums::TheatreStatus::PENDING)
            {
                if (m_dataStore.updateTheatreStatus(iterator->second->getTheatreId(), Enums::TheatreStatus::PENDING) == Enums::ProcessStatus::SUCCESS)
                {
                    (iterator->second)->setStatus(Enums::TheatreStatus::PENDING);
                    return Enums::ProcessStatus::SUCCESS;
                }
            }
            else if (theatreStatus == Enums::TheatreStatus::PENDING)
            {
                if (m_dataStore.updateTheatreStatus(iterator->second->getTheatreId(), Enums::TheatreStatus::PENDING) == Enums::ProcessStatus::SUCCESS)
                {
                    (iterator->second)->setStatus(Enums::TheatreStatus::PENDING);
                    return Enums::ProcessStatus::SUCCESS;
                }
            }
            else
            {
                return Enums::ProcessStatus::FAILED;
            }
        }
    }
    Enums::ProcessStatus deactivateStatus = deactivateTheatre(theatreId);
    if (deactivateStatus == Enums::ProcessStatus::SUCCESS)
    {
        Theatre* theatre = m_dataStore.getTheatreById(theatreId);
        if (theatre && (m_dataStore.updateTheatreStatus(theatreId, Enums::TheatreStatus::INACTIVE) == Enums::ProcessStatus::SUCCESS)
            && (m_dataStore.clearMoviesFromTheatre(theatreId) == Enums::ProcessStatus::SUCCESS))
        {
            theatre->setMovies({});
            theatre->setStatus(Enums::TheatreStatus::INACTIVE);
            std::string message = "Your theatre " + theatre->getName() + "has been Rejected!";
            m_event.notify("", theatre->getTheatreOwner()->getUserId(), message);
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    else
    {
        return Enums::ProcessStatus::FAILED;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::isScreenDeactivatable
 * Description: Determines whether a given screen can be deactivated. Acquires locks
 *              on both the show and screen mutexes to ensure thread-safe access.
 *              Iterates through all shows in the DataStore, checking if any show
 *              is associated with the provided screen. If a match is found, delegates
 *              to ShowManagementService::isShowChangable to verify whether the show
 *              linked to the screen can be modified or removed. Returns the result
 *              of that check. If no associated show is found, returns FAILED.
 * Parameters:
 *    screen - A pointer to the Screen object to be checked for deactivation eligibility.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the screen can be deactivated,
 *    Enums::ProcessStatus::FAILED otherwise.
 */
Enums::ProcessStatus TheatreManagementService::isScreenDeactivatable(Screen* screen)
{
    ScopedLock showLock(m_showMutex);
    ScopedLock screenLock(m_screenMutex);
    ShowManagementService showManagementService;
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
    {
        if ((iterator->second) && (iterator->second->getScreen()))
        {
            if (screen->getScreenId() == (iterator->second)->getScreen()->getScreenId())
            {
                return showManagementService.isShowChangable((iterator->second)->getShowId());
            }
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::removeMovieFromTheatre
 * Description: Attempts to remove a Movie from a Theatre in the DataStore.
 *              Validates that the theatre exists, is active, and that the movie exists.
 *              Ensures the movie is not currently associated with any active Show in the theatre.
 *              If the movie is found in the theatre’s movie list and not in use, it is removed.
 *              Returns SUCCESS if removal is successful, otherwise FAILED.
 * Parameters:
 *    theatreId - The unique identifier of the Theatre.
 *    movieId   - The unique identifier of the Movie to be removed.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was removed successfully.
 *    Enums::ProcessStatus::FAILED if validation fails or the movie cannot be removed.
 */
Enums::ProcessStatus TheatreManagementService::removeMovieFromTheatre(const std::string& theatreId, const std::string& movieId)
{
    ScopedLock theatreLock(m_theatreMutex);
    ScopedLock showLock(m_showMutex);
    ScopedLock movieLock(m_movieMutex);
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    User* currentOwner = m_dataStore.getAuthenticatedUser();
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (theatre == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (theatre->getStatus() != Enums::TheatreStatus::ACTIVE)
    {
        return Enums::ProcessStatus::FAILED;
    }
    Movie* movie = m_dataStore.getMovieById(movieId);
    if (movie == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
    {
        Show* show = iterator->second;
        if (show == nullptr)
        {
            continue;
        }
        if (show->getScreen()->getTheatre()->getTheatreId() == theatreId && show->getMovie()->getMovieId() == movieId)
        {
            return Enums::ProcessStatus::FAILED;
        }
    }
    std::vector<Movie*>& movies = theatre->getMoviesForUpdation();
    for (std::vector<Movie*>::iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
    {
        if ((*iterator)->getMovieId() == movieId)
        {
            if (m_dataStore.removeMovieFromTheatre(theatre->getTheatreId(), movieId) == Enums::ProcessStatus::SUCCESS)
            {
                movies.erase(iterator);
                return Enums::ProcessStatus::SUCCESS;
            }
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TheatreManagementService::getAllAdminOwnersId
 * Description: Retrieves the IDs of all active admin users from the system.
 *              Iterates through the user map in the DataStore, checks each user’s
 *              status and type, and collects the IDs of those who are active and
 *              classified as ADMIN. Returns the list of admin user IDs for use
 *              in operations such as managing theatres, approving requests, or
 *              sending system-wide notifications.
 * Parameters:
 *    None
 * Returns:
 *    A std::vector<std::string> containing the user IDs of all active admin users.
 */
std::vector<std::string> TheatreManagementService::getAllAdminsId()
{
    std::vector<std::string> theatreOwnerIds;
    const std::map<std::string, User*> users = m_dataStore.getUsers();
    for (auto theatreOwner : users)
    {
        if (theatreOwner.second->getStatus() == Enums::UserStatus::ACTIVE && theatreOwner.second->getUserType() == Enums::UserType::ADMIN)
        {
            theatreOwnerIds.push_back(theatreOwner.second->getUserId());
        }
    }
    return theatreOwnerIds;
}