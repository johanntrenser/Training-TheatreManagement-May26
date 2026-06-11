/*
 * File: Enums.h
 * Description: Declares various enumerations used throughout the system,
 *              including statuses for theatres, movies, shows, screens,
 *              bookings, payments, notifications, users, seats, and refunds.
 *              Also provides utility functions to convert between enum values
 *              and their corresponding string representations.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>

namespace Enums {

    enum class TheatreStatus
    {
        ACTIVE,
        INACTIVE,
        UNDER_MAINTENANCE,
        PENDING
    };

    enum class MovieStatus
    {
        ACTIVE,
        INACTIVE,
        CANCELLED
    };

    enum class ShowStatus
    {
        SCHEDULED,
        RUNNING,
        COMPLETED,
        CANCELLED,
        NOT_FOUND
    };

    enum class ScreenStatus
    {
        AVAILABLE,
        UNAVAILABLE,
        UNDER_MAINTENANCE
    };

    enum class BookingStatus
    {
        CONFIRMED,
        CANCELLED,
        PENDING,
        COMPLETED,
        NOT_BOOKED
    };

    enum class PaymentStatus
    {
        SUCCESS,
        FAILED,
        PENDING,
        REFUNDED
    };

    enum class NotificationStatus
    {
        READ,
        UNREAD,
        SENT,
        DELIVERED,
        FAILED
    };

    enum class UserType
    {
        ADMIN,
        CUSTOMER,
        THEATRE_OWNER,
        USER_NOT_FOUND
    };

    enum class UserStatus
    {
        ACTIVE,
        INACTIVE,
        NOT_FOUND
    };

    enum class SeatStatus
    {
        AVAILABLE,
        BOOKED,
        RESERVED,
        BLOCKED
    };

    enum class RefundStatus
    {
        INITIATED,
        COMPLETED,
        FAILED
    };

    enum class PaymentMethod
    {
        UPI,
        CREDIT_CARD,
        DEBIT_CARD,
        UNKNOWN
    };

    enum class ProcessStatus
    {
        FAILED,
        SUCCESS,
        ALREADY_EXISTS,
        NOT_FOUND
    };

    enum class LoginStatus
    {
        USER_NOT_FOUND,
        INVALID_PASSWORD,
        USER_FOUND
    };

    enum class LogType
    {
        SYSTEM_ACTIVITY,
        ERROR,
        UNKNOWN
    };

    enum class TicketStatus
    {
        ACTIVE,
        CANCELLED,
        NOT_FOUND,
        COMPLETED
    };

    inline std::string getTheatreStatusString(TheatreStatus status)
    {
        switch (status)
        {
        case TheatreStatus::ACTIVE:
            return "ACTIVE";
        case TheatreStatus::INACTIVE:
            return "INACTIVE";
        case TheatreStatus::UNDER_MAINTENANCE:
            return "UNDER_MAINTENANCE";
        default:
            return "UNKNOWN";
        }
    }

    inline TheatreStatus getTheatreStatus(const std::string& input)
    {
        if (input == "ACTIVE")
        {
            return TheatreStatus::ACTIVE;
        }
        if (input == "INACTIVE")
        {
            return TheatreStatus::INACTIVE;
        }
        if (input == "UNDER_MAINTENANCE")
        {
            return TheatreStatus::UNDER_MAINTENANCE;
        }
        return TheatreStatus::INACTIVE;
    }

    inline std::string getMovieStatusString(MovieStatus status)
    {
        switch (status)
        {
        case MovieStatus::ACTIVE:
            return "ACTIVE";
        case MovieStatus::INACTIVE:
            return "INACTIVE";
        case MovieStatus::CANCELLED:
            return "CANCELLED";
        default:
            return "UNKNOWN";
        }
    }

    inline MovieStatus getMovieStatus(const std::string& input)
    {
        if (input == "ACTIVE")
        {
            return MovieStatus::ACTIVE;
        }
        if (input == "INACTIVE")
        {
            return MovieStatus::INACTIVE;
        }
        if (input == "CANCELLED")
        {
            return MovieStatus::CANCELLED;
        }
        return MovieStatus::CANCELLED;
    }

    inline std::string getShowStatusString(ShowStatus status)
    {
        switch (status)
        {
        case ShowStatus::SCHEDULED:
            return "SCHEDULED";
        case ShowStatus::RUNNING:
            return "RUNNING";
        case ShowStatus::COMPLETED:
            return "COMPLETED";
        case ShowStatus::CANCELLED:
            return "CANCELLED";
        case ShowStatus::NOT_FOUND:
            return "NOT_FOUND";
        default:
            return "NOT_FOUND";
        }
    }

    inline ShowStatus getShowStatus(const std::string& input)
    {
        if (input == "SCHEDULED")
        {
            return ShowStatus::SCHEDULED;
        }
        if (input == "RUNNING")
        {
            return ShowStatus::RUNNING;
        }
        if (input == "COMPLETED")
        {
            return ShowStatus::COMPLETED;
        }
        if (input == "CANCELLED")
        {
            return ShowStatus::CANCELLED;
        }
        if (input == "NOT_FOUND")
        {
            return ShowStatus::NOT_FOUND;
        }
        return ShowStatus::NOT_FOUND;
    }

    inline std::string getScreenStatusString(ScreenStatus status)
    {
        switch (status)
        {
        case ScreenStatus::AVAILABLE:
            return "AVAILABLE";
        case ScreenStatus::UNAVAILABLE:
            return "UNAVAILABLE";
        case ScreenStatus::UNDER_MAINTENANCE:
            return "UNDER_MAINTENANCE";
        default:
            return "UNKNOWN";
        }
    }

    inline ScreenStatus getScreenStatus(const std::string& input)
    {
        if (input == "AVAILABLE")
        {
            return ScreenStatus::AVAILABLE;
        }
        if (input == "UNAVAILABLE")
        {
            return ScreenStatus::UNAVAILABLE;
        }
        if (input == "UNDER_MAINTENANCE")
        {
            return ScreenStatus::UNDER_MAINTENANCE;
        }
        return ScreenStatus::UNAVAILABLE;
    }

    inline std::string getBookingStatusString(BookingStatus status)
    {
        switch (status)
        {
        case BookingStatus::CONFIRMED:
            return "CONFIRMED";
        case BookingStatus::CANCELLED:
            return "CANCELLED";
        case BookingStatus::PENDING:
            return "PENDING";
        case BookingStatus::COMPLETED:
            return "COMPLETED";
        case BookingStatus::NOT_BOOKED:
            return "NOT_BOOKED";
        default:
            return "UNKNOWN";
        }
    }

    inline BookingStatus getBookingStatus(const std::string& input)
    {
        if (input == "CONFIRMED")
        {
            return BookingStatus::CONFIRMED;
        }
        if (input == "CANCELLED")
        {
            return BookingStatus::CANCELLED;
        }
        if (input == "PENDING")
        {
            return BookingStatus::PENDING;
        }
        if (input == "COMPLETED")
        {
            return BookingStatus::COMPLETED;
        }
        if (input == "NOT_BOOKED")
        {
            return BookingStatus::NOT_BOOKED;
        }
        return BookingStatus::CANCELLED;
    }

    inline std::string getPaymentStatusString(PaymentStatus status)
    {
        switch (status)
        {
        case PaymentStatus::SUCCESS:
            return "SUCCESS";
        case PaymentStatus::FAILED:
            return "FAILED";
        case PaymentStatus::PENDING:
            return "PENDING";
        case PaymentStatus::REFUNDED:
            return "REFUNDED";
        default:
            return "UNKNOWN";
        }
    }

    inline PaymentStatus getPaymentStatus(const std::string& input)
    {
        if (input == "SUCCESS")
        {
            return PaymentStatus::SUCCESS;
        }
        if (input == "FAILED")
        {
            return PaymentStatus::FAILED;
        }
        if (input == "PENDING")
        {
            return PaymentStatus::PENDING;
        }
        if (input == "REFUNDED")
        {
            return PaymentStatus::REFUNDED;
        }
        return PaymentStatus::FAILED;
    }

    inline std::string getNotificationStatusString(NotificationStatus status)
    {
        switch (status)
        {
        case NotificationStatus::SENT:
            return "SENT";
        case NotificationStatus::DELIVERED:
            return "DELIVERED";
        case NotificationStatus::READ:
            return "READ";
        case NotificationStatus::FAILED:
            return "FAILED";
        case NotificationStatus::UNREAD:
            return "UNREAD";
        default:
            return "UNKNOWN";
        }
    }

    inline NotificationStatus getNotificationStatus(const std::string& input)
    {
        if (input == "SENT")
        {
            return NotificationStatus::SENT;
        }
        if (input == "DELIVERED")
        {
            return NotificationStatus::DELIVERED;
        }
        if (input == "READ")
        {
            return NotificationStatus::READ;
        }
        if (input == "FAILED")
        {
            return NotificationStatus::FAILED;
        }
        if (input == "UNREAD")
        {
            return NotificationStatus::UNREAD;
        }
        return NotificationStatus::FAILED;
    }

    inline std::string getUserTypeString(UserType type)
    {
        switch (type)
        {
        case UserType::ADMIN:
            return "ADMIN";
        case UserType::CUSTOMER:
            return "CUSTOMER";
        case UserType::THEATRE_OWNER:
            return "THEATRE_OWNER";
        default:
            return "UNKNOWN";
        }
    }

    inline UserType getUserType(const std::string& input)
    {
        if (input == "ADMIN")
        {
            return UserType::ADMIN;
        }
        if (input == "CUSTOMER")
        {
            return UserType::CUSTOMER;
        }
        if (input == "THEATRE_OWNER")
        {
            return UserType::THEATRE_OWNER;
        }
        return UserType::CUSTOMER;
    }

    inline std::string getUserStatusString(UserStatus status)
    {
        switch (status)
        {
        case UserStatus::ACTIVE:
            return "ACTIVE";
        case UserStatus::INACTIVE:
            return "INACTIVE";
        default:
            return "UNKNOWN";
        }
    }

    inline UserStatus getUserStatus(const std::string& input)
    {
        if (input == "ACTIVE")
        {
            return UserStatus::ACTIVE;
        }
        if (input == "INACTIVE")
        {
            return UserStatus::INACTIVE;
        }
        return UserStatus::INACTIVE;
    }

    inline std::string getSeatStatusString(SeatStatus status)
    {
        switch (status)
        {
        case SeatStatus::AVAILABLE:
            return "AVAILABLE";
        case SeatStatus::BOOKED:
            return "BOOKED";
        case SeatStatus::RESERVED:
            return "RESERVED";
        case SeatStatus::BLOCKED:
            return "BLOCKED";
        default:
            return "UNKNOWN";
        }
    }

    inline SeatStatus getSeatStatus(const std::string& input)
    {
        if (input == "AVAILABLE")
        {
            return SeatStatus::AVAILABLE;
        }
        if (input == "BOOKED")
        {
            return SeatStatus::BOOKED;
        }
        if (input == "RESERVED")
        {
            return SeatStatus::RESERVED;
        }
        if (input == "BLOCKED")
        {
            return SeatStatus::BLOCKED;
        }
        return SeatStatus::AVAILABLE;
    }

    inline std::string getRefundStatusString(RefundStatus status)
    {
        switch (status)
        {
        case RefundStatus::INITIATED:
            return "INITIATED";
        case RefundStatus::COMPLETED:
            return "COMPLETED";
        case RefundStatus::FAILED:
            return "FAILED";
        default:
            return "UNKNOWN";
        }
    }

    inline RefundStatus getRefundStatus(const std::string& input)
    {
        if (input == "INITIATED")
        {
            return RefundStatus::INITIATED;
        }
        if (input == "COMPLETED")
        {
            return RefundStatus::COMPLETED;
        }
        if (input == "FAILED")
        {
            return RefundStatus::FAILED;
        }
        return RefundStatus::FAILED;
    }

    inline std::string getLogTypeString(LogType type)
    {
        switch (type)
        {
        case LogType::SYSTEM_ACTIVITY:
            return "SYSTEM_ACTIVITY";
        case LogType::ERROR:
            return "ERROR";
        case LogType::UNKNOWN:
            return "UNKNOWN";
        default:
            return "UNKNOWN";
        }
    }

    inline LogType getLogType(const std::string& input)
    {
        if (input == "SYSTEM_ACTIVITY")
        {
            return LogType::SYSTEM_ACTIVITY;
        }
        if (input == "ERROR")
        {
            return LogType::ERROR;
        }
        if (input == "UNKNOWN")
        {
            return LogType::UNKNOWN;
        }
        return LogType::UNKNOWN;
    }

    inline TicketStatus getTicketStatus(const std::string& input)
    {
        if (input == "ACTIVE")
        {
            return TicketStatus::ACTIVE;
        }
        if (input == "CANCELLED")
        {
            return TicketStatus::CANCELLED;
        }
        if (input == "NOT_FOUND")
        {
            return TicketStatus::NOT_FOUND;
        }
        if (input == "COMPLETED")
        {
            return TicketStatus::COMPLETED;
        }
        return TicketStatus::NOT_FOUND;
    }

    inline std::string getTicketStatusString(TicketStatus status)
    {
        switch (status)
        {
        case TicketStatus::ACTIVE:
            return "ACTIVE";
        case TicketStatus::CANCELLED:
            return "CANCELLED";
        case TicketStatus::NOT_FOUND:
            return "NOT_FOUND";
        case TicketStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
        }
    }

    inline std::string getPaymentMethodString(PaymentMethod method)
    {
        switch (method)
        {
        case PaymentMethod::CREDIT_CARD:
            return "CREDIT_CARD";
        case PaymentMethod::DEBIT_CARD:
            return "DEBIT_CARD";
        case PaymentMethod::UPI:
            return "UPI";
        default:
            return "UNKNOWN";
        }
    }

    inline PaymentMethod getPaymentMethod(const std::string& input)
    {
        if (input == "CREDIT_CARD")
        {
            return PaymentMethod::CREDIT_CARD;
        }
        if (input == "DEBIT_CARD")
        {
            return PaymentMethod::DEBIT_CARD;
        }
        if (input == "UPI")
        {
            return PaymentMethod::UPI;
        }
        return PaymentMethod::UNKNOWN;
    }

}