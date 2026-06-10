///*
// * File: DataStore.h
// * Description: Defines the DataStore singleton class responsible for managing
// *              in-memory storage of system entities such as users, bookings,
// *              logs, movies, notifications, payments, refunds, screens, seats,
// *              shows, show seat availability, theatres, and tickets.
// * Author: Trenser
// * Created: 20 May 2026
// */
//#pragma once
//#include <string>
//#include <map>
//#include "User.h"
//#include "Booking.h"
//#include "Log.h"
//#include "Movie.h"
//#include "Notification.h"
//#include "Payment.h"
//#include "Refund.h"
//#include "Screen.h"
//#include "Seat.h"
//#include "Show.h"
//#include "ShowSeatAvailability.h"
//#include "Theatre.h"
//#include "Ticket.h"
//class DataStore
//{
//private:
//	DataStore() : m_currentUser(nullptr) {}
//	User* m_currentUser;
//	std::map<std::string, User*> m_users;
//	std::map<std::string, Booking*> m_bookings;
//	std::map<std::string, Log*> m_logs;
//	std::map<std::string, Movie*> m_movies;
//	std::map<std::string, Notification*> m_notifications;
//	std::map<std::string, Payment*> m_payments;
//	std::map<std::string, Refund*> m_refunds;
//	std::map<std::string, Screen*> m_screens;
//	std::map<std::string, Seat*> m_seats;
//	std::map<std::string, Show*> m_shows;
//	std::map<std::string, ShowSeatAvailability*> m_showSeatAvailabilitys;
//	std::map<std::string, Theatre*> m_theatres;
//	std::map<std::string, Ticket*> m_tickets;
//public:
//	const std::map<std::string, User*>& getUsers() const;
//	void addUser(User* user);
//	const std::map<std::string, Log*>& getLogs() const;
//	void addLog(Log* log);
//	DataStore(const DataStore&) = delete;
//	DataStore& operator=(const DataStore&) = delete;
//	static DataStore& getInstance();
//	User* getAuthenticatedUser() const;
//	void setAuthenticatedUser(User* user);
//	const Enums::UserType getAuthenticatedUserType() const;
//	void setAuthenticatedUserEmail(const std::string& email);
//	void setAuthenticatedUserPhoneNumber(const std::string& phoneNumber);
//	void setAuthenticatedUserName(const std::string& userName);
//	std::map<std::string, Notification*>& getNotifications();
//	const std::map<std::string, Movie*>& getMovies() const;
//	void addMovieToSystem(Movie* movie);
//	const std::map<std::string, Show*>& getShows() const;
//	std::map<std::string, Show*>& getShowsForUpdation();
//	const std::map<std::string, Screen*>& getScreens() const;
//	const std::map<std::string, Theatre*>& getTheatres() const;
//	const std::map<std::string, Booking*>& getBookings() const;
//	void addTheatre(Theatre* theatre);
//	Theatre* getTheatreById(const std::string& theatreId) const;
//	Movie* getMovieById(const std::string& movieId) const;
//	Screen* getScreenById(const std::string& screenId) const;
//	void addShow(Show* show);
//	const std::map<std::string, ShowSeatAvailability*>& getShowSeatAvailabilitys() const;
//	void addShowSeatAvailability(ShowSeatAvailability* showSeatAvailability);
//	const Show* getShowById(const std::string& showId);
//	Show* getShowByIdForUpdation(const std::string& showId);
//	std::map<std::string, Ticket*>& getTickets();
//	void addTicket(Ticket* ticket);
//	Ticket* getTicketById(const std::string& ticketId) const;
//	void addPayment(Payment* payment);
//	void addRefund(Refund* refund);
//	const std::map < std::string, Payment*>& getPayments() const;
//	const std::map<std::string, Refund*>& getRefunds() const;
//	const Booking* getBookingById(const std::string& bookingId);
//	Booking* getBookingByIdForUpdation(const std::string& bookingId);
//	Seat* getSeatById(const std::string seatId);
//	void addBooking(Booking* booking);
//	const Ticket* getTicketForBooking(const Booking* booking);
//	const std::map<std::string, Seat*>& getSeats() const;
//	User* getUserById(std::string& id);
//	Show* getShowDetailsById(std::string& id);
//	void addSeat(Seat* seat);
//	void addScreen(Screen* screen);
//	void addNotification(Notification* notification);
//	Booking* getBookingDetailsById(const std::string& bookingId);
//	std::map<std::string, ShowSeatAvailability*> getShowSeatAvailabilityList();
//	void addShowSeatAvailabilityList(ShowSeatAvailability* showSeatAvailability);
//	~DataStore();
//};


