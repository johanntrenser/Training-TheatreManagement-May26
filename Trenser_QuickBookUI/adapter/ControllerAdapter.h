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
#include "Controller.h"
#include "EnumsAdapter.h"
#include "Enums.h"

class ControllerAdapter : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isAuthenticated READ isAuthenticated NOTIFY authenticationChanged)
    Q_PROPERTY(EnumsAdapter::UserType userType READ userType NOTIFY authenticationChanged)
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
    Q_INVOKABLE bool updateProfile(const QString& name, const QString& email, const QString& phone);
    Q_INVOKABLE bool changePassword(const QString& currentPassword, const QString& newPassword);
    Q_INVOKABLE bool isEmailUnique(const QString& email);
    Q_INVOKABLE bool isPhoneNumberUnique(const QString& phoneNumber);
signals:
    void authenticationChanged();

private:
    Controller* m_controller = nullptr;
    bool m_authenticated = false;
    EnumsAdapter::UserType m_currentUserType = EnumsAdapter::UserType::USER_NOT_FOUND;
    bool m_initialized = false;

    static QString userTypeToString(Enums::UserType type);
    static QString userStatusToString(Enums::UserStatus status);
    static QString movieStatusToString(Enums::MovieStatus status);
    static Enums::UserType stringToUserType(const QString& role);
};

#endif // CONTROLLERADAPTER_H