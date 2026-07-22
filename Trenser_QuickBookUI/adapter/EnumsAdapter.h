/*
 * File: EnumsAdapter.h
 * Description: Declares the EnumsAdapter class, which exposes strongly typed
 *              enumerations from the backend Enums namespace to QML. This adapter
 *              provides QML-accessible constants for theatre, movie, show, screen,
 *              booking, payment, notification, user, seat, refund, process, login,
 *              log, and ticket statuses. It ensures type safety and readability
 *              when working with enums in QML by registering them with Qt’s meta-object
 *              system. The class is uncreatable in QML and serves purely as a namespace
 *              for constants.
 * Author: Trenser
 * Created: 22 July 2026
 */
#ifndef ENUMSADAPTER_H
#define ENUMSADAPTER_H

#include <QObject>
#include <qqmlintegration.h>
#include "Enums.h"

class EnumsAdapter : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("EnumsAdapter is an uncreatable namespace for constants only")
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")

public:
    // 1. TheatreStatus
    enum class TheatreStatus {
        ACTIVE = static_cast<int>(Enums::TheatreStatus::ACTIVE),
        INACTIVE = static_cast<int>(Enums::TheatreStatus::INACTIVE),
        UNDER_MAINTENANCE = static_cast<int>(Enums::TheatreStatus::UNDER_MAINTENANCE),
        PENDING = static_cast<int>(Enums::TheatreStatus::PENDING)
    };
    Q_ENUM(TheatreStatus)

    // 2. MovieStatus
    enum class MovieStatus {
        ACTIVE = static_cast<int>(Enums::MovieStatus::ACTIVE),
        INACTIVE = static_cast<int>(Enums::MovieStatus::INACTIVE),
        CANCELLED = static_cast<int>(Enums::MovieStatus::CANCELLED)
    };
    Q_ENUM(MovieStatus)

    // 3. ShowStatus
    enum class ShowStatus {
        SCHEDULED = static_cast<int>(Enums::ShowStatus::SCHEDULED),
        RUNNING = static_cast<int>(Enums::ShowStatus::RUNNING),
        COMPLETED = static_cast<int>(Enums::ShowStatus::COMPLETED),
        CANCELLED = static_cast<int>(Enums::ShowStatus::CANCELLED),
        NOT_FOUND = static_cast<int>(Enums::ShowStatus::NOT_FOUND)
    };
    Q_ENUM(ShowStatus)

    // 4. ScreenStatus
    enum class ScreenStatus {
        AVAILABLE = static_cast<int>(Enums::ScreenStatus::AVAILABLE),
        UNAVAILABLE = static_cast<int>(Enums::ScreenStatus::UNAVAILABLE),
        UNDER_MAINTENANCE = static_cast<int>(Enums::ScreenStatus::UNDER_MAINTENANCE)
    };
    Q_ENUM(ScreenStatus)

    // 5. BookingStatus
    enum class BookingStatus {
        CONFIRMED = static_cast<int>(Enums::BookingStatus::CONFIRMED),
        CANCELLED = static_cast<int>(Enums::BookingStatus::CANCELLED),
        PENDING = static_cast<int>(Enums::BookingStatus::PENDING),
        COMPLETED = static_cast<int>(Enums::BookingStatus::COMPLETED),
        NOT_BOOKED = static_cast<int>(Enums::BookingStatus::NOT_BOOKED)
    };
    Q_ENUM(BookingStatus)

    // 6. PaymentStatus
    enum class PaymentStatus {
        SUCCESS = static_cast<int>(Enums::PaymentStatus::SUCCESS),
        FAILED = static_cast<int>(Enums::PaymentStatus::FAILED),
        PENDING = static_cast<int>(Enums::PaymentStatus::PENDING),
        REFUNDED = static_cast<int>(Enums::PaymentStatus::REFUNDED)
    };
    Q_ENUM(PaymentStatus)

    // 7. NotificationStatus
    enum class NotificationStatus {
        READ = static_cast<int>(Enums::NotificationStatus::READ),
        UNREAD = static_cast<int>(Enums::NotificationStatus::UNREAD),
        SENT = static_cast<int>(Enums::NotificationStatus::SENT),
        DELIVERED = static_cast<int>(Enums::NotificationStatus::DELIVERED),
        FAILED = static_cast<int>(Enums::NotificationStatus::FAILED)
    };
    Q_ENUM(NotificationStatus)

    // 8. UserType
    enum class UserType {
        ADMIN = static_cast<int>(Enums::UserType::ADMIN),
        CUSTOMER = static_cast<int>(Enums::UserType::CUSTOMER),
        THEATRE_OWNER = static_cast<int>(Enums::UserType::THEATRE_OWNER),
        USER_NOT_FOUND = static_cast<int>(Enums::UserType::USER_NOT_FOUND)
    };
    Q_ENUM(UserType)

    // 9. UserStatus
    enum class UserStatus {
        ACTIVE = static_cast<int>(Enums::UserStatus::ACTIVE),
        INACTIVE = static_cast<int>(Enums::UserStatus::INACTIVE),
        NOT_FOUND = static_cast<int>(Enums::UserStatus::NOT_FOUND)
    };
    Q_ENUM(UserStatus)

    // 10. SeatStatus
    enum class SeatStatus {
        AVAILABLE = static_cast<int>(Enums::SeatStatus::AVAILABLE),
        BOOKED = static_cast<int>(Enums::SeatStatus::BOOKED),
        RESERVED = static_cast<int>(Enums::SeatStatus::RESERVED),
        BLOCKED = static_cast<int>(Enums::SeatStatus::BLOCKED)
    };
    Q_ENUM(SeatStatus)

    // 11. RefundStatus
    enum class RefundStatus {
        INITIATED = static_cast<int>(Enums::RefundStatus::INITIATED),
        COMPLETED = static_cast<int>(Enums::RefundStatus::COMPLETED),
        FAILED = static_cast<int>(Enums::RefundStatus::FAILED)
    };
    Q_ENUM(RefundStatus)

    // 12. PaymentMethod
    enum class PaymentMethod {
        UPI = static_cast<int>(Enums::PaymentMethod::UPI),
        CREDIT_CARD = static_cast<int>(Enums::PaymentMethod::CREDIT_CARD),
        DEBIT_CARD = static_cast<int>(Enums::PaymentMethod::DEBIT_CARD),
        UNKNOWN = static_cast<int>(Enums::PaymentMethod::UNKNOWN)
    };
    Q_ENUM(PaymentMethod)

    // 13. ProcessStatus
    enum class ProcessStatus {
        FAILED = static_cast<int>(Enums::ProcessStatus::FAILED),
        SUCCESS = static_cast<int>(Enums::ProcessStatus::SUCCESS),
        ALREADY_EXISTS = static_cast<int>(Enums::ProcessStatus::ALREADY_EXISTS),
        NOT_FOUND = static_cast<int>(Enums::ProcessStatus::NOT_FOUND),
        EMAIL_ALREADY_EXISTS = static_cast<int>(Enums::ProcessStatus::EMAIL_ALREADY_EXISTS),
        PHONE_NUMBER_ALREADY_EXISTS = static_cast<int>(Enums::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS)
    };
    Q_ENUM(ProcessStatus)

    // 14. LoginStatus
    enum class LoginStatus {
        USER_NOT_FOUND = static_cast<int>(Enums::LoginStatus::USER_NOT_FOUND),
        INVALID_PASSWORD = static_cast<int>(Enums::LoginStatus::INVALID_PASSWORD),
        USER_FOUND = static_cast<int>(Enums::LoginStatus::USER_FOUND),
        USER_ALREADY_LOGGED_IN = static_cast<int>(Enums::LoginStatus::USER_ALREADY_LOGGED_IN)
    };
    Q_ENUM(LoginStatus)

    // 15. LogType
    enum class LogType {
        SYSTEM_ACTIVITY = static_cast<int>(Enums::LogType::SYSTEM_ACTIVITY),
        ERROR_LOG = static_cast<int>(Enums::LogType::ERROR_LOG),
        UNKNOWN = static_cast<int>(Enums::LogType::UNKNOWN)
    };
    Q_ENUM(LogType)

    // 16. TicketStatus
    enum class TicketStatus {
        ACTIVE = static_cast<int>(Enums::TicketStatus::ACTIVE),
        CANCELLED = static_cast<int>(Enums::TicketStatus::CANCELLED),
        NOT_FOUND = static_cast<int>(Enums::TicketStatus::NOT_FOUND),
        COMPLETED = static_cast<int>(Enums::TicketStatus::COMPLETED)
    };
    Q_ENUM(TicketStatus)
};

#endif // ENUMSADAPTER_H