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
	Enums::ProcessStatus registerUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType);
	Enums::ProcessStatus isEmailUnique(const std::string& email);
	Enums::ProcessStatus isPhoneNumberUnique(const std::string& phoneNumber);
	const std::vector<const Log*> getLogsByType(const Enums::LogType logType);
	Enums::ProcessStatus addMovie(const std::string& title, const std::string& language, const std::string& genre, const int duration);
	Enums::ProcessStatus isMovieUnique(const std::string& title, const std::string& language, const std::string& genre, const int duration);
	const std::vector<const Movie*> searchMovieByTitle(const std::string& title);
	Enums::ProcessStatus setMovieTitleByID(const std::string& movieId, const std::string& title);
	Enums::ProcessStatus setMovieLanguageByID(const std::string& movieId, const std::string& language);
	Enums::ProcessStatus setMovieGenreByID(const std::string& movieId, const std::string& genre);
	Enums::ProcessStatus setMovieDurationByID(const std::string& movieId, const int& duration);
	std::vector<const Movie*> getAllActiveMovies();
	Enums::ProcessStatus setMovieDeactivate(const std::string& movieId);
	Enums::ProcessStatus setMovieActivate(const std::string& movieId);
	const std::vector<const Movie*> searchDeactivatedMovieByTitle(const std::string& title);
	void logout();
	Enums::ProcessStatus createUser(const std::string& userName, const std::string& email, const std::string& password, const std::string& phoneNumber, Enums::UserType userType);
	const std::vector<const User*> getActiveUsers() const;
	Enums::ProcessStatus setAuthenticatedUserEmail(const std::string& email);
	Enums::ProcessStatus setAuthenticatedUserPhoneNumber(const std::string& phoneNumber);
	Enums::ProcessStatus setAuthenticatedUserUserName(const std::string& userName);
	Enums::ProcessStatus deactivateUser(const std::string& userId);
	Enums::ProcessStatus reactivateUser(const std::string& userId);
	const User* const getAuthenticatedUser();
	const std::vector<const User*> getInactiveUsers();
	Enums::ProcessStatus changePassword(const std::string& currentPassword, const std::string& newPassword);
	Enums::UserStatus getUserStatus(const std::string& userId);
	std::vector<std::string> getUnreadNotifications(int batchSize, int& remainingUnreadCount);
	std::vector<const Movie*> getAllInactiveMovies();
	Enums::ProcessStatus updateSeatLayout(Screen* screen, int rows, int columns, double amount);
	const std::vector<std::vector<Seat*>>& getSeatLayout(const Screen* screen) const;
	Enums::ProcessStatus deactivateSeat(Screen* screen, const std::string& seatId);
	Enums::ProcessStatus reactivateSeat(Screen* screen, const std::string& seatId);
	const std::vector<std::vector<std::string>> viewShowSeatLayout(const Show* show);
	Enums::ProcessStatus addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount);
	~Controller();
};