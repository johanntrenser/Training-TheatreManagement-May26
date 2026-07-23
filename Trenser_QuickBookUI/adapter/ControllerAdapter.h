/*
 * File: ControllerAdapter.h
 * Description: Declares the ControllerAdapter class, which serves as a Qt-based
 *              adapter between the UI layer and the backend Controller. This class
 *              exposes high-level QML-invokable methods for authentication, user
 *              registration, profile management, and user lifecycle operations.
 *              It also provides utility functions for converting enums to strings
 *              and mapping roles. Signals are used to notify the UI of authentication
 *              state changes, ensuring seamless integration with QML components.
 * Author: Trenser
 * Created: 22 July 2026
 */

#ifndef CONTROLLERADAPTER_H
#define CONTROLLERADAPTER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <qqmlintegration.h>
#include <ctime>
#include <QDateTime>
#include <unordered_set>
#include <QDebug>
#include <vector>
#include <string>
#include <exception>
#include "Controller.h"
#include "NotificationEvent.h"
#include "EnumsAdapter.h"
#include "Enums.h"

class ControllerAdapter : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isAuthenticated READ isAuthenticated NOTIFY authenticationChanged)
    Q_PROPERTY(EnumsAdapter::UserType userType READ userType NOTIFY authenticationChanged)
    Q_PROPERTY(QVariantList bookings READ bookings NOTIFY bookingsChanged)
    Q_PROPERTY(QVariantList payments READ payments NOTIFY paymentsChanged)
    Q_PROPERTY(QVariantList refunds READ refunds NOTIFY refundsChanged)
public:
    explicit ControllerAdapter(QObject *parent = nullptr);
    ~ControllerAdapter();
    Q_INVOKABLE bool initialize();
    Q_INVOKABLE int login(const QString &email, const QString &password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE int registerUser(const QString& name,
                                     const QString& email,
                                     const QString& password,
                                     const QString& phone,
                                     const EnumsAdapter::UserType userType);

    bool isAuthenticated() const { return m_authenticated; }
    EnumsAdapter::UserType userType() const { return m_currentUserType; }
    Q_INVOKABLE QVariantMap getProfile();
    Q_INVOKABLE QVariantList getActiveUsers();
    Q_INVOKABLE QVariantList getInactiveUsers();
    Q_INVOKABLE bool createUser(const QString& name, const QString& email, const QString& password,const QString& phone, const QString& role);
    Q_INVOKABLE bool deactivateUser(const QString& userId);
    Q_INVOKABLE bool reactivateUser(const QString& userId);
    Q_INVOKABLE bool updateProfile(const QString& name, const QString& email, const QString& phoneNumber);
    Q_INVOKABLE bool changePassword(const QString& currentPassword, const QString& newPassword);
    Q_INVOKABLE bool isEmailUnique(const QString& email);
    Q_INVOKABLE bool isPhoneNumberUnique(const QString& phoneNumber);
    // ------TheatreOwner: Theatres-------
    Q_INVOKABLE QVariantList getMyTheatres();
    Q_INVOKABLE int addTheatre(const QString& name, const QString& city,
                               const QString& address, const QString& phone,
                               const QString& email);
    Q_INVOKABLE int updateTheatre(const QString& theatreId, const QString& name, const QString& city,
                                  const QString& address, const QString& phone, const QString& email);
    Q_INVOKABLE int deactivateTheatre(const QString& theatreId);
    Q_INVOKABLE int reactivateTheatre(const QString& theatreId);
    // ------Admin: Theatres-------
    Q_INVOKABLE QVariantList getAllTheatres();
    Q_INVOKABLE int approveTheatre(const QString& theatreId);
    Q_INVOKABLE int rejectTheatre(const QString& theatreId);
    Q_INVOKABLE QVariantList getUnreadNotifications(const int batchSize);
    Q_INVOKABLE int isMovieUnique(
        const QString& title,
        const QString& language,
        const QString& genre,
        int duration);
    Q_INVOKABLE int addMovie(
        const QString& title,
        const QString& language,
        const QString& genre,
        int duration);
    Q_INVOKABLE QVariantList getAllMovies();
    Q_INVOKABLE int updateMovie(
        const QString& movieId,
        const QString& currentTitle,
        const QString& currentLanguage,
        const QString& currentGenre,
        int currentDuration,
        const QString& updatedTitle,
        const QString& updatedLanguage,
        const QString& updatedGenre,
        int updatedDuration);
    Q_INVOKABLE int deactivateMovie(const QString& movieId);
    Q_INVOKABLE int reactivateMovie(const QString& movieId);
    Q_INVOKABLE QVariantList getLog(const QString& type);

    QVariantList bookings() const { return m_bookings; }
    Q_INVOKABLE void loadBookings();
    QString displayTimeAndDate(time_t time) const;
    Q_INVOKABLE QVariantMap cancelBooking(const QString& bookingId);

    QVariantList payments() const { return m_payments; }
    Q_INVOKABLE void loadPayments();
    QVariantMap paymentToMap(const Payment* payment) const;

    QVariantList refunds() const { return m_refunds; }
    Q_INVOKABLE void loadRefunds();
    QVariantMap refundToMap(const Refund* refund) const;
    // --------Theatre - Movie management ---
    Q_INVOKABLE QVariantList getMoviesInTheatre(const QString& theatreId);
    Q_INVOKABLE QVariantList getActiveMovies();
    Q_INVOKABLE int addMovieToTheatre(const QString& theatreId, const QString& movieId);
    Q_INVOKABLE int removeMovieFromTheatre(const QString& theatreId, const QString& movieId);
    // --------Screen Management ---
    Q_INVOKABLE QVariantList getScreensInTheatre(const QString& theatreId);
    Q_INVOKABLE int addScreenToTheatre(const QString& theatreId, const QString& screenName, int seatRows, int seatColumns, double seatAmount);
    Q_INVOKABLE int updateScreenName(const QString& theatreId, const QString& screenId, const QString& newName);
    Q_INVOKABLE int deactivateScreen(const QString& theatreId, const QString& screenId);
    Q_INVOKABLE int reactivateScreen(const QString& theatreId, const QString& screenId);
signals:
    void authenticationChanged();
    void notificationReceived(const QString& message);
    void bookingsChanged();
    void paymentsChanged();
    void refundsChanged();
private:
    Controller* m_controller = nullptr;
    bool m_authenticated = false;
    NotificationEvent* m_event;
    EnumsAdapter::UserType m_currentUserType = EnumsAdapter::UserType::USER_NOT_FOUND;
    bool m_initialized = false;
    int m_cachedRemainingUnread = 0;

    static QString userTypeToString(Enums::UserType type);
    static QString userStatusToString(Enums::UserStatus status);
    static QString movieStatusToString(Enums::MovieStatus status);
    static Enums::UserType stringToUserType(const QString& role);
    QVariantMap convertMovieToVariantMap(const Movie* movie) const;
    QVariantMap bookingToMap(const Booking* booking) const;
    QVariantList m_bookings;
    QVariantList m_payments;
    QVariantList m_refunds;
};

#endif