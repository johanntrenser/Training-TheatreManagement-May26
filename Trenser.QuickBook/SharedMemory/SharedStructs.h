/*
 * File: SharedStructs.h
 * Description: Declares the shared data structures used by the QuickBook
 *              application for memory-mapped file storage and inter-process
 *              communication. Defines the file metadata header and shared
 *              representations of users, movies, theatres, screens, seats,
 *              shows, bookings, payments, refunds, tickets, notifications,
 *              show seat availability, and logs using fixed-size arrays and
 *              primitive data types suitable for shared memory.
 * Author: Trenser
 * Created: 11 June 2026
 */
#pragma once
#define NOMINMAX
#include <Windows.h>
#include "ApplicationConfig.h"

struct FileHeader
{
	int recordCount;
	int maximumRecords;
	int recordSize;
	int version;
};

struct SharedUser
{
	char userId[config::Limit::ID_MAX_LENGTH];
	char username[config::Limit::USERNAME_MAX_LENGTH];
	char email[config::Limit::EMAIL_MAX_LENGTH];
	char password[config::Limit::PASSWORD_MAX_LENGTH];
	char phoneNumber[config::Limit::PHONENUMBER_MAX_LENGTH];
	int userType;
	int status;
};

struct SharedMovie
{
	char movieId[config::Limit::ID_MAX_LENGTH];
	char title[config::Limit::NAME_MAX_LENGTH];
	char language[config::Limit::LANGUAGE_MAX_LENGTH];
	char genre[config::Limit::GENRE_MAX_LENGTH];
	int  duration;
	int  status;
};

struct SharedTheatre
{
	char theatreId[config::Limit::ID_MAX_LENGTH];
	char name[config::Limit::NAME_MAX_LENGTH];
	char city[config::Limit::CITY_MAX_LENGTH];
	char address[config::Limit::ADDRESS_MAX_LENGTH];
	char phoneNumber[config::Limit::PHONENUMBER_MAX_LENGTH];
	char email[config::Limit::EMAIL_MAX_LENGTH];
	char ownerId[config::Limit::ID_MAX_LENGTH];
	int  status;
	char screenIds[config::Limit::SCREEN_MAX_COUNT][config::Limit::ID_MAX_LENGTH];
	int  screenCount;
	char movieIds[config::Limit::MOVIE_MAX_COUNT][config::Limit::ID_MAX_LENGTH];
	int  movieCount;
};

struct SharedScreen
{
	char screenId[config::Limit::ID_MAX_LENGTH];
	char theatreId[config::Limit::ID_MAX_LENGTH];
	char name[config::Limit::NAME_MAX_LENGTH];
	int  totalRows;
	int  totalColumns;
	int  status;
	char seatIds[config::Limit::SEAT_MAX_COUNT][config::Limit::ID_MAX_LENGTH];
	int  seatCount;
};

struct SharedSeat
{
	char seatId[config::Limit::ID_MAX_LENGTH];
	char screenId[config::Limit::ID_MAX_LENGTH];
	int  seatRow;
	int  seatColumn;
	double amount;
	int  status;
};

struct SharedShow
{
	char showId[config::Limit::ID_MAX_LENGTH];
	char movieId[config::Limit::ID_MAX_LENGTH];
	char screenId[config::Limit::ID_MAX_LENGTH];
	char seatAvailabilityId[config::Limit::ID_MAX_LENGTH];
	char startTime[config::Limit::TIME_MAX_LENGTH];
	char endTime[config::Limit::TIME_MAX_LENGTH];
	int  status;
};

struct SharedBooking
{
	char   bookingId[config::Limit::ID_MAX_LENGTH];
	char   customerId[config::Limit::ID_MAX_LENGTH];
	char   showId[config::Limit::ID_MAX_LENGTH];
	char   seatIds[config::Limit::SEAT_MAX_COUNT][config::Limit::ID_MAX_LENGTH];
	int    seatCount;
	int    status;
	double amount;
};

struct SharedPayment
{
	char   paymentId[config::Limit::ID_MAX_LENGTH];
	char   bookingId[config::Limit::ID_MAX_LENGTH];
	char   customerId[config::Limit::ID_MAX_LENGTH];
	double amount;
	int paymentMethod;
	int    status;
	char   time[config::Limit::TIME_MAX_LENGTH];
};

struct SharedRefund
{
	char   refundId[config::Limit::ID_MAX_LENGTH];
	char   paymentId[config::Limit::ID_MAX_LENGTH];
	char   customerId[config::Limit::ID_MAX_LENGTH];
	double amount;
	int    status;
	char   time[config::Limit::TIME_MAX_LENGTH];
};

struct SharedTicket
{
	char ticketId[config::Limit::ID_MAX_LENGTH];
	char paymentId[config::Limit::ID_MAX_LENGTH];
	char customerId[config::Limit::ID_MAX_LENGTH];
	int status;
};

struct SharedNotification
{
	char notificationId[config::Limit::ID_MAX_LENGTH];
	char userId[config::Limit::ID_MAX_LENGTH];
	char message[config::Limit::MESSAGE_MAX_LENGTH];
	int  status;
	char time[config::Limit::TIME_MAX_LENGTH];
};

struct SharedShowSeatAvailability
{
	char availabilityId[config::Limit::ID_MAX_LENGTH];
	char showId[config::Limit::ID_MAX_LENGTH];
	char seatIds[config::Limit::SEAT_MAX_COUNT][config::Limit::ID_MAX_LENGTH];
	int  seatStatuses[config::Limit::SEAT_MAX_COUNT];
	int  seatCount;
};

struct SharedLog
{
	char logId[config::Limit::ID_MAX_LENGTH];
	char description[config::Limit::MESSAGE_MAX_LENGTH];
	int type;
	char time[config::Limit::TIME_MAX_LENGTH];
};

struct SessionEntry
{
	char userId[config::Limit::ID_MAX_LENGTH];
	DWORD processId;
};

struct SharedSession
{
	int sessionCount;
	SessionEntry sessions[config::Limit::SESSION_MAX_COUNT];
};

struct NotifyMessage
{
	char targetType[20];  
	char targetId[20];    
	char message[500];
};