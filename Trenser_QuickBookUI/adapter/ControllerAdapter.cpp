#include "ControllerAdapter.h"
#include <utility>
#include <QDebug>
#include "AuthenticationManagementService.h"
#include "UserManagementService.h"
#include "TheatreManagementService.h"
#include "MovieManagementService.h"
#include "ShowManagementService.h"
#include "BookingManagementService.h"
#include "PaymentManagementService.h"
#include "TicketManagementService.h"
#include "NotificationManagementService.h"
#include "LogManagementService.h"
#include "ScreenManagementService.h"
#include "SeatManagementService.h"
#include "RefundManagementService.h"

ControllerAdapter::ControllerAdapter(QObject *parent)
    : QObject(parent) {}

ControllerAdapter::~ControllerAdapter() {
    delete m_controller;
}

bool ControllerAdapter::initialize() {
    if (m_initialized) return true;

    m_controller = new Controller(
        new AuthenticationManagementService(),
        new UserManagementService(),
        new TheatreManagementService(),
        new MovieManagementService(),
        new ShowManagementService(),
        new BookingManagementService(),
        new PaymentManagementService(),
        new TicketManagementService(),
        new NotificationManagementService(),
        new LogManagementService(),
        new ScreenManagementService(),
        new SeatManagementService(),
        new RefundManagementService()
        );

    m_initialized = true;
    return true;
}

