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
    try
    {
        if (!m_initialized) {
            initialize();
        }

        std::pair<Enums::LoginStatus, Enums::UserType> loginResult =
            m_controller->login(email.toStdString(), password.toStdString());

        Enums::LoginStatus status = loginResult.first;
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
        Enums::ProcessStatus status = m_controller->registerUser(
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
    try{
        QVariantMap m;
        const User* user = m_controller->getAuthenticatedUser();
        if (!user) return m;
        m["id"] = QString::fromStdString(user->getUserId());
        m["name"] = QString::fromStdString(user->getUserName());
        m["email"] = QString::fromStdString(user->getEmail());
        m["phone"] = QString::fromStdString(user->getPhoneNumber());
        m["role"] = userTypeToString(user->getUserType());
        m["password"] = QString::fromStdString(user->getPassword());
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Get Profile error:" << ex.what();
    }
    return m;
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
}