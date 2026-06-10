/*
 * File: SharedStructs.h
 * Description: Flat fixed-size structs for all entities.
 *              No pointers. No std::string. No std::map.
 *              These go directly into shared memory.
 */
#pragma once
#include <ctime>

 // ── User ────────────────────────────────────────────────────────
struct SharedUser
{
    char  userId[20];
    char  userName[100];
    char  email[100];
    char  password[100];
    char  phoneNumber[20];
    int   userType;         // Enums::UserType as int
    int   status;           // Enums::UserStatus as int
    bool  isOccupied;       // is this slot in use
};

// ── Movie ────────────────────────────────────────────────────────
struct SharedMovie
{
    char  movieId[20];
    char  title[100];
    char  language[50];
    char  genre[50];
    int   duration;         // in minutes
    int   status;           // Enums::MovieStatus as int
    bool  isOccupied;
};

// ── Theatre ──────────────────────────────────────────────────────
struct SharedTheatre
{
    char  theatreId[20];
    char  name[100];
    char  city[50];
    char  address[200];
    char  phoneNumber[20];
    char  email[100];
    char  ownerId[20];      // userId of theatre owner
    int   status;           // Enums::TheatreStatus as int
    bool  isOccupied;
};

// ── Screen ───────────────────────────────────────────────────────
struct SharedScreen
{
    char  screenId[20];
    char  theatreId[20];    // which theatre this belongs to
    char  name[50];
    int   rows;
    int   columns;
    double seatAmount;
    int   status;           // Enums::ScreenStatus as int
    bool  isOccupied;
};

// ── Seat ─────────────────────────────────────────────────────────
struct SharedSeat
{
    char  seatId[20];
    char  screenId[20];     // which screen this belongs to
    int   row;
    int   col;
    int   status;           // Enums::SeatStatus as int
    bool  isOccupied;
};

// ── Show ─────────────────────────────────────────────────────────
struct SharedShow
{
    char   showId[20];
    char   movieId[20];
    char   screenId[20];
    time_t startTime;
    time_t endTime;
    int    status;          // Enums::ShowStatus as int
    bool   isOccupied;
};

// ── ShowSeatEntry — one seat slot in a show ───────────────────────
struct SharedShowSeatEntry
{
    char  showSeatAvailId[20];
    char  showId[20];
    char  seatId[20];
    int   bookingStatus;    // Enums::BookingStatus as int
    bool  isOccupied;
};

// ── Booking ──────────────────────────────────────────────────────
struct SharedBooking
{
    char   bookingId[20];
    char   showId[20];
    char   customerId[20];
    char   seatIds[500];    // comma separated: "A1,A2,A3"
    int    status;          // Enums::BookingStatus as int
    double amount;
    bool   isOccupied;
};

// ── Payment ──────────────────────────────────────────────────────
struct SharedPayment
{
    char   paymentId[20];
    char   bookingId[20];
    double amount;
    int    paymentMethod;   // Enums::PaymentMethod as int
    int    status;          // Enums::PaymentStatus as int
    time_t timestamp;
    bool   isOccupied;
};

// ── Ticket ───────────────────────────────────────────────────────
struct SharedTicket
{
    char  ticketId[20];
    char  paymentId[20];
    char  customerId[20];
    int   status;           // Enums::TicketStatus as int
    bool  isOccupied;
};

// ── Refund ───────────────────────────────────────────────────────
struct SharedRefund
{
    char   refundId[20];
    char   ticketId[20];
    double refundAmount;
    time_t timestamp;
    int    status;          // Enums::RefundStatus as int
    bool   isOccupied;
};

// ── Notification ─────────────────────────────────────────────────
struct SharedNotification
{
    char  notificationId[20];
    char  receiverId[20];
    char  message[500];
    time_t timestamp;
    int   status;           // Enums::NotificationStatus as int
    bool  isOccupied;
};

// ── Log ──────────────────────────────────────────────────────────
struct SharedLog
{
    char  logId[20];
    char  message[500];
    time_t timestamp;
    int   logType;          // Enums::LogType as int
    bool  isOccupied;
};