int ControllerAdapter::login(const QString &email, const QString &password) {
    Enums::LoginStatus  status;
    try
    {
        if (!m_initialized) {
            initialize();
        }
        std::pair<Enums::LoginStatus, Enums::UserType> loginResult =
            m_controller->login(email.toStdString(), password.toStdString());
        status = loginResult.first;
        Enums::UserType type = loginResult.second;
        if (status == Enums::LoginStatus::USER_FOUND) {
            m_authenticated = true;
            m_currentUserType = static_cast<EnumsAdapter::UserType>(type);
            emit authenticationChanged();
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Login error:" << ex.what();
    }
    return static_cast<int>(status);
}

void ControllerAdapter::logout() {
    try
    {
        m_authenticated = false;
        m_currentUserType = EnumsAdapter::UserType::USER_NOT_FOUND;
        m_controller->logout();
        emit authenticationChanged();
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Logout error:" << ex.what();
    }
}

int ControllerAdapter::registerUser(const QString& name,
                                            const QString& email,
                                            const QString& password,
                                            const QString& phone,
                                            const EnumsAdapter::UserType userType)
{
    Enums::ProcessStatus status;
    try{
        if (!m_initialized || !m_controller)
        {
            qWarning() << "Registration failed: Backend not initialized!";
            return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
        }
        std::string stdName = name.toStdString();
        std::string stdEmail = email.toStdString();
        std::string stdPassword = password.toStdString();
        std::string stdPhone = phone.toStdString();
        Enums::UserType type = Enums::UserType::USER_NOT_FOUND;
        if(userType == EnumsAdapter::UserType::CUSTOMER)
        {
            type = Enums::UserType::CUSTOMER;
        }
        else if(userType == EnumsAdapter::UserType::THEATRE_OWNER)
        {
            type = Enums::UserType::THEATRE_OWNER;
        }
        else
        {
            type = Enums::UserType::ADMIN;
        }
        status = m_controller->registerUser(
            stdName,
            stdEmail,
            stdPassword,
            stdPhone,
            type
            );
        qInfo() << "Registered theatre owner:" << email
                << "Status Code:" << static_cast<int>(status);
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Registration error:" << ex.what();
    }
    return static_cast<int>(status);
}

QVariantMap ControllerAdapter::getProfile()
{
    QVariantMap userMap;
    try{
        const User* user = m_controller->getAuthenticatedUser();
        if (!user) return userMap;
        userMap["id"] = QString::fromStdString(user->getUserId());
        userMap["name"] = QString::fromStdString(user->getUserName());
        userMap["email"] = QString::fromStdString(user->getEmail());
        userMap["phone"] = QString::fromStdString(user->getPhoneNumber());
        userMap["role"] = userTypeToString(user->getUserType());
        userMap["password"] = QString::fromStdString(user->getPassword());
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Get Profile error:" << ex.what();
    }
    return userMap;
}

QString ControllerAdapter::userTypeToString(Enums::UserType type)
{
    try{
        switch (type)
        {
        case Enums::UserType::ADMIN: return "Admin";
        case Enums::UserType::THEATRE_OWNER: return "TheatreOwner";
        case Enums::UserType::CUSTOMER: return "Customer";
        default: return "";
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Get user type to string error:" << ex.what();
    }
    return "";
}

QVariantList ControllerAdapter::getActiveUsers()
{
    QVariantList userlist;
    try{
        for (const User* user : m_controller->getActiveUsers())
        {
            QVariantMap userMap;
            userMap["id"] = QString::fromStdString(user->getUserId());
            userMap["name"] = QString::fromStdString(user->getUserName());
            userMap["email"] = QString::fromStdString(user->getEmail());
            userMap["phone"] = QString::fromStdString(user->getPhoneNumber());
            userMap["type"] = userTypeToString(user->getUserType());
            userMap["status"] = userStatusToString(user->getStatus());
            userlist.append(userMap);
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Get active user error:" << ex.what();
    }
    return userlist;
}

QVariantList ControllerAdapter::getInactiveUsers()
{
    QVariantList userlist;
    try{
        for (const User* user : m_controller->getInactiveUsers())
        {
            QVariantMap userMap;
            userMap["id"] = QString::fromStdString(user->getUserId());
            userMap["name"] = QString::fromStdString(user->getUserName());
            userMap["email"] = QString::fromStdString(user->getEmail());
            userMap["phone"] = QString::fromStdString(user->getPhoneNumber());
            userMap["type"] = userTypeToString(user->getUserType());
            userMap["status"] = userStatusToString(user->getStatus());
            userlist.append(userMap);
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Get inactive user error:" << ex.what();
    }
    return userlist;
}

QString ControllerAdapter::userStatusToString(Enums::UserStatus status)
{
    try{
        switch (status)
        {
        case Enums::UserStatus::ACTIVE: return "ACTIVE";
        case Enums::UserStatus::INACTIVE: return "INACTIVE";
        case Enums::UserStatus::NOT_FOUND: return "NOT_FOUND";
        default: return "";
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Get user status to string error:" << ex.what();
    }
    return "";
}

bool ControllerAdapter::createUser(const QString& name, const QString& email, const QString& password,const QString& phone, const QString& role)
{
    try{
        return m_controller->createUser(name.toStdString(), email.toStdString(), password.toStdString(),phone.toStdString(), stringToUserType(role)) == Enums::ProcessStatus::SUCCESS;
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Create user error:" << ex.what();
    }
    return false;
}

Enums::UserType ControllerAdapter::stringToUserType(const QString& role)
{
    try{
        if (role == "admin")
        {
            return Enums::UserType::ADMIN;
        }
        if (role == "theatreOwner") {
            return Enums::UserType::THEATRE_OWNER;
        }
        return Enums::UserType::CUSTOMER;
    }
    catch (const std::exception &ex)
    {
        qDebug() << "String to user type error:" << ex.what();
    }
}

bool ControllerAdapter::deactivateUser(const QString& userId)
{
    try{
        return m_controller->deactivateUser(userId.toStdString()) == Enums::ProcessStatus::SUCCESS;
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Deactivate user error:" << ex.what();
    }
    return false;
}

bool ControllerAdapter::reactivateUser(const QString& userId)
{
    try{
        return m_controller->reactivateUser(userId.toStdString()) == Enums::ProcessStatus::SUCCESS;
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Reactivate user error:" << ex.what();
    }
    return false;
}

bool ControllerAdapter::updateProfile(const QString& name, const QString& email, const QString& phone)
{
    bool flag = false;
    try{
        if (!name.isEmpty()) {
            m_controller->setAuthenticatedUserUserName(name.toStdString());
            flag=true;
        }
        if (!email.isEmpty())
        {
            m_controller->setAuthenticatedUserEmail(email.toStdString());
            flag=true;
        }
        if (!phone.isEmpty())
        {
            m_controller->setAuthenticatedUserPhoneNumber(phone.toStdString());
            flag=true;
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Update profile error:" << ex.what();
    }
    return flag;
}

bool ControllerAdapter::changePassword(const QString& currentPassword, const QString& newPassword)
{
    try{
        return m_controller->changePassword(currentPassword.toStdString(), newPassword.toStdString()) == Enums::ProcessStatus::SUCCESS;
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Change password error:" << ex.what();
    }
    return false;
}
bool ControllerAdapter::isEmailUnique(const QString& email)
{
    try{
        return Enums::ProcessStatus::SUCCESS == m_controller->isEmailUnique(email.toStdString());
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Email Unique error:" << ex.what();
    }
    return false;
}

bool ControllerAdapter::isPhoneNumberUnique(const QString& phoneNumber)
{
    try{
        return Enums::ProcessStatus::SUCCESS == m_controller->isPhoneNumberUnique(phoneNumber.toStdString());
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Phone number Unique error:" << ex.what();
    }
    return false;
}