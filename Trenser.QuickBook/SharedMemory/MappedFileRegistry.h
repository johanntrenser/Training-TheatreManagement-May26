/*
* File: MappedFileRegistry.h
* Description: Declares the MappedFileRegistry class, which manages the
*              creation, initialization, access, and cleanup of all
*              memory-mapped files used by the QuickBook application.
*              Provides centralized access to mapped files containing
*              shared representations of users, movies, theatres, screens,
*              seats, shows, bookings, payments, refunds, tickets,
*              notifications, show seat availability, and logs.
* Author: Trenser
* Created: 14 June 2026
*/
#pragma once
#include "MappedFile.h"

class MappedFileRegistry
{
private:
    MappedFile<SharedUser>* m_users;
    MappedFile<SharedMovie>* m_movies;
    MappedFile<SharedTheatre>* m_theatres;
    MappedFile<SharedScreen>* m_screens;
    MappedFile<SharedSeat>* m_seats;
    MappedFile<SharedShow>* m_shows;
    MappedFile<SharedBooking>* m_bookings;
    MappedFile<SharedPayment>* m_payments;
    MappedFile<SharedRefund>* m_refunds;
    MappedFile<SharedTicket>* m_tickets;
    MappedFile<SharedNotification>* m_notifications;
    MappedFile<SharedShowSeatAvailability>* m_availability;
    MappedFile<SharedLog>* m_logs;
public:
    MappedFileRegistry() :
        m_users(nullptr),
        m_movies(nullptr),
        m_theatres(nullptr),
        m_screens(nullptr),
        m_seats(nullptr),
        m_shows(nullptr),
        m_bookings(nullptr),
        m_payments(nullptr),
        m_refunds(nullptr),
        m_tickets(nullptr),
        m_notifications(nullptr),
        m_availability(nullptr),
        m_logs(nullptr)
    {}
    MappedFileRegistry(const MappedFileRegistry&) = delete;
    MappedFileRegistry& operator=(const MappedFileRegistry&) = delete;
    bool openAll();
    MappedFile<SharedUser>* getUsers();
    MappedFile<SharedMovie>* getMovies();
    MappedFile<SharedTheatre>* getTheatres();
    MappedFile<SharedScreen>* getScreens();
    MappedFile<SharedSeat>* getSeats();
    MappedFile<SharedShow>* getShows();
    MappedFile<SharedBooking>* getBookings();
    MappedFile<SharedPayment>* getPayments();
    MappedFile<SharedRefund>* getRefunds();
    MappedFile<SharedTicket>* getTickets();
    MappedFile<SharedNotification>* getNotifications();
    MappedFile<SharedShowSeatAvailability>* getAvailability();
    MappedFile<SharedLog>* getLogs();
    int getUsersCount() const;
    int getScreenCount() const;
    int getSeatCount() const;
    int getLogsCount() const;
    void closeAll();
    ~MappedFileRegistry()
    {
        closeAll();
    }
};

