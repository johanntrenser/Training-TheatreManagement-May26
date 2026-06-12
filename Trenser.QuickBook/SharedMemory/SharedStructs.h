#pragma once
#include "ApplicationConfig.h"

struct FileHeader
{
	int recordCount;
	int maxRecords;
	int recordSize;
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
	int  status;
};

struct SharedShow
{
	char showId[config::Limit::ID_MAX_LENGTH];
	char movieId[config::Limit::ID_MAX_LENGTH];
	char screenId[config::Limit::ID_MAX_LENGTH];
	char seatAvailabilityId[config::Limit::ID_MAX_LENGTH];
	char startTime[config::Limit::TIME_MAX_COUNT];
	char endTime[config::Limit::TIME_MAX_COUNT];
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
	int    status;
	char   time[config::Limit::TIME_MAX_COUNT];
};

struct SharedRefund
{
	char   refundId[config::Limit::ID_MAX_LENGTH];
	char   paymentId[config::Limit::ID_MAX_LENGTH];
	char   customerId[config::Limit::ID_MAX_LENGTH];
	double amount;
	int    status;
	char   time[config::Limit::TIME_MAX_COUNT];
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
	char message[config::Limit::MESSAGE_MAX_COUNT];
	int  status;
	char time[config::Limit::TIME_MAX_COUNT];
};

struct SharedShowSeatAvailability
{
	char availabilityId[config::Limit::ID_MAX_LENGTH];
	char showId[config::Limit::ID_MAX_LENGTH];
	char seatIds[config::Limit::SEAT_MAX_COUNT][config::Limit::ID_MAX_LENGTH];
	int  seatStatuses[config::Limit::SEAT_MAX_COUNT];
	int  seatCount;
};
