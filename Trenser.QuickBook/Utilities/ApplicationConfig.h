#pragma once

namespace config
{
    namespace File
    {
        constexpr const char* BOOKING_FILEPATH = "Files/Booking.csv";
        constexpr const char* LOG_FILEPATH = "Files/Log.csv";
        constexpr const char* MOVIE_FILEPATH = "Files/Movie.csv";
        constexpr const char* NOTIFICATION_FILEPATH = "Files/Notification.csv";
        constexpr const char* PAYMENT_FILEPATH = "Files/Payment.csv";
        constexpr const char* REFUND_FILEPATH = "Files/Refund.csv";
        constexpr const char* SCREEN_FILEPATH = "Files/Screen.csv";
        constexpr const char* SEAT_FILEPATH = "Files/Seat.csv";
        constexpr const char* SHOW_FILEPATH = "Files/Show.csv";
        constexpr const char* THEATRE_FILEPATH = "Files/Theatre.csv";
        constexpr const char* USER_FILEPATH = "Files/User.csv";
        constexpr const char* TICKET_FILEPATH = "Files/Ticket.csv";
        constexpr const char* SHOW_SEAT_FILEPATH = "Files/ShowSeatAvailability.csv";
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
}