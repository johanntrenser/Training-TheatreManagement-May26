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
#include <direct.h>
#include <errno.h>
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
	const std::map<std::string, Show*>& getShows();
	std::map<std::string, Show*>& getShowsForUpdation();
	const std::map<std::string, Screen*>& getScreens();
	const std::map<std::string, Theatre*>& getTheatres();
	const std::map<std::string, Booking*>& getBookings();
	void addTheatre(Theatre* theatre);
	Theatre* getTheatreById(const std::string& theatreId);
	Movie* getMovieById(const std::string& movieId);
	Screen* getScreenById(const std::string& screenId);
	void addShow(Show* show);
	const std::map<std::string, ShowSeatAvailability*>& getShowSeatAvailabilitys();
	void addShowSeatAvailability(ShowSeatAvailability* showSeatAvailability);
	const Show* getShowById(const std::string& showId);
	Show* getShowByIdForUpdation(const std::string& showId);
	std::map<std::string, Ticket*>& getTickets();
	void addTicket(Ticket* ticket);
	Ticket* getTicketById(const std::string& ticketId);
	void addPayment(Payment* payment);
	void addRefund(Refund* refund);
	const std::map < std::string, Payment*>& getPayments();
	const std::map<std::string, Refund*>& getRefunds();
	Booking* getBookingById(const std::string& bookingId);
	Booking* getBookingByIdForUpdation(const std::string& bookingId);
	Seat* getSeatById(const std::string seatId);
	void addBooking(Booking* booking);
	const Ticket* getTicketForBooking(const Booking* booking);
	const std::map<std::string, Seat*>& getSeats();
	User* getUserById(const std::string& userId);
	Show* getShowDetailsById(std::string& id);
	void addSeat(Seat* seat);
	void addScreen(Screen* screen);
	void addNotification(Notification* notification);
	Booking* getBookingDetailsById(const std::string& bookingId);
	int getUsersCount() const;
	int getLogsCount() const;
	int getTheatresCount() const;
	void clearData();
	void setAuthenticatedUserPassword(const std::string& password);
	int getScreenCount() const;
	int getSeatCount() const;
	int getPaymentCount() const;
	int getRefundCount() const;
	int getShowCount() const;
	int getShowSeatAvailabilityCount() const;
	Enums::ProcessStatus updateUserStatus(const std::string& userId, Enums::UserStatus status);
	Enums::ProcessStatus updateScreenName(const std::string& screenId, const std::string& screenName);
	Enums::ProcessStatus updateScreenStatus(const std::string& screenId, Enums::ScreenStatus status);
	Enums::ProcessStatus updateSeatStatus(const std::string& seatId, Enums::SeatStatus status);
	Enums::ProcessStatus updateShowStatus(const std::string& showId, Enums::ShowStatus status);
	Enums::ProcessStatus addMovieToTheatre(const std::string& theatreId, const std::string& movieId);
	Enums::ProcessStatus updateTheatreName(const std::string& theatreId, const std::string& name);
	Enums::ProcessStatus updateTheatreCity(const std::string& theatreId, const std::string& city);
	Enums::ProcessStatus updateTheatreAddress(const std::string& theatreId, const std::string& address);
	Enums::ProcessStatus updateTheatrePhoneNumber(const std::string& theatreId, const std::string& phoneNumber);
	Enums::ProcessStatus updateTheatreEmail(const std::string& theatreId, const std::string& email);
	Enums::ProcessStatus updateTheatreStatus(const std::string& theatreId, Enums::TheatreStatus status);
	Enums::ProcessStatus clearMoviesFromTheatre(const std::string& theatreId);
	Enums::ProcessStatus removeMovieFromTheatre(const std::string& theatreId, const std::string& movieId);
	Enums::ProcessStatus updateShowTime(const std::string& showId, const time_t& startTime, const time_t& endTime);
	Enums::ProcessStatus addScreenToTheatre(const std::string& theatreId, const std::string& screenId);
	Enums::ProcessStatus updateShowSeatAvailabilityStatus(const std::string& availabilityId, const std::string& seatId, Enums::BookingStatus status);
	const std::string getTheatreIdFromScreen(const std::string& screenId);
	bool isUserLoggedIn(const std::string& userId);
	bool addLoggedInUser(const std::string& userId);
	bool removeLoggedInUser(const std::string& userId);
	int getTicketCount() const;
	Enums::ProcessStatus updateTicketStatus(const std::string& ticketId, Enums::TicketStatus status);
	Payment* getPaymentById(const std::string& paymentId);
	Enums::ProcessStatus updatePaymentStatus(const std::string& paymentId, Enums::PaymentStatus status);
	Enums::ProcessStatus updateRefundStatus(const std::string& refundId, Enums::RefundStatus status);
	int getBookingCount() const;
	Enums::ProcessStatus updateBookingStatus(const std::string& bookingId, Enums::BookingStatus status);
	ShowSeatAvailability* getShowSeatAvailabilityById(const std::string& seatAvailabilityId);
	bool isCurrentUserStillActive();
	void refreshUsers();
	void refreshTheatres();
	void refreshScreens();
	void refreshMovies();
	void refreshShows();
	void refreshSeats();
	void refreshBookings();
	void refreshPayments();
	void refreshTickets();
	void refreshRefunds();
	void refreshNotifications();
	void refreshLogs();
	void refreshShowSeatAvailabilitys();
	User* findUserById(const std::string& userId);
	Movie* findMovieById(const std::string& movieId);
	Theatre* findTheatreById(const std::string& theatreId);
	Screen* findScreenById(const std::string& screenId);
	Seat* findSeatById(const std::string& seatId);
	Show* findShowById(const std::string& showId);
	ShowSeatAvailability* findShowSeatAvailabilityById(const std::string& availabilityId);
	Booking* findBookingById(const std::string& bookingId);
	Payment* findPaymentById(const std::string& paymentId);
	Refund* findRefundById(const std::string& refundId);
	Ticket* findTicketById(const std::string& ticketId);
	Notification* findNotificationById(const std::string& notificationId);
	Log* findLogById(const std::string& logId);
	void rebuildRelationships();
	void rebuildTheatreRelationships();
	void rebuildScreenRelationships();
	void rebuildSeatRelationships();
	void rebuildShowSeatAvailabilityRelationships();
	void rebuildShowRelationships();
	void rebuildBookingRelationships();
	void rebuildPaymentRelationships();
	void rebuildTicketRelationships();
	void rebuildRefundRelationships();
	void rebuildNotificationRelationships();
	void refreshUserById(const std::string& userId);
	void refreshMovieById(const std::string& movieId);
	void refreshTheatreById(const std::string& theatreId);
	void refreshScreenById(const std::string& screenId);
	void refreshSeatById(const std::string& seatId);
	void refreshShowById(const std::string& showId);
	void refreshShowSeatAvailabilityById(const std::string& availabilityId);
	void refreshBookingById(const std::string& bookingId);
	void refreshPaymentById(const std::string& paymentId);
	void refreshTicketById(const std::string& ticketId);
	void refreshRefundById(const std::string& refundId);
	void refreshNotificationById(const std::string& notificationId);
	void refreshLogById(const std::string& logId);
	void rebuildTheatreRelationship(const std::string& theatreId);
	void rebuildScreenRelationship(const std::string& screenId);
	void rebuildSeatRelationship(const std::string& seatId);
	void rebuildShowRelationship(const std::string& showId);
	void rebuildShowSeatAvailabilityRelationship(const std::string& availabilityId);
	void rebuildBookingRelationship(const std::string& bookingId);
	void rebuildPaymentRelationship(const std::string& paymentId);
	void rebuildTicketRelationship(const std::string& ticketId);
	void rebuildRefundRelationship(const std::string& refundId);
	void rebuildNotificationRelationship(const std::string& notificationId);
	void updateNotificationStatus(const std::string& notificationId, Enums::NotificationStatus status);
	void ensureFolder(const char* path);
	~DataStore();
};