#pragma once

namespace config
{
    namespace File
    {
        constexpr const char* FILEPATH = "Files";
        constexpr const char* BOOKING_FILEPATH = "Files/Booking.dat";
        constexpr const char* LOG_FILEPATH = "Files/Log.dat";
        constexpr const char* MOVIE_FILEPATH = "Files/Movie.dat";
        constexpr const char* NOTIFICATION_FILEPATH = "Files/Notification.dat";
        constexpr const char* PAYMENT_FILEPATH = "Files/Payment.dat";
        constexpr const char* REFUND_FILEPATH = "Files/Refund.dat";
        constexpr const char* SCREEN_FILEPATH = "Files/Screen.dat";
        constexpr const char* SEAT_FILEPATH = "Files/Seat.dat";
        constexpr const char* SHOW_FILEPATH = "Files/Show.dat";
        constexpr const char* THEATRE_FILEPATH = "Files/Theatre.dat";
        constexpr const char* USER_FILEPATH = "Files/User.dat";
        constexpr const char* TICKET_FILEPATH = "Files/Ticket.dat";
        constexpr const char* SHOW_SEAT_FILEPATH = "Files/ShowSeatAvailability.dat";
    }
    namespace Header
    {
        constexpr const char* BOOKING_HEADER = "BOOKING ID, CUSTOMER ID, SHOW ID, BOOKED SEAT ID, STATUS, AMOUNT";
        constexpr const char* LOG_HEADER = "LOG ID,DESCRIPTION";
        constexpr const char* MOVIE_HEADER = "MOVIE ID,MOVIE TITLE,LANGUAGE,GENRE,DURATION,STATUS";
        constexpr const char* NOTIFICATION_HEADER = "NOTIFICATION ID,RECEIVER ID,MESSAGE,STATUS,TIME";
        constexpr const char* PAYMENT_HEADER = "PAYMENT ID,BOOKING ID,AMOUNT,PAYMENT METHOD,STATUS,TIME STAMP";
        constexpr const char* REFUND_HEADER = "REFUND ID,BOOKED TICKET ID,REFUND AMOUNT,TIME,STATUS";
        constexpr const char* SCREEN_HEADER = "SCREEN ID,THEATRE ID,NAME,TOTAL ROWS,TOTAL COLUMNS,SEAT ID,SCREEN STATUS";
        constexpr const char* SEAT_HEADER = "SEAT ID,SCREEN ID,SEAT ROW,SEAT COLUMN,SEAT AMOUNT,STATUS";
        constexpr const char* SHOW_HEADER = "SHOW ID,MOVIE ID,SCREEN ID,STARTING TIME,ENDING TIME,SEAT AVAILABILITY ID,STATUS";
        constexpr const char* THEATRE_HEADER = "THEATRE ID,THEATRE NAME,CITY,ADDRESS,PHONE NUMBER,EMAIL,THEATRE OWNER, STATUS, SCREENS ID, MOVIES ID";
        constexpr const char* USER_HEADER = "USER ID,USER NAME,EMAIL,PASSWORD,PHONE NUMBER,USER TYPE,STATUS";
        constexpr const char* TICKET_HEADER = "TICKET ID,PAYMENT ID,CUSTOMER ID,TICKET STATUS";
        constexpr const char* SHOW_SEAT_HEADER = "SHOW SEAT AVAILABLITY ID,SHOW ID,SHOW SEAT AVAILABLITY MAP";
    }
    namespace delimeter
    {
        constexpr const char* comma = ",";
        constexpr const char* verticalBar = "|";
        constexpr const char* colon = ":";
    }
    namespace Limit
    {
        constexpr const int ID_MAX_LENGTH = 20;
        constexpr const int USERNAME_MAX_LENGTH = 20;
        constexpr const int NAME_MAX_LENGTH = 100;
        constexpr const int EMAIL_MAX_LENGTH = 100;
        constexpr const int PASSWORD_MAX_LENGTH = 64;
        constexpr const int PHONENUMBER_MAX_LENGTH = 15;
        constexpr const int LANGUAGE_MAX_LENGTH = 30;
        constexpr const int GENRE_MAX_LENGTH = 50;
        constexpr const int CITY_MAX_LENGTH = 50;
        constexpr const int ADDRESS_MAX_LENGTH = 200;
        constexpr const int SCREEN_MAX_COUNT = 50;
        constexpr const int MOVIE_MAX_COUNT = 100;
        constexpr const int SEAT_MAX_COUNT = 1000;
        constexpr const int TIME_MAX_LENGTH = 30;
        constexpr const int MESSAGE_MAX_LENGTH = 500;
        constexpr const int INITIAL_FILE_MAP_SIZE = 1000;
        constexpr const int SESSION_MAX_COUNT = 1000;
        constexpr const int MAX_SHARED_BUFFER_SIZE = 1000;
    }
    namespace FileMappings
    {
        constexpr const char* BOOKING_MAPPING_NAME = "QuickBookBookings";
        constexpr const char* LOG_MAPPING_NAME = "QuickBookLogs";
        constexpr const char* MOVIE_MAPPING_NAME = "QuickBookMovies";
        constexpr const char* NOTIFICATION_MAPPING_NAME = "QuickBookNotifications";
        constexpr const char* PAYMENT_MAPPING_NAME = "QuickBookPayments";
        constexpr const char* REFUND_MAPPING_NAME = "QuickBookRefunds";
        constexpr const char* SCREEN_MAPPING_NAME = "QuickBookScreens";
        constexpr const char* SEAT_MAPPING_NAME = "QuickBookSeats";
        constexpr const char* SHOW_MAPPING_NAME = "QuickBookShows";
        constexpr const char* THEATRE_MAPPING_NAME = "QuickBookTheatres";
        constexpr const char* USER_MAPPING_NAME = "QuickBookUsers";
        constexpr const char* TICKET_MAPPING_NAME = "QuickBookTickets";
        constexpr const char* SHOW_SEAT_MAPPING_NAME = "QuickBookAvailability";
        constexpr const char* SESSION_MAPPING_NAME = "QuickBookSessions";
    }
    namespace MutexMappings
    {
        constexpr const char* BOOKING_MUTEX_NAME = "QuickBookBookingsMutex";
        constexpr const char* LOG_MUTEX_NAME = "QuickBookLogsMutex";
        constexpr const char* MOVIE_MUTEX_NAME = "QuickBookMoviesMutex";
        constexpr const char* NOTIFICATION_MUTEX_NAME = "QuickBookNotificationsMutex";
        constexpr const char* PAYMENT_MUTEX_NAME = "QuickBookPaymentsMutex";
        constexpr const char* REFUND_MUTEX_NAME = "QuickBookRefundsMutex";
        constexpr const char* SCREEN_MUTEX_NAME = "QuickBookScreensMutex";
        constexpr const char* SEAT_MUTEX_NAME = "QuickBookSeatsMutex";
        constexpr const char* SHOW_MUTEX_NAME = "QuickBookShowsMutex";
        constexpr const char* THEATRE_MUTEX_NAME = "QuickBookTheatresMutex";
        constexpr const char* USER_MUTEX_NAME = "QuickBookUsersMutex";
        constexpr const char* TICKET_MUTEX_NAME = "QuickBookTicketsMutex";
        constexpr const char* SHOW_SEAT_MUTEX_NAME = "QuickBookAvailabilityMutex";
        constexpr const char* SESSION_MUTEX_NAME = "QuickBookSessionsMutex";
    }
}