/*
 * File: DataStore.h
 */
#pragma once
#include "SharedMemoryManager.h"
#include "SharedSizesManager.h"
#include "SharedStructs.h"
#include "SharedMemoryConfig.h"

class DataStore
{
private:
    DataStore() {}

    SharedSizesManager m_sizesManager;

    SharedMemoryManager<SharedUser>* m_users = nullptr;
    SharedMemoryManager<SharedMovie>* m_movies = nullptr;
    SharedMemoryManager<SharedTheatre>* m_theatres = nullptr;
    SharedMemoryManager<SharedScreen>* m_screens = nullptr;
    SharedMemoryManager<SharedSeat>* m_seats = nullptr;
    SharedMemoryManager<SharedShow>* m_shows = nullptr;
    SharedMemoryManager<SharedShowSeatEntry>* m_showSeats = nullptr;
    SharedMemoryManager<SharedBooking>* m_bookings = nullptr;
    SharedMemoryManager<SharedPayment>* m_payments = nullptr;
    SharedMemoryManager<SharedTicket>* m_tickets = nullptr;
    SharedMemoryManager<SharedRefund>* m_refunds = nullptr;
    SharedMemoryManager<SharedNotification>* m_notifications = nullptr;
    SharedMemoryManager<SharedLog>* m_logs = nullptr;

    char m_currentUserId[20] = {};

public:
    DataStore(const DataStore&) = delete;
    DataStore& operator=(const DataStore&) = delete;

    static DataStore& getInstance()
    {
        static DataStore instance;
        return instance;
    }

    bool initialize()
    {
        if (!m_sizesManager.initialize()) { return false; }

        m_users = new SharedMemoryManager<SharedUser>(
            SHM_NAME_USERS, MTX_NAME_USERS,
            L"Files/Users.dat",
            m_sizesManager.getUserSizeInfo());

        m_movies = new SharedMemoryManager<SharedMovie>(
            SHM_NAME_MOVIES, MTX_NAME_MOVIES,
            L"Files/Movies.dat",
            m_sizesManager.getMovieSizeInfo());

        m_theatres = new SharedMemoryManager<SharedTheatre>(
            SHM_NAME_THEATRES, MTX_NAME_THEATRES,
            L"Files/Theatres.dat",
            m_sizesManager.getTheatreSizeInfo());

        m_screens = new SharedMemoryManager<SharedScreen>(
            SHM_NAME_SCREENS, MTX_NAME_SCREENS,
            L"Files/Screens.dat",
            m_sizesManager.getScreenSizeInfo());

        m_seats = new SharedMemoryManager<SharedSeat>(
            SHM_NAME_SEATS, MTX_NAME_SEATS,
            L"Files/Seats.dat",
            m_sizesManager.getSeatSizeInfo());

        m_shows = new SharedMemoryManager<SharedShow>(
            SHM_NAME_SHOWS, MTX_NAME_SHOWS,
            L"Files/Shows.dat",
            m_sizesManager.getShowSizeInfo());

        m_showSeats = new SharedMemoryManager<SharedShowSeatEntry>(
            SHM_NAME_NOTIFS, MTX_NAME_NOTIFS,
            L"Files/ShowSeats.dat",
            m_sizesManager.getShowSeatSizeInfo());

        m_bookings = new SharedMemoryManager<SharedBooking>(
            SHM_NAME_BOOKINGS, MTX_NAME_BOOKINGS,
            L"Files/Bookings.dat",
            m_sizesManager.getBookingSizeInfo());

        m_payments = new SharedMemoryManager<SharedPayment>(
            SHM_NAME_PAYMENTS, MTX_NAME_PAYMENTS,
            L"Files/Payments.dat",
            m_sizesManager.getPaymentSizeInfo());

        m_tickets = new SharedMemoryManager<SharedTicket>(
            SHM_NAME_TICKETS, MTX_NAME_TICKETS,
            L"Files/Tickets.dat",
            m_sizesManager.getTicketSizeInfo());

        m_refunds = new SharedMemoryManager<SharedRefund>(
            SHM_NAME_REFUNDS, MTX_NAME_REFUNDS,
            L"Files/Refunds.dat",
            m_sizesManager.getRefundSizeInfo());

        m_notifications = new SharedMemoryManager<SharedNotification>(
            SHM_NAME_NOTIFS, MTX_NAME_NOTIFS,
            L"Files/Notifications.dat",
            m_sizesManager.getNotificationSizeInfo());

        m_logs = new SharedMemoryManager<SharedLog>(
            SHM_NAME_LOGS, MTX_NAME_LOGS,
            L"Files/Logs.dat",
            m_sizesManager.getLogSizeInfo());

        return m_users->initialize() &&
            m_movies->initialize() &&
            m_theatres->initialize() &&
            m_screens->initialize() &&
            m_seats->initialize() &&
            m_shows->initialize() &&
            m_showSeats->initialize() &&
            m_bookings->initialize() &&
            m_payments->initialize() &&
            m_tickets->initialize() &&
            m_refunds->initialize() &&
            m_notifications->initialize() &&
            m_logs->initialize();
    }

