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

void Controller::saveData()
{
    m_userManagementService->saveUserData();
    m_theatreManagementService->saveTheatreData();
    //m_movieManagementService->saveMovieData();
    m_ticketManagementService->saveTicketData();
    m_ScreenManagementService->saveScreenData();
    m_paymentManagementService->savePaymentData();
    m_notificationManagementService->saveNotificationData();
    m_logManagementService->saveLogData();
    m_refundManagementService->saveRefundData();
    m_seatManagementService->saveSeatData();
    m_showManagementService->saveShowData();
}