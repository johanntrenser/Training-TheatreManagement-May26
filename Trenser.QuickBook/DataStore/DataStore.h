/*
 * File: DataStore.h
 * Description: Defines the DataStore singleton class responsible for managing
 *              in-memory storage of system entities such as users, bookings,
 *              logs, movies, notifications, payments, refunds, screens, seats,
 *              shows, show seat availability, theatres, and tickets.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <map>
#include "User.h"
#include "Booking.h"
#include "Log.h"
#include "Movie.h"
#include "Notification.h"
#include "Payment.h"
#include "Refund.h"
#include "Screen.h"
#include "Seat.h"
#include "Show.h"
#include "ShowSeatAvailability.h"
#include "Theatre.h"
#include "Ticket.h"
#include "MappedFileRegistry.h"
#include "SessionManager.h"
#include "NamedMutex.h"
#include "ScopedLock.h"

class DataStore
{
private:
	DataStore() : m_currentUser(nullptr), m_sessionMutex(config::MutexMappings::SESSION_MUTEX_NAME)
	{
		initialize();
	}
	User* m_currentUser;
	MappedFileRegistry m_registry;
	SessionManager m_sessionManager;
	NamedMutex m_sessionMutex;
	std::map<std::string, User*> m_users;
	std::map<std::string, Booking*> m_bookings;
	std::map<std::string, Log*> m_logs;
	std::map<std::string, Movie*> m_movies;
	std::map<std::string, Notification*> m_notifications;
	std::map<std::string, Payment*> m_payments;
	std::map<std::string, Refund*> m_refunds;
	std::map<std::string, Screen*> m_screens;
	std::map<std::string, Seat*> m_seats;
	std::map<std::string, Show*> m_shows;
	std::map<std::string, ShowSeatAvailability*> m_showSeatAvailabilitys;
	std::map<std::string, Theatre*> m_theatres;
	std::map<std::string, Ticket*> m_tickets;
public:
	bool initialize();
	const std::map<std::string, User*>& getUsers();
	void addUser(User* user);
	const std::map<std::string, Log*>& getLogs();
	void addLog(Log* log);
	DataStore(const DataStore&) = delete;
	DataStore& operator=(const DataStore&) = delete;
	static DataStore& getInstance();
	User* getAuthenticatedUser() const;
	void setAuthenticatedUser(User* user);
	const Enums::UserType getAuthenticatedUserType() const;
	void setAuthenticatedUserEmail(const std::string& email);
	void setAuthenticatedUserPhoneNumber(const std::string& phoneNumber);
	void setAuthenticatedUserName(const std::string& userName);
	std::map<std::string, Notification*>& getNotifications();
	const std::map<std::string, Movie*>& getMovies();
	void addMovieToSystem(Movie* movie);
	const std::map<std::string, Show*>& getShows() const;
	std::map<std::string, Show*>& getShowsForUpdation();
	const std::map<std::string, Screen*>& getScreens();
	const std::map<std::string, Theatre*>& getTheatres() const;
	const std::map<std::string, Booking*>& getBookings() const;
	void addTheatre(Theatre* theatre);
	Theatre* getTheatreById(const std::string& theatreId) const;
	Movie* getMovieById(const std::string& movieId) const;
	Screen* getScreenById(const std::string& screenId) const;
	void addShow(Show* show);
	const std::map<std::string, ShowSeatAvailability*>& getShowSeatAvailabilitys() const;
	void addShowSeatAvailability(ShowSeatAvailability* showSeatAvailability);
	const Show* getShowById(const std::string& showId);
	Show* getShowByIdForUpdation(const std::string& showId);
	std::map<std::string, Ticket*>& getTickets();
	void addTicket(Ticket* ticket);
	Ticket* getTicketById(const std::string& ticketId) const;
	void addPayment(Payment* payment);
	void addRefund(Refund* refund);
	const std::map < std::string, Payment*>& getPayments() const;
	const std::map<std::string, Refund*>& getRefunds() const;
	const Booking* getBookingById(const std::string& bookingId);
	Booking* getBookingByIdForUpdation(const std::string& bookingId);
	Seat* getSeatById(const std::string seatId);
	void addBooking(Booking* booking);
	const Ticket* getTicketForBooking(const Booking* booking);
	const std::map<std::string, Seat*>& getSeats() const;
	User* getUserById(const std::string& userId);
	Show* getShowDetailsById(std::string& id);
	void addSeat(Seat* seat);
	void addScreen(Screen* screen);
	void addNotification(Notification* notification);
	Booking* getBookingDetailsById(const std::string& bookingId);
	std::map<std::string, ShowSeatAvailability*> getShowSeatAvailabilityList();
	void addShowSeatAvailabilityList(ShowSeatAvailability* showSeatAvailability);
	int getUsersCount() const;
	int getLogsCount() const;
	void clearData();
	void setAuthenticatedUserPassword(const std::string& password);
	int getScreenCount() const;
	int getSeatCount() const;
	Enums::ProcessStatus updateUserStatus(const std::string& userId, Enums::UserStatus status);
	Enums::ProcessStatus updateScreenName(const std::string& screenId, const std::string& screenName);
	Enums::ProcessStatus updateScreenStatus(const std::string& screenId, Enums::ScreenStatus status);
	Enums::ProcessStatus updateSeatStatus(const std::string& seatId, Enums::SeatStatus status);
	bool isUserLoggedIn(const std::string& userId);
	bool addLoggedInUser(const std::string& userId);
	bool removeLoggedInUser(const std::string& userId);
	int getTicketCount() const;
	Enums::ProcessStatus updateTicketStatus(const std::string& ticketId, Enums::TicketStatus status);
	Payment* getPaymentById(const std::string& paymentId);
	~DataStore();
};