    // ── accessors ────────────────────────────────────────────────
    SharedMemoryManager<SharedUser>& getUsers()
    {
        return *m_users;
    }

    SharedMemoryManager<SharedMovie>& getMovies()
    {
        return *m_movies;
    }

    SharedMemoryManager<SharedTheatre>& getTheatres()
    {
        return *m_theatres;
    }

    SharedMemoryManager<SharedScreen>& getScreens()
    {
        return *m_screens;
    }

    SharedMemoryManager<SharedSeat>& getSeats()
    {
        return *m_seats;
    }

    SharedMemoryManager<SharedShow>& getShows()
    {
        return *m_shows;
    }

    SharedMemoryManager<SharedShowSeatEntry>& getShowSeats()
    {
        return *m_showSeats;
    }

    SharedMemoryManager<SharedBooking>& getBookings()
    {
        return *m_bookings;
    }

    SharedMemoryManager<SharedPayment>& getPayments()
    {
        return *m_payments;
    }

    SharedMemoryManager<SharedTicket>& getTickets()
    {
        return *m_tickets;
    }

    SharedMemoryManager<SharedRefund>& getRefunds()
    {
        return *m_refunds;
    }

    SharedMemoryManager<SharedNotification>& getNotifications()
    {
        return *m_notifications;
    }

    SharedMemoryManager<SharedLog>& getLogs()
    {
        return *m_logs;
    }

    SharedSizesManager& getSizesManager()
    {
        return m_sizesManager;
    }

    // ── current user (per process) ────────────────────────────────
    const char* getCurrentUserId() const
    {
        return m_currentUserId;
    }

    void setCurrentUserId(const char* id)
    {
        strncpy_s(m_currentUserId, id, sizeof(m_currentUserId));
    }

    void clearCurrentUser()
    {
        memset(m_currentUserId, 0, sizeof(m_currentUserId));
    }

    ~DataStore()
    {
        delete m_users;
        delete m_movies;
        delete m_theatres;
        delete m_screens;
        delete m_seats;
        delete m_shows;
        delete m_showSeats;
        delete m_bookings;
        delete m_payments;
        delete m_tickets;
        delete m_refunds;
        delete m_notifications;
        delete m_logs;
    }
};