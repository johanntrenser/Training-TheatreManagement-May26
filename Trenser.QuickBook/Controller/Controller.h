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
	Enums::ProcessStatus deactivateMovie(const std::string& movieId);
	Enums::ProcessStatus reactivateMovie(const std::string& movieId);
	const std::vector<const Movie*> searchDeactivatedMovieByTitle(const std::string& title);
	const std::vector<const Theatre*> getCurrentOwnerTheatres();
	const std::vector<const Theatre*> getCurrentOwnerInavtiavteTheatres();
	const std::vector<std::string> getCurrentOwnerTheatreIds();
	const std::vector<const Movie*> getMoviesFromTheatre(const std::string& theatreId);
	const std::vector<const Theatre*> searchTheatreByName(const std::string& theatreName);
	const User* getAuthenticatedUser() const;
	const std::vector<const Theatre*> getAllTheatres();
	Enums::ProcessStatus isTheatrePhoneNumberUnique(const std::string& phoneNumber);
	Enums::ProcessStatus isTheatreEmailUnique(const std::string& email);
	Enums::ProcessStatus addTheatre(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	Enums::ProcessStatus isTheatreUnique(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	Enums::ProcessStatus isMovieInTheatre(const std::string& movieId, const std::string& theatreId);
	const std::vector<const Screen*> getScreensFromTheatre(const std::string& theatreId);
	Enums::ProcessStatus setTheatreNameById(const std::string& theatreId, const std::string& name);
	Enums::ProcessStatus setTheatreCityById(const std::string& theatreId, const std::string& city);
	Enums::ProcessStatus setTheatreAddressById(const std::string& theatreId, const std::string& address);
	Enums::ProcessStatus setTheatrePhoneNumberById(const std::string& theatreId, const std::string& phoneNumber);
	Enums::ProcessStatus setTheatreEmailById(const std::string& theatreId, const std::string& email);
	const std::vector<const Theatre*> getPendingTheatres();
	Enums::ProcessStatus setTheatreStatusById(const std::string& theatreId, Enums::TheatreStatus& theatreStatus);
	std::vector<const Theatre*> listAllTheatres() const;
	Enums::ProcessStatus addMovieToTheatre(const std::string& theatreId, const std::string& movieId);
	Enums::ProcessStatus isShowTimeConflicting(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute);
	Enums::ProcessStatus isNewShowTimeConflicting(const std::string& showId, const time_t& newTime);
	Enums::ProcessStatus addShow(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinutes);
	Enums::ProcessStatus updateShow(const time_t& time, const std::string& showId);
	const std::vector<const Show*> getActiveShows();
	const std::vector<std::string> getActiveShowIds();
	const std::vector<const Show*> getAllShows();
	const std::vector<std::string> getAllShowIds();
	Enums::ShowStatus getShowStatus(const std::string& showId);
	Enums::ProcessStatus isShowChangable(const std::string& showId);
	Enums::ProcessStatus setShowStatusById(const std::string& showId, Enums::ShowStatus status);
	const std::vector<const Show*> getShowsForMovie(const std::string movieId);
	Enums::ProcessStatus initiatePayment(const std::string& bookingId, Enums::PaymentMethod paymentMethod, double amount);
	const std::vector<const Booking*> getAllBookings();
	const std::vector<std::string> getAllBookingIds();
	const Booking* getBookingById(const std::string& bookingId);
	const std::vector<const Booking*> getCancellableCustomerBookings();
	Enums::ProcessStatus cancelBooking(const std::string& bookingId);
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
	std::vector<std::string> getUnreadNotifications(int batchSize, int& remainingUnreadCount);
	std::vector<const Movie*> getAllInactiveMovies();
	Enums::ProcessStatus updateSeatLayout(const std::string& selectedScreenId, int rows, int columns, double amount);
	const std::vector<std::vector<Seat*>>& getSeatLayout(const std::string& selectedScreenId) const;
	Enums::ProcessStatus deactivateSeat(const std::string& selectedScreenId, const std::string& seatId);
	Enums::ProcessStatus reactivateSeat(const std::string& selectedScreenId, const std::string& seatId);
	const std::vector<std::vector<std::string>> getShowSeatLayout(const Show* show);
	Enums::ProcessStatus addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount);
	Enums::ProcessStatus updateScreenName(const std::string& theatreId, const std::string& screenId, const std::string& name);
	Enums::ProcessStatus deactivateScreen(const std::string& theatreId, const std::string& screenId);
	Enums::ProcessStatus reactivateScreen(const std::string& theatreId, const std::string& screenId);
	const std::vector<const Screen*> viewTheatreScreens(const std::string& theatreId);
	Enums::UserType getAuthenticatedUserType();
	const std::vector<const Ticket*> viewTicketDetails();
	const std::vector<const Ticket*> viewAllTickets();
	const std::vector<const Ticket*> viewTicketHistory();
	Enums::UserType getAuthenticatedUserType() const;
	const Show* getShowById(const std::string& showId);
	const Booking* bookSelectedSeats(const std::string& showId, const std::vector<std::string>& selectedSeatIds);
	void logout();
	void saveData();
	void loadDataFromFile();
	Enums::ProcessStatus removeMovieFromTheatre(const std::string& theatreId, const std::string& movieId);
	const std::vector<Payment*> getAllPayments();
	const std::vector<Refund*> getRefunds();
	~Controller();
};