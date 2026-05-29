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
#include <User.h>
#include <Booking.h>
#include <Log.h>
#include <Movie.h>
#include <Notification.h>
#include <Payment.h>
#include <Refund.h>
#include <Screen.h>
#include <Seat.h>
#include <Show.h>
#include <ShowSeatAvailability.h>
#include <Theatre.h>
#include <Ticket.h>
class DataStore
{
private:
	DataStore() : m_currentUser(nullptr) {}
	User* m_currentUser;
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
	const std::map<std::string, User*>& getUsers() const;
	void addUser(User* user);
	DataStore(const DataStore&) = delete;
	DataStore& operator=(const DataStore&) = delete;
	static DataStore& getInstance();
	void setAuthenticatedUser(User* user);
	const std::map<std::string, Theatre*>& getTheatres() const;
	const std::map<std::string, Movie*>& getMovies() const;
	void addMovieToSystem(Movie* movie);
	std::map<std::string, Ticket*>& getTickets();
	const std::map<std::string, Screen*>& getScreens() const;
	const std::map < std::string, Payment*>& getPayments() const;
	std::map<std::string, Notification*>& getNotifications();
	const std::map<std::string, Log*>& getLogs() const;
	const std::map<std::string, Refund*>& getRefunds() const;
	const std::map<std::string, Seat*>& getSeats() const;
	const std::map<std::string, Show*>& getShows() const;
	const std::map<std::string, Booking*>& getBookings() const;
	void addTicket(Ticket* ticket);
	void addTheatre(Theatre* theatre);
	Screen* getScreenById(const std::string& screenId) const;
	Movie* getMovieById(std::string& movieId);
	void addShow(Show* show);
	void addSeat(Seat* seat);
	Ticket* getTicketById(const std::string& ticketId) const;
	void addRefund(Refund* refund);
	Booking* getBookingById(const std::string& bookingId);
	void addPayment(Payment* payment);
	User* getUserById(std::string& id);
	void addNotification(Notification* notification);
	void addLog(Log* log);
	Theatre* getTheatreById(const std::string& theatreId);
	void addScreen(Screen* screen);
	Seat* getSeatById(std::string& id);
	Show* getShowById(std::string& id);
	void addBooking(Booking* book);
};