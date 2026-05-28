/*
 * File: Controller.cpp
 * Description: Implements the Controller class responsible for managing
 *              various services including authentication, user, theatre,
 *              movie, show, booking, payment, ticket, notification, log,
 *              screen, seat, and refund management. Provides constructors
 *              for initialization and a destructor for cleanup.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "Controller.h"
using namespace::std;
#include<iostream>

 /*
  * Function: Controller::Controller
  * Description: Default constructor. Initializes all management service
  *              pointers to nullptr.
  * Returns: None
  */
Controller::Controller()
    : m_authenticationManagementService(nullptr),
    m_userManagementService(nullptr),
    m_theatreManagementService(nullptr),
    m_movieManagementService(nullptr),
    m_showManagementService(nullptr),
    m_bookingManagementService(nullptr),
    m_paymentManagementService(nullptr),
    m_ticketManagementService(nullptr),
    m_notificationManagementService(nullptr),
    m_logManagementService(nullptr),
    m_ScreenManagementService(nullptr),
    m_seatManagementService(nullptr),
    m_refundManagementService(nullptr)
{
}

/*
 * Function: Controller::Controller
 * Description: Parameterized constructor. Initializes all management service
 *              pointers with provided service instances.
 * Parameters:
 *    AuthenticationManagementService* authService - Authentication service
 *    UserManagementService* userService - User service
 *    TheatreManagementService* theatreService - Theatre service
 *    MovieManagementService* movieService - Movie service
 *    ShowManagementService* showService - Show service
 *    BookingManagementService* bookingService - Booking service
 *    PaymentManagementService* paymentService - Payment service
 *    TicketManagementService* ticketService - Ticket service
 *    NotificationManagementService* notificationService - Notification service
 *    LogManagementService* logService - Log service
 *    ScreenManagementService* screenService - Screen service
 *    SeatManagementService* seatService - Seat service
 *    RefundManagementService* refundService - Refund service
 * Returns: None
 */
Controller::Controller(AuthenticationManagementService* authService,
    UserManagementService* userService,
    TheatreManagementService* theatreService,
    MovieManagementService* movieService,
    ShowManagementService* showService,
    BookingManagementService* bookingService,
    PaymentManagementService* paymentService,
    TicketManagementService* ticketService,
    NotificationManagementService* notificationService,
    LogManagementService* logService,
    ScreenManagementService* screenService,
    SeatManagementService* seatService,
    RefundManagementService* refundService)
    : m_authenticationManagementService(authService),
    m_userManagementService(userService),
    m_theatreManagementService(theatreService),
    m_movieManagementService(movieService),
    m_showManagementService(showService),
    m_bookingManagementService(bookingService),
    m_paymentManagementService(paymentService),
    m_ticketManagementService(ticketService),
    m_notificationManagementService(notificationService),
    m_logManagementService(logService),
    m_ScreenManagementService(screenService),
    m_seatManagementService(seatService),
    m_refundManagementService(refundService)
{
}

/*
 * Function: Controller::registerUser
 * Description: Registers a new user by delegating the operation to the
 *              AuthenticationManagementService. Returns the process status
 *              indicating success or failure.
 * Parameters:
 *    userName (const std::string&) - The name of the user
 *    email (const std::string&) - The email address of the user
 *    password (const std::string&) - The password for the user
 *    phoneNumber (const std::string&) - The phone number of the user
 *    userType (Enums::UserType) - The type of user (Customer, Theatre Owner, Admin)
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the user is registered, FAILED otherwise
 */
