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




