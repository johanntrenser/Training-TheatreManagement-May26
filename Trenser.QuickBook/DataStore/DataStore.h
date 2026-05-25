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
	std::map<std::string, Booking*> m_booking;
	std::map<std::string, Log*> m_log;
	std::map<std::string, Movie*> m_movie;
	std::map<std::string, Notification*> m_notification;
	std::map<std::string, Payment*> m_payment;
	std::map<std::string, Refund*> m_refund;
	std::map<std::string, Screen*> m_screen;
	std::map<std::string, Seat*> m_seat;
	std::map<std::string, Show*> m_show;
	std::map<std::string, ShowSeatAvailability*> m_showSeatAvailability;
	std::map<std::string, Theatre*> m_theatre;
	std::map<std::string, Ticket*> m_ticket;
public:
	const std::map<std::string, User*>& getUsers() const;
	void addUser(User* user);
	DataStore(const DataStore&) = delete;
	DataStore& operator=(const DataStore&) = delete;
	static DataStore& getInstance();
	User* const getAuthenticatedUser() const;
	void setAuthenticatedUser(User* user);
	const Enums::UserType getAuthenticatedUserType() const;
	void setAuthenticatedUserEmail(const std::string& email);
	void setAuthenticatedUserPhoneNumber(const std::string& phoneNumber);
	void setAuthenticatedUserName(const std::string& userName);
	const User* getAuthenticatedUser() const;
	std::map<std::string, Notification*>& getNotifications();
};