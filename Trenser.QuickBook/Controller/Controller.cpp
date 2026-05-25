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
Enums::ProcessStatus Controller::registerUser(const std::string& userName, const std::string& email, const std::string& password, const std::string phoneNumber, Enums::UserType userType)
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
 * Function: Controller::getTheatreById
 * Description: Retrieves a theatre object by its unique identifier. Passes
 *              the request to the TheatreManagementService to fetch the
 *              corresponding theatre from the datastore.
 * Parameters:
 *    theatreId - A string representing the unique identifier of the theatre
 *                to be retrieved.
 * Returns:
 *    Pointer to the Theatre object if found, or nullptr if no theatre exists
 *    with the given ID.
 */
Theatre* Controller::getTheatreById(const std::string& theatreId)
{
    return m_theatreManagementService->getTheatreById(theatreId);
}

/*
 * Function: Controller::getMovieById
 * Description: Retrieves a movie object by its unique identifier. Passes the
 *              request to the TheatreManagementService to fetch the corresponding
 *              movie from the datastore.
 * Parameters:
 *    movieId - A string representing the unique identifier of the movie to be retrieved.
 * Returns:
 *    Pointer to the Movie object if found, or nullptr if no movie exists with the given ID.
 */
Movie* Controller::getMovieById(const std::string& movieId)
{
    return m_theatreManagementService->getMovieById(movieId);
}

/*
 * Function: Controller::getAllActiveMovies
 * Description: Retrieves all active movies from the MovieManagementService.
 *              Acts as a passing layer to access the list of movies that
 *              are currently marked as active in the system.
 * Parameters: None
 * Returns:
 *    A vector of Movie pointers representing all active movies available
 *    in the datastore. Returns an empty vector if no active movies exist.
 */
std::vector<const Movie*> Controller::getAllActiveMovies()
{
    return m_movieManagementService->getAllActiveMovies();
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




