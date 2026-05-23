/*
 * File: Controller.h
 * Description: Implements the Controller class responsible for managing
 *              various services including authentication, user, theatre,
 *              movie, show, booking, payment, ticket, notification, log,
 *              screen, seat, and refund management. Provides constructors
 *              for initialization and a destructor for cleanup.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <utility>
#include "AuthenticationManagementService.h"
#include "UserManagementService.h"
#include "TheatreManagementService.h"
#include "MovieManagementService.h"
#include "ShowManagementService.h"
#include "BookingManagementService.h"
#include "PaymentManagementService.h"
#include "TicketManagementService.h"
#include "NotificationManagementService.h"
#include "LogManagementService.h"
#include "ScreenManagementService.h"
#include "SeatManagementService.h"
#include "RefundManagementService.h"

class Controller
{
private:
	AuthenticationManagementService* m_authenticationManagementService;
	UserManagementService* m_userManagementService;
	TheatreManagementService* m_theatreManagementService;
	MovieManagementService* m_movieManagementService;
	ShowManagementService* m_showManagementService;
	BookingManagementService* m_bookingManagementService;
	PaymentManagementService* m_paymentManagementService;
	TicketManagementService* m_ticketManagementService;
	NotificationManagementService* m_notificationManagementService;
	LogManagementService* m_logManagementService;
	ScreenManagementService* m_ScreenManagementService;
	SeatManagementService* m_seatManagementService;
	RefundManagementService* m_refundManagementService;
public:
	Controller();
	Controller(AuthenticationManagementService* authService,
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
		RefundManagementService* refundService);
	std::pair<Enums::LoginStatus, Enums::UserType> login(std::string email, std::string password);
	Enums::ProcessStatus registerUser(const std::string& userName, const std::string& email, const std::string& password, const std::string phoneNumber, Enums::UserType userType);
	Enums::ProcessStatus isEmailUnique(const std::string& email);
	Enums::ProcessStatus isPhoneNumberUnique(const std::string& phoneNumber);
	const std::vector<const Theatre*> getCurrentOwnerTheatres();
	const std::vector<const Theatre*> searchTheatreByName(const std::string& theatreName);
	const User* getAuthenticatedUser() const;
	Enums::ProcessStatus isTheatrePhoneNumberUnique(const std::string& phoneNumber);
	Enums::ProcessStatus isTheatreEmailUnique(const std::string& email);
	Enums::ProcessStatus addTheatre(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	Enums::ProcessStatus isTheatreUnique(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	const std::vector<const Theatre*> getAllTheatres();
	void logout();
	~Controller();
};