Enums::ProcessStatus Controller::registerUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType)
{
    if (m_authenticationManagementService->registerUser(userName, email, password, phoneNumber, userType) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: Controller::isEmailUnique
 * Description: Validates whether the provided email address is unique by
 *              checking against existing users in the AuthenticationManagementService.
 * Parameters:
 *    email (const std::string&) - The email address to validate
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the email is unique, FAILED otherwise
 */
Enums::ProcessStatus Controller::isEmailUnique(const std::string& email)
{
    if (m_authenticationManagementService->isEmailIdUnique(email))
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: Controller::isPhoneNumberUnique
 * Description: Validates whether the provided phone number is unique by
 *              checking against existing users in the AuthenticationManagementService.
 * Parameters:
 *    phoneNumber (const std::string&) - The phone number to validate
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the phone number is unique, FAILED otherwise
 */
Enums::ProcessStatus Controller::isPhoneNumberUnique(const std::string& phoneNumber)
{
    if (m_authenticationManagementService->isPhoneNumberUnique(phoneNumber))
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: Controller::getLogsByType
 * Description: Retrieves logs filtered by the specified log type from the
 *              LogManagementService.
 * Parameters:
 *    const Enums::LogType logType - The type of logs to retrieve
 * Returns:
 *    const std::vector<const Log*> - Vector of log entries matching the type
 */
const std::vector<const Log*> Controller::getLogsByType(const Enums::LogType logType)
{
    return m_logManagementService->getLogsByType(logType);
}

/*
 * Function: Controller::addMovie
 * Description: Adds a new movie to the system through the MovieManagementService.
 * Parameters:
 *    title    - Title of the movie
 *    language - Language of the movie
 *    genre    - Genre of the movie
 *    duration - Duration of the movie in minutes
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was added successfully,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::addMovie(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
    if (m_movieManagementService->addMovieToSystem(title, language, genre, duration) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: Controller::isMovieUnique
 * Description: Verifies whether a movie with the given attributes already exists in the system.
 * Parameters:
 *    title    - Title of the movie
 *    language - Language of the movie
 *    genre    - Genre of the movie
 *    duration - Duration of the movie in minutes
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie is unique,
 *    Enums::ProcessStatus::FAILED if a duplicate exists
 */
Enums::ProcessStatus Controller::isMovieUnique(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
    return m_movieManagementService->isMovieUniqueInSystem(title, language, genre, duration);
}

/*
 * Function: Controller::searchMovieByTitleById
 * Description: Searches for movies in the system that match the given title.
 * Parameters:
 *    title - Title of the movie to search for
 * Returns:
 *    A vector of constant Movie pointers representing the matching movies
 */
const std::vector<const Movie*> Controller::searchMovieByTitle(const std::string& title)
{
    return m_movieManagementService->searchMovieByTitle(title);
}

/*
 * Function: Controller::setMovieTitleById
 * Description: Updates the title of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId - Unique identifier of the movie
 *    title   - New title to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::setMovieTitleByID(const std::string& movieId, const std::string& title)
{
    if (m_movieManagementService->setMovieTitleByID(movieId, title) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    else
    {
        return Enums::ProcessStatus::FAILED;
    }
}

/*
 * Function: Controller::setMovieGenreById
 * Description: Updates the genre of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId - Unique identifier of the movie
 *    genre   - New genre to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::setMovieGenreByID(const std::string& movieId, const std::string& genre)
{
    if (m_movieManagementService->setMovieGenreByID(movieId, genre) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    else
    {
        return Enums::ProcessStatus::FAILED;
    }
}

/*
 * Function: Controller::setMovieDuration
 * Description: Updates the duration of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    duration - New duration (in minutes) to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::setMovieDurationByID(const std::string& movieId, const int& duration)
{
    if (m_movieManagementService->setMovieDurationByID(movieId, duration) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    else
    {
        return Enums::ProcessStatus::FAILED;
    }
}

/*
 * Function: Controller::setMovieLanguageById
 * Description: Updates the language of a movie identified by its unique movie ID.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    language - New language to set for the movie
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the update was successful,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::setMovieLanguageByID(const std::string& movieId, const std::string& language)
{
    if (m_movieManagementService->setMovieLanguageByID(movieId, language) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    else
    {
        return Enums::ProcessStatus::FAILED;
    }
}

/*
 * Function: Controller::getCurrentOwnerTheatres
 * Description: Retrieves the list of theatres owned by the currently logged-in
 *              user. Delegates the request to the TheatreManagementService
 *              to fetch theatres associated with the current owner.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers representing the theatres owned by the
 *    current user. Returns an empty vector if no theatres are found.
 */
const std::vector<const Theatre*> Controller::getCurrentOwnerTheatres()
{
    return m_theatreManagementService->getCurrentOwnerTheatres();
}

/*
 * Function: Controller::getCurrentOwnerTheatreIds
 * Description: Retrieves the theatre IDs associated with the currently logged-in
 *              owner. Calls getCurrentOwnerTheatres to obtain the list of theatres
 *              and extracts their unique identifiers.
 * Parameters: None
 * Returns:
 *    const std::vector<std::string> - Vector containing theatre IDs owned by the
 *                                     current user
 */
const std::vector<std::string> Controller::getCurrentOwnerTheatreIds()
{
    return m_theatreManagementService->getCurrentOwnerTheatreIds();
}

/*
 * Function: Controller::getMoviesFromTheatre
 * Description: Retrieves all movies associated with a specific theatre.
 *              Delegates the request to the TheatreManagementService to
 *              fetch movies linked to the given theatre ID.
 * Parameters:
 *    theatreId (const std::string&) - The unique identifier of the theatre
 * Returns:
 *    const std::vector<const Movie*> - Vector of Movie pointers representing
 *                                      movies available in the specified theatre.
 */
const std::vector<const Movie*> Controller::getMoviesFromTheatre(const std::string& theatreId)
{
    return m_theatreManagementService->getMoviesFromTheatre(theatreId);
}

/*
 * Function: login
 * Description: Passes the login process to the AuthenticationManagementService.
 * Parameters:
 *    email (string) - The email address of the user attempting to log in
 *    password (string) - The password associated with the given email
 * Returns:
 *    A pair containing:
 *      enum - LoginStatus indicating the result of the login attempt
 *      enum - UserType of the authenticated user if login is successful,
 *             otherwise USER_NOT_FOUND
 */
pair<Enums::LoginStatus, Enums::UserType> Controller::login(string email, string password)
{
    return m_authenticationManagementService->login(email, password);
}

/*
 * Function: Controller::searchTheatreByName
 * Description: Searches for theatres by their name. Delegates the request to the
 *              TheatreManagementService to retrieve theatres that match the given name.
 * Parameters:
 *    theatreName - The name of the theatre to search for.
 * Returns:
 *    A vector of Theatre pointers representing theatres that match the provided name.
 *    Returns an empty vector if no theatres are found.
 */
const std::vector<const Theatre*> Controller::searchTheatreByName(const std::string& theatreName)
{
    return m_theatreManagementService->searchByTheatreName(theatreName);
}

/*
 * Function: Controller::getAuthenticatedUser
 * Description: Retrieves the currently authenticated user through the
 *              TheatreManagementService. Provides access to the user object
 *              representing the active session.
 * Parameters: None
 * Returns:
 *    Pointer to the User object representing the authenticated user, or nullptr
 *    if no user is currently logged in.
 */
const User* Controller::getAuthenticatedUser() const
{
    return m_theatreManagementService->getAuthenticatedUser();
}

/*
 * Function: Controller::getAllTheatres
 * Description: Retrieves all theatres from the TheatreManagementService.
 *              Acts as a delegation layer to access the complete list of
 *              theatres stored in the system.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers representing all theatres available
 *    in the datastore. Returns an empty vector if no theatres exist.
 */
const std::vector<const Theatre*> Controller::getAllTheatres()
{
    return m_theatreManagementService->getAllTheatres();
}

/*
 * Function: Controller::getAllActiveMovies
 * Description: Retrieves all movies currently marked as ACTIVE in the system by delegating
 *              the request to the MovieManagementService.
 * Parameters:
 *    None
 * Returns:
 *    A vector of constant Movie pointers representing all active movies
 */
std::vector<const Movie*> Controller::getAllActiveMovies()
{
    return m_movieManagementService->getAllActiveMovies();
}

/*
 * Function: Controller::isTheatrePhoneNumberUnique
 * Description: Validates whether the provided theatre phone number is unique
 *              by checking against existing theatres in the system.
 * Parameters:
 *    phoneNumber (const std::string&) - Theatre contact phone number to validate
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the phone number is unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::isTheatrePhoneNumberUnique(const std::string& phoneNumber)
{
    return m_theatreManagementService->isTheatrePhoneNumberUnique(phoneNumber);
}

/*
 * Function: Controller::isTheatreEmailUnique
 * Description: Validates whether the provided theatre email address is unique
 *              by checking against existing theatres in the system.
 * Parameters:
 *    email (const std::string&) - Theatre email address to validate
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the email address is unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::isTheatreEmailUnique(const std::string& email)
{
    return m_theatreManagementService->isTheatreEmailUnique(email);
}

/*
 * Function: Controller:: addTheatre
 * Description: Adds a new theatre to the system with the provided details.
 * Parameters:
 *    name (const std::string&) - Name of the theatre
 *    city (const std::string&) - City where the theatre is located
 *    address (const std::string&) - Address of the theatre
 *    phoneNumber (const std::string&) - Contact phone number of the theatre
 *    email (const std::string&) - Contact email address of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the theatre is added successfully,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::addTheatre(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email)
{
    return m_theatreManagementService->addTheatre(name, city, address, phoneNumber, email);
}

/*
 * Function: Controller::isTheatreUnique
 * Description: Validates whether the provided theatre details are unique
 *              in the system by checking existing theatre records.
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
Enums::ProcessStatus Controller::isTheatreUnique(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email)
{
    return m_theatreManagementService->isTheatreUniqueInSystem(name, city, address, phoneNumber, email);
}

/*
 * Function: Controller::isMovieInTheatre
 * Description: Checks whether the specified movie is available in the given theatre.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the movie is present in the theatre,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::isMovieInTheatre(const std::string& movieId, const std::string& theatreId)
{
    return m_showManagementService->isMovieInTheatre(movieId, theatreId);
}

/*
 * Function: Controller::getScreensFromTheatre
 * Description: Retrieves all screens available in the specified theatre.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    std::vector<const Screen*> - List of screens present in the theatre
 */
const std::vector<const Screen*> Controller::getScreensFromTheatre(const std::string& theatreId)
{
    return m_showManagementService->getScreensFromTheatre(theatreId);
}

/*
 * Function: Controller::setTheatreNameById
 * Description: Updates the name of a theatre using the given theatre ID.
 *              Delegates the theatre name update operation to the
 *              TheatreManagementService.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    name      - New name to be assigned to the theatre.
 * Returns:
 *    ProcessStatus indicating whether the theatre name update
 *    operation was successful or failed.
 */
Enums::ProcessStatus Controller::setTheatreNameById(const std::string& theatreId, const std::string& name)
{
    return m_theatreManagementService->setTheatreNameById(theatreId, name);
}

/*
 * Function: Controller::setTheatreCityById
 * Description: Updates the city of a theatre using the given theatre ID.
 *              Delegates the theatre city update operation to the
 *              TheatreManagementService.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    city      - New city to be assigned to the theatre.
 * Returns:
 *    ProcessStatus indicating whether the theatre city update
 *    operation was successful or failed.
 */
Enums::ProcessStatus Controller::setTheatreCityById(const std::string& theatreId, const std::string& city)
{
    return m_theatreManagementService->setTheatreCityById(theatreId, city);
}

/*
 * Function: Controller::setTheatreAddressById
 * Description: Updates the address of a theatre using the given theatre ID.
 *              Delegates the theatre address update operation to the
 *              TheatreManagementService.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    address   - New address to be assigned to the theatre.
 * Returns:
 *    ProcessStatus indicating whether the theatre address update
 *    operation was successful or failed.
 */
Enums::ProcessStatus Controller::setTheatreAddressById(const std::string& theatreId, const std::string& address)
{
    return m_theatreManagementService->setTheatreAddressById(theatreId, address);
}

/*
 * Function: Controller::setTheatrePhoneNumberById
 * Description: Updates the phone number of a theatre using the given
 *              theatre ID. Delegates the theatre phone number update
 *              operation to the TheatreManagementService.
 * Parameters:
 *    theatreId   - Unique identifier of the theatre.
 *    phoneNumber - New phone number to be assigned to the theatre.
 * Returns:
 *    ProcessStatus indicating whether the theatre phone number update
 *    operation was successful or failed.
 */
Enums::ProcessStatus Controller::setTheatrePhoneNumberById(const std::string& theatreId, const std::string& phoneNumber)
{
    return m_theatreManagementService->setTheatrePhoneNumberById(theatreId, phoneNumber);
}

/*
 * Function: Controller::setTheatreEmailById
 * Description: Updates the email address of a theatre using the given
 *              theatre ID. Delegates the theatre email update operation
 *              to the TheatreManagementService.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    email     - New email address to be assigned to the theatre.
 * Returns:
 *    ProcessStatus indicating whether the theatre email update
 *    operation was successful or failed.
 */
Enums::ProcessStatus Controller::setTheatreEmailById(const std::string& theatreId, const std::string& email)
{
    return m_theatreManagementService->setTheatreEmailById(theatreId, email);
}

/*
 * Function: Controller::getPendingTheatres
 * Description: Retrieves all theatres that are currently in pending
 *              status from the TheatreManagementService.
 *              Acts as a delegation layer between the user interface
 *              and the service layer.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers containing all theatres with
 *    pending status. Returns an empty vector if no pending
 *    theatres are available.
 */
const std::vector<const Theatre*> Controller::getPendingTheatres()
{
    return m_theatreManagementService->getPendingTheatres();
}

/*
 * Function: Controller::setTheatreStatusById
 * Description: Updates the status of a theatre identified by the given
 *              theatre ID. Delegates the theatre status update
 *              operation to the TheatreManagementService.
 * Parameters:
 *    theatreId    - Unique identifier of the theatre.
 *    theatreStatus - New status to be assigned to the theatre.
 * Returns:
 *    ProcessStatus indicating whether the theatre status update
 *    operation was successful or failed.
 */
Enums::ProcessStatus Controller::setTheatreStatusById(const std::string& theatreId, Enums::TheatreStatus& theatreStatus)
{
    return m_theatreManagementService->setTheatreStatusById(theatreId, theatreStatus);
}

/*
 * Function: Controller::listAllTheatres
 * Description: Retrieves all theatres available in the system from
 *              the TheatreManagementService.
 *              Acts as a delegation layer between the user interface
 *              and the service layer.
 * Parameters: None
 * Returns:
 *    A vector of Theatre pointers containing all theatres
 *    stored in the datastore. Returns an empty vector if
 *    no theatres are available.
 */
std::vector<const Theatre*> Controller::listAllTheatres() const
{
    return m_theatreManagementService->listAllTheatres();
}

/*
 * Function: Controller::addMovieToTheatre
 * Description: Adds a movie to a specified theatre by passing the request
 *              to the TheatreManagementService. Validates the operation and
 *              returns the process status indicating success or failure.
 * Parameters:
 *    theatreId - A string representing the unique identifier of the theatre
 *                where the movie should be added.
 *    movieId   - A string representing the unique identifier of the movie
 *                to be added to the theatre.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was successfully added.
 *    Enums::ProcessStatus::FAILED if the movie already exists in the theatre
 *    or if the operation could not be completed.
 */
Enums::ProcessStatus Controller::addMovieToTheatre(const std::string& theatreId, const std::string& movieId)
{
    return m_theatreManagementService->addMovieToTheatre(theatreId, movieId);
}

/*
 * Function: Controller::isShowTimeConflicting
 * Description: Checks whether a show with the given movie and screen conflicts
 *              with any existing show's time. Delegates the validation to the
 *              ShowManagementService.
 * Parameters:
 *    movieId  - Unique identifier of the movie
 *    screenId - Unique identifier of the screen
 *    year     - Year of the proposed show
 *    month    - Month of the proposed show
 *    day      - Day of the proposed show
 *    hour     - Hour of the proposed show start time
 *    minute   - Minute of the proposed show start time
 * Returns:
 *    ProcessStatus::SUCCESS if no conflict exists,
 *    ProcessStatus::FAILED if a conflicting show is found
 */
Enums::ProcessStatus Controller::isShowTimeConflicting(const std::string& movieId, const std::string& screenId, int year, int month, int day, int hour, int minute)
{
    return m_showManagementService->isShowTimeConflicting(movieId, screenId, year, month, day, hour, minute);
}

/*
 * Function: createUser
 * Description: Delegates the Admin-driven user registration process to the
 *              UserManagementService. Attempts to register a new user with
 *              the provided details and role. Returns the outcome of the
 *              registration attempt.
 * Parameters:
 *    userName   - The name of the user to be registered
 *    email      - The email address of the user
 *    password   - The password for the user account
 *    phoneNumber- The phone number of the user
 *    userType   - The role of the user
 * Returns:
 *    enum - SUCCESS if registration is successful,
 *    enum - FAILED otherwise
 */
Enums::ProcessStatus Controller::createUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType)
{
    if (m_userManagementService->createUser(userName, email, password, phoneNumber, userType) == Enums::ProcessStatus::SUCCESS)
    {
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: getActiveUsers
 * Description: Retrieves a list of all active users from the UserManagementService.
 * Parameters:
 *    None
 * Returns:
 *    A vector of const User* pointers representing active users
 */
const std::vector<const User*> Controller::getActiveUsers() const
{
    return m_userManagementService->getActiveUsers();
}

/*
 * Function: getInactiveUsers
 * Description: Retrieves a list of all inactive users from the UserManagementService.
 * Parameters:
 *    None
 * Returns:
 *    A vector of const User* pointers representing inactive users
 */
const std::vector<const User*> Controller::getInactiveUsers()
{
    return m_userManagementService->getInactiveUsers();
}

/*
 * Function: setAuthenticatedUserUserName
 * Description: Updates the username of the currently authenticated user.
 * Parameters:
 *    username - The new username to be set
 * Returns:
 *    SUCCESS if the update succeeds,
 *    FAILED otherwise
 */
Enums::ProcessStatus Controller::setAuthenticatedUserUserName(const std::string& username)
{
    return m_userManagementService->setAuthenticatedUserUserName(username);
}

/*
 * Function: setAuthenticatedUserEmail
 * Description: Updates the email of the currently authenticated user after verifying uniqueness.
 * Parameters:
 *    email - The new email to be set
 * Returns:
 *    SUCCESS if the update succeeds,
 *    FAILED otherwise
 */
Enums::ProcessStatus Controller::setAuthenticatedUserEmail(const std::string& email)
{
    if (isEmailUnique(email) == Enums::ProcessStatus::SUCCESS)
    {
        return m_userManagementService->setAuthenticatedUserEmail(email);
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: setAuthenticatedUserPhoneNumber
 * Description: Updates the phone number of the currently authenticated user after verifying uniqueness.
 * Parameters:
 *    phoneNumber - The new phone number to be set
 * Returns:
 *    SUCCESS if the update succeeds,
 *    FAILED otherwise
 */
Enums::ProcessStatus Controller::setAuthenticatedUserPhoneNumber(const std::string& phoneNumber)
{
    if (isPhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS)
    {
        return m_userManagementService->setAuthenticatedUserPhoneNumber(phoneNumber);
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: deactivateUser
 * Description: Passes the deactivation of a user account to the UserManagementService.
 * Parameters:
 *    userId - The unique identifier of the user to be deactivated
 * Returns:
 *    enum - SUCCESS if activation succeeds,
 *    enum - FAILED otherwise
 */
Enums::ProcessStatus Controller::deactivateUser(const std::string& userId)
{
    return m_userManagementService->deactivateUser(userId);
}

/*
 * Function: reactivateUser
 * Description: Delegates the activation of a user account to the UserManagementService.
 * Parameters:
 *    userId - The unique identifier of the user to be activated
 * Returns:
 *    enum - SUCCESS if activation succeeds,
 *    enum - FAILED otherwise
 */
Enums::ProcessStatus Controller::reactivateUser(const std::string& userId)
{
    return m_userManagementService->reactivateUser(userId);
}

/*
 * Function: getAuthenticatedUser
 * Description: Retrieves the currently authenticated user from the UserManagementService.
 * Parameters:
 *    None
 * Returns:
 *    A pointer to the authenticated User object, or nullptr if no user is authenticated
 */
const User* const Controller::getAuthenticatedUser()
{
    return m_userManagementService->getAuthenticatedUser();
}

/*
 * Function: changePassword
 * Description: Delegates the password change request to the UserManagementService.
 * Parameters:
 *    currentPassword - The current password entered by the user
 *    newPassword     - The new password to be set
 * Returns:
 *    SUCCESS if the password change succeeds,
 *    FAILED otherwise
 */
Enums::ProcessStatus Controller::changePassword(const std::string& currentPassword, const std::string& newPassword)
{
    return m_userManagementService->changePassword(currentPassword, newPassword);
}

/*
 * Function: getUserStatus
 * Description: Retrieves the status (Active/Inactive) of a user by their ID.
 * Parameters:
 *    userId - The unique identifier of the user
 * Returns:
 *    ACTIVE if the user is active,
 *    INACTIVE if the user is inactive,
 *    or another appropriate status if not found
 */
Enums::UserStatus Controller::getUserStatus(const std::string& userId)
{
    return m_userManagementService->getUserStatus(userId);
}

/*
* Function Name : getUnreadNotifications
* Description   : Retrieves unread notifications for the user.
* Parameters    :
*                  batchSize             - Number of notifications to retrieve
*                  remainingUnreadCount  - Remaining unread notification count
* Return Type   : std::vector<std::string>
*/
std::vector<std::string> Controller::getUnreadNotifications(int batchSize, int& remainingUnreadCount)
{
    return m_notificationManagementService->getUnreadNotifications(batchSize, remainingUnreadCount);
}

/*
 * Function: Controller::setMovieDeactivate
 * Description: Deactivates a movie in the system by delegating the request to the MovieManagementService.
 * Parameters:
 *    movieId - Unique identifier of the movie to deactivate
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was successfully deactivated,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::setMovieDeactivate(const std::string& movieId)
{
    return m_movieManagementService->setMovieDeactive(movieId);
}

/*
 * Function: Controller::setMovieActivate
 * Description: Activates a movie in the system by delegating the request to the MovieManagementService.
 * Parameters:
 *    movieId - Unique identifier of the movie to activate
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie was successfully activated,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus Controller::setMovieActivate(const std::string& movieId)
{
    return m_movieManagementService->setMovieActive(movieId);
}

/*
 * Function: Controller::searchDeactivatedMovieByTitle
 * Description: Searches for movies in the system that match the given title and are marked as INACTIVE.
 *              Delegates the search request to the MovieManagementService.
 * Parameters:
 *    title - Title of the movie to search for
 * Returns:
 *    A vector of constant Movie pointers representing the matching deactivated movies
 */
const std::vector<const Movie*> Controller::searchDeactivatedMovieByTitle(const std::string& title)
{
    return m_movieManagementService->searchDeactivatedMovieByTitle(title);
}

/*
 * Function: Controller::getAllInactiveMovies
 * Description: Retrieves all movies currently marked as INACTIVE in the system by delegating
 *              the request to the MovieManagementService.
 * Parameters:
 *    None
 * Returns:
 *    A vector of constant Movie pointers representing all inactive movies
 */
std::vector<const Movie*> Controller::getAllInactiveMovies()
{
    return m_movieManagementService->getAllInactiveMovies();
}

/*
 * Function: updateSeatLayout
 * Description: Passes seat layout update request to SeatManagementService.
 * Parameters:
 *    screen  - Target screen object
 *    rows    - Number of rows
 *    columns - Number of columns
 * Returns:
 *    enum - ProcessStatus
 */
Enums::ProcessStatus Controller::updateSeatLayout(Screen* screen, int rows, int columns, double amount)
{
    return m_seatManagementService->updateSeatLayout(screen, rows, columns, amount);
}

/*
 * Function: getSeatLayout
 * Description: Retrieves the seating grid for a given screen from SeatManagementService.
 * Parameters:
 *    Screen* - Target screen
 * Returns:
 *    Const reference to 2D vector of Seat pointers
 */
const std::vector<std::vector<Seat*>>& Controller::getSeatLayout(const Screen* screen) const
{
    return m_seatManagementService->getSeatLayout(screen);
}

/*
 * Function: deactivateSeat
 * Description: Delegates seat deactivation request to SeatManagementService.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    enum - ProcessStatus
 */
Enums::ProcessStatus Controller::deactivateSeat(Screen* screen, const std::string& seatId)
{
    return m_seatManagementService->deactivateSeat(screen, seatId);
}

/*
 * Function: reactivateSeat
 * Description: Delegates seat reactivation request to SeatManagementService.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    ProcessStatus indicating success or failure
 */
Enums::ProcessStatus Controller::reactivateSeat(Screen* screen, const std::string& seatId)
{
    return m_seatManagementService->reactivateSeat(screen, seatId);
}

/*
* Function Name : viewShowSeatLayout
* Description   : Retrieves the seat layout for a given show by passing
*                 the call to SeatManagementService. Returns a 2D vector
*                 representing the seat arrangement.
* Parameters    :
*                  show - Pointer to the Show object whose seat layout is to be retrieved
* Return Type   : const std::vector<std::vector<std::string>>
*/
const std::vector<std::vector<std::string>> Controller::viewShowSeatLayout(const Show* show)
{
    return m_seatManagementService->getShowsSeatLayout(show);
}

/*
* Function Name : addScreen
* Description   : Adds a new screen to the theatre through the screen management service.
* Parameters    :
*                  theatre     - Theatre where the screen is added
*                  name        - Name of the screen
*                  seatRows    - Number of seat rows
*                  seatColumns - Number of seat columns
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus Controller::addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount)
{
    return m_ScreenManagementService->addScreen(theatreId, name, seatRows, seatColumns, seatAmount);
}

/*
* Function Name : updateScreenName
* Description   : Updates the name of a screen through the service.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
*                  name     - New screen name
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus Controller::updateScreenName(const std::string& theatreId, const std::string& screenId, const std::string& name)
{
    return m_ScreenManagementService->updateScreenName(theatreId, screenId, name);
}

/*
* Function Name : deactivateScreen
* Description   : Deactivates a screen through the service.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus Controller::deactivateScreen(const std::string& theatreId, const std::string& screenId)
{
    return m_ScreenManagementService->deactivateScreen(theatreId, screenId);
}

/*
* Function Name : reactivateScreen
* Description   : Reactivates an inactive screen through the service.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus Controller::reactivateScreen(const std::string& theatreId, const std::string& screenId)
{
    return m_ScreenManagementService->reactivateScreen(theatreId, screenId);
}

/*
* Function Name : viewTheatreScreens
* Description   : Retrieves all screens available in the theatre.
* Parameters    :
*                  theatre - Theatre whose screens are viewed
* Return Type   : const std::vector<Screen*>
*/
const std::vector<const Screen*> Controller::viewTheatreScreens(const std::string& theatreId)
{
    return m_ScreenManagementService->viewTheatreScreens(theatreId);
}

/*
* Function Name : getAuthenticatedUserType
* Description   : Retrieves the Authenticated User Type.
* Parameters    :
*                  None
* Return Type   : Enums::UserType
*/
Enums::UserType Controller::getAuthenticatedUserType()
{
    return m_ScreenManagementService->getAuthenticatedUserType();
}

/*
*Function: Controller::isShowTimeConflicting
* Description : Determines whether the specified show timing conflicts with
* any existing show on the same screen.
* Parameters :
    *movieId(const std::string&) - Unique identifier of the movie
    * screenId(const std::string&) - Unique identifier of the screen
    * year(int) - Year of the show
    * month(int) - Month of the show
    * day(int) - Day of the show
    * startTimeHour(int) - Hour component of show start time
    * startTimeMinute(int) - Minute component of show start time
    * Returns:
*Enums::ProcessStatus - FAILED if there is a conflict,
* SUCCESS otherwise
* /
Enums::ProcessStatus Controller::isShowTimeConflicting(const std::string & movieId, const std::string & screenId, int year, int month, int day, int startTimeHour, int startTimeMinute)
{
    return m_showManagementService->isShowTimeConflicting(movieId, screenId, year, month, day, startTimeHour, startTimeMinute);
}

/*
 * Function: Controller::isNewShowTimeConflicting
 * Description: Validates whether a new show time conflicts with existing shows on the same screen.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 *    newTime (const time_t&) - Proposed new start time
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if no conflict, FAILED otherwise
 */
Enums::ProcessStatus Controller::isNewShowTimeConflicting(const std::string& showId, const time_t& newTime)
{
    return m_showManagementService->isNewShowTimeConflicting(showId, newTime);
}

/*
 * Function: Controller::addShow
 * Description: Adds a new show for a given movie and screen after validating
 *              inputs and checking for conflicts.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 *    screenId (const std::string&) - Unique identifier of the screen
 *    year (int) - Year of the show
 *    month (int) - Month of the show
 *    day (int) - Day of the show
 *    startTimeHour (int) - Hour component of show start time
 *    startTimeMinute (int) - Minute component of show start time
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the show is added successfully,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::addShow(const std::string & movieId, const std::string & screenId, int year, int month, int day, int startTimeHour, int startTimeMinutes)
{
    return m_showManagementService->addShow(movieId, screenId, year, month, day, startTimeHour, startTimeMinutes);
}

/*
 * Function: Controller::updateShow
 * Description: Updates the start and end time of a show.
 * Parameters:
 *    time (const time_t&) - New start time
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if updated, FAILED otherwise
 */
Enums::ProcessStatus Controller::updateShow(const time_t& time, const std::string& showId)
{
    return m_showManagementService->updateShow(time, showId);
}

/*
 * Function: Controller::getActiveShows
 * Description: Retrieves all active shows (scheduled and not yet started) via ShowManagementService.
 * Returns:
 *    const std::vector<const Show*> - List of active shows
 */
const std::vector<const Show*> Controller::getActiveShows()
{
    return m_showManagementService->getActiveShows();
}

/*
 * Function: Controller::getActiveShowIds
 * Description: Retrieves all active show IDs owned by the authenticated theatre owner.
 * Returns:
 *    const std::vector<std::string> - List of show IDs
 */
const std::vector<std::string> Controller::getActiveShowIds()
{
    return m_showManagementService->getActiveShowIds();
}

/*
 * Function: Controller::getAllShows
 * Description: Retrieves all shows owned by the authenticated theatre owner.
 * Returns:
 *    const std::vector<const Show*> - List of shows
 */
const std::vector<const Show*> Controller::getAllShows()
{
    return m_showManagementService->getAllShows(); 
}

/*
 * Function: Controller::getAllShowIds
 * Description: Retrieves all show IDs owned by the authenticated theatre owner.
 * Returns:
 *    const std::vector<std::string> - List of show IDs
 */
const std::vector<std::string> Controller::getAllShowIds()
{
    return m_showManagementService->getAllShowIds();
}

/*
 * Function: Controller::getShowStatus
 * Description: Retrieves the current status of a show by its ID.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    Enums::ShowStatus - Current status of the show or NOT_FOUND if invalid
 */
Enums::ShowStatus Controller::getShowStatus(const std::string& showId)
{
    return m_showManagementService->getShowStatus(showId);
}

/*
 * Function: Controller::isShowChangable
 * Description: Checks whether the specified show can be cancelled or updated by verifying
 *              booking status through the ShowManagementService.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the show can be cancelled,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::isShowChangable(const std::string& showId)
{
    return m_showManagementService->isShowChangable(showId);
}

/*
 * Function: Controller::setShowStatusById
 * Description: Updates the status of a show identified by its ID.
 * Parameters:
 *    showId (const std::string&) - Unique identifier of the show
 *    status (Enums::ShowStatus) - New status to be assigned to the show
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the status is updated successfully,
 *                           FAILED otherwise
 */
Enums::ProcessStatus Controller::setShowStatusById(const std::string& showId, Enums::ShowStatus status)
{
    return m_showManagementService->setShowStatusById(showId, status);
}

/*
 * Function: Controller::getShowsForMovie
 * Description: Retrieves all scheduled future shows for a given movie ID.
 * Parameters:
 *    movieId (const std::string) - Unique identifier of the movie
 * Returns:
 *    const std::vector<const Show*> - List of shows for the movie
 */
const std::vector<const Show*> Controller::getShowsForMovie(const std::string movieId)
{
    return m_showManagementService->getShowsForMovie(movieId);
}

/*
*Function: Controller::initiatePayment
* Description : Initiates the payment process for a given booking by passing
* the request to the PaymentManagementService.Handles payment
* creation, validation, and ticket generation through the service layer.
* Parameters :
    *bookingId - Unique identifier of the booking for which payment is to be initiated.
    * paymentMethod - Enum value representing the chosen payment method(e.g., CARD, UPI).
    * amount - The payment amount to be processed.
    * Returns :
    *Enums::ProcessStatus::SUCCESS if the payment and ticket generation were successful.
    * Enums::ProcessStatus::FAILED if the booking does not exist or if the operation fails.
*/
Enums::ProcessStatus Controller::initiatePayment(const std::string & bookingId, Enums::PaymentMethod paymentMethod, double amount)
{
    return m_paymentManagementService->initiatePayment(bookingId, paymentMethod, amount);
}

/*
 * Function: Controller::viewPaymentStatus
 * Description: Retrieves the status and details of a payment by passing
 *              the request to the PaymentManagementService. Populates the
 *              provided reference parameters with booking ID, amount, payment
 *              method, payment status, and payment date if the payment exists.
 * Parameters:
 *    paymentId    - Unique identifier of the payment to be viewed.
 *    bookingId    - Reference string to store the associated booking ID.
 *    amount       - Reference double to store the payment amount.
 *    paymentMethod- Reference enum to store the payment method used.
 *    paymentStatus- Reference enum to store the current status of the payment.
 *    paymentDate  - Reference string to store the payment date.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the payment details were successfully retrieved.
 *    Enums::ProcessStatus::FAILED if the payment does not exist.
 */
Enums::ProcessStatus Controller::viewPaymentStatus(const std::string& paymentId, std::string& bookingId,
    double& amount, Enums::PaymentMethod& paymentMethod, Enums::PaymentStatus& paymentStatus, std::string& paymentDate)
{
    return m_paymentManagementService->viewPaymentStatus(paymentId, bookingId, amount, paymentMethod, paymentStatus, paymentDate);
}

/*
*Function: Controller::getAllBookings
* Description : Retrieves all bookings for the authenticated user by delegating to BookingManagementService.
* Parameters :
    *None
    * Returns :
    * const std::vector<const Booking*> -List of bookings
*/
const std::vector<const Booking*> Controller::getAllBookings()
{
    return m_bookingManagementService->getAllBookings();
}

/*
 * Function: Controller::getAllBookingIds
 * Description: Retrieves all booking IDs for the authenticated user.
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<std::string> - List of booking IDs
 */
const std::vector<std::string> Controller::getAllBookingIds()
{
    return m_bookingManagementService->getAllBookingIds();
}

/*
 * Function: Controller::getBookingById
 * Description: Retrieves a booking object by its unique ID.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    const Booking* - Pointer to the booking if found, nullptr otherwise
 */
const Booking* Controller::getBookingById(const std::string& bookingId)
{
    return m_bookingManagementService->getBookingById(bookingId);
}

/*
 * Function: Controller::getCancellableCustomerBookings
 * Description: Retrieves all cancellable bookings for the authenticated customer.
 * Parameters:
 *    None
 * Returns:
 *    const std::vector<const Booking*> - List of cancellable bookings
 */
const std::vector<const Booking*> Controller::getCancellableCustomerBookings()
{
    return m_bookingManagementService->getCancellableCustomerBookings();
}

/*
 * Function: Controller::cancelBooking
 * Description: Cancels a booking by its ID.
 * Parameters:
 *    bookingId (const std::string&) - Unique identifier of the booking
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if cancelled, FAILED otherwise
 */
Enums::ProcessStatus Controller::cancelBooking(const std::string& bookingId)
{
    return m_bookingManagementService->cancelBooking(bookingId);
}

/*
 * Function: Controller::logout
 * Description: Logs out the currently authenticated user by delegating the
 *              operation to the AuthenticationManagementService.
 * Parameters: None
 * Returns: None
 */
void Controller::logout()
{
    m_authenticationManagementService->logout();
}

/*
* Function Name : Controller::viewTicketDetails
* Description   : Retrieves all active tickets for the currently authenticated user
*                 by delegating to TicketManagementService.
* Parameters    : None
* Return Type   : const std::vector<Ticket*>
*/
const std::vector<const Ticket*> Controller::viewTicketDetails()
{
    return m_ticketManagementService->viewTicketDetails();
}

/*
* Function Name : Controller::viewAllTickets
* Description   : Returns all tickets stored in the datastore by delegating
*                 to TicketManagementService.
* Parameters    : None
* Return Type   : const std::map<std::string, Ticket*>&
*/
const std::vector<const Ticket*> Controller::viewAllTickets()
{
    return m_ticketManagementService->viewAllTickets();
}

/*
* Function Name : Controller::viewTicketHistory
* Description   : Retrieves the complete ticket history for the currently authenticated user
*                 by delegating to TicketManagementService.
* Parameters    : None
* Return Type   : const std::vector<Ticket*>
*/
const std::vector<const Ticket*> Controller::viewTicketHistory()
{
    return m_ticketManagementService->viewTicketHistory();
}

/*
* Function Name : Controller::getAuthenticatedUserType
* Description   : Retrieves the user type of the currently authenticated user
*                 by delegating the call to TicketManagementService.
* Parameters    : None
* Return Type   : Enums::UserType
*/
Enums::UserType Controller::getAuthenticatedUserType() const
{
    return m_ticketManagementService->getAuthenticatedUserType();
}

/*
* Function Name : Controller::viewTicketStatus
* Description   : Retrieves the status of a ticket based on the provided Ticket ID
*                 by delegating the call to TicketManagementService.
* Parameters    :
*                  ticketId - The unique identifier of the ticket whose status is to be retrieved
* Return Type   : Enums::TicketStatus
*/
Enums::TicketStatus Controller::viewTicketStatus(const std::string& ticketId)
{
    return m_ticketManagementService->viewTicketStatus(ticketId);
}

/*
 * Function: Controller::~Controller
 * Description: Destructor. Cleans up allocated memory by deleting all
 *              management service pointers.
 * Returns: None
 */
Controller::~Controller()
{
    delete m_authenticationManagementService;
    delete m_userManagementService;
    delete m_theatreManagementService;
    delete m_movieManagementService;
    delete m_showManagementService;
    delete m_bookingManagementService;
    delete m_paymentManagementService;
    delete m_ticketManagementService;
    delete m_notificationManagementService;
    delete m_logManagementService;
    delete m_ScreenManagementService;
    delete m_seatManagementService;
    delete m_refundManagementService;
}