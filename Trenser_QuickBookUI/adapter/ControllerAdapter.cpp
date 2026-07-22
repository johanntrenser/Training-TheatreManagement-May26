/*
 * File: ControllerAdapter.cpp
 * Description: Implements the ControllerAdapter class, which acts as a bridge between
 *              the Qt-based UI layer and the backend Controller. This adapter manages
 *              initialization of all service modules (authentication, user, theatre,
 *              movie, show, booking, payment, ticket, notification, log, screen, seat,
 *              and refund), and exposes high-level methods for login, logout, user
 *              registration, profile management, and user lifecycle operations. It also
 *              provides utility functions for converting enums to strings and retrieving
 *              active/inactive users. Exception handling and Qt signals are used to
 *              ensure robust communication with the UI.
 * Author: Trenser
 * Created: 22 July 2026
 */

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

/*
 * Function: ControllerAdapter::ControllerAdapter
 * Description: Constructs a ControllerAdapter object, initializing the base QObject
 *              with the provided parent.
 * Parameters:
 *    parent (QObject*) - Optional parent object for QObject hierarchy
 * Returns:
 *    None
 */
ControllerAdapter::ControllerAdapter(QObject *parent)
    : QObject(parent) {}

/*
 * Function: ControllerAdapter::~ControllerAdapter
 * Description: Destructor for ControllerAdapter. Cleans up allocated resources
 *              by deleting the Controller instance.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
ControllerAdapter::~ControllerAdapter() {
    delete m_controller;
}

/*
 * Function: ControllerAdapter::initialize
 * Description: Initializes the ControllerAdapter by creating a Controller instance
 *              with all required management services.
 * Parameters:
 *    None
 * Returns:
 *    bool - true if initialization succeeds, false otherwise
 */
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

/*
 * Function: ControllerAdapter::login
 * Description: Authenticates a user with the given email and password.
 * Parameters:
 *    email (const QString&) - User's email address
 *    password (const QString&) - User's password
 * Returns:
 *    int - Login status code (Enums::LoginStatus)
 */

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
            const User* user = m_controller->getAuthenticatedUser();
            if (user)
            {
                m_event->init(user->getUserId());
                m_event->startListener(
                    userTypeToString(user->getUserType()).toStdString(),
                    user->getUserId(),
                    user->getUserName(),
                    [this](const std::string& message)
                    {
                        QString qMessage = QString::fromStdString(message);
                        QMetaObject::invokeMethod(this, [this, qMessage]() {
                            emit notificationReceived(qMessage);
                        }, Qt::QueuedConnection);
                    });
            }
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

/*
 * Function: ControllerAdapter::logout
 * Description: Logs out the currently authenticated user and resets authentication state.
 * Parameters:
 *    None
 * Returns:
 *    void
 */

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

/*
 * Function: ControllerAdapter::registerUser
 * Description: Registers a new user with the provided details and role.
 * Parameters:
 *    name (const QString&) - Full name of the user
 *    email (const QString&) - Email address of the user
 *    password (const QString&) - Password for the account
 *    phone (const QString&) - Phone number of the user
 *    userType (EnumsAdapter::UserType) - Role of the user (Customer, TheatreOwner, Admin)
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::getProfile
 * Description: Retrieves the profile details of the currently authenticated user.
 * Parameters:
 *    None
 * Returns:
 *    QVariantMap - Map containing user details (id, name, email, phone, role, password)
 */
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

/*
 * Function: ControllerAdapter::getActiveUsers
 * Description: Fetches a list of all active users in the system.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of active users with their details
 */
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

/*
 * Function: ControllerAdapter::getInactiveUsers
 * Description: Fetches a list of all inactive users in the system.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of inactive users with their details
 */
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

/*
 * Function: ControllerAdapter::createUser
 * Description: Creates a new user with the given details and role.
 * Parameters:
 *    name (const QString&) - Full name of the user
 *    email (const QString&) - Email address of the user
 *    password (const QString&) - Password for the account
 *    phone (const QString&) - Phone number of the user
 *    role (const QString&) - Role of the user (admin, theatreOwner, customer)
 * Returns:
 *    bool - true if user creation succeeds, false otherwise
 */
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

/*
 * Function: ControllerAdapter::stringToUserType
 * Description: Converts a role string into its corresponding Enums::UserType value.
 * Parameters:
 *    role (const QString&) - Role string ("admin", "theatreOwner", or defaults to "customer")
 * Returns:
 *    Enums::UserType - The mapped user type (ADMIN, THEATRE_OWNER, CUSTOMER)
 */
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

/*
 * Function: ControllerAdapter::deactivateUser
 * Description: Deactivates a user account by its unique identifier.
 * Parameters:
 *    userId (const QString&) - Unique identifier of the user
 * Returns:
 *    bool - true if the user was successfully deactivated, false otherwise
 */
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

/*
 * Function: ControllerAdapter::reactivateUser
 * Description: Reactivates a previously deactivated user account by its unique identifier.
 * Parameters:
 *    userId (const QString&) - Unique identifier of the user
 * Returns:
 *    bool - true if the user was successfully reactivated, false otherwise
 */
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

/*
 * Function: ControllerAdapter::updateProfile
 * Description: Updates the profile information of the authenticated user.
 * Parameters:
 *    name (const QString&) - New name (optional)
 *    email (const QString&) - New email (optional)
 *    phone (const QString&) - New phone number (optional)
 * Returns:
 *    bool - true if any field was updated, false otherwise
 */
bool ControllerAdapter::updateProfile(const QString& name, const QString& email, const QString& phoneNumber)
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
        if (!phoneNumber.isEmpty())
        {
            m_controller->setAuthenticatedUserPhoneNumber(phoneNumber.toStdString());
            flag=true;
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "Update profile error:" << ex.what();
    }
    return flag;
}

/*
 * Function: ControllerAdapter::changePassword
 * Description: Changes the password of the authenticated user.
 * Parameters:
 *    currentPassword (const QString&) - Current password
 *    newPassword (const QString&) - New password
 * Returns:
 *    bool - true if password change succeeds, false otherwise
 */
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

/*
 * Function: ControllerAdapter::isEmailUnique
 * Description: Checks whether the given email is unique in the system.
 * Parameters:
 *    email (const QString&) - Email address to check
 * Returns:
 *    bool - true if email is unique, false otherwise
 */
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

/*
 * Function: ControllerAdapter::isPhoneNumberUnique
 * Description: Checks whether the given phone number is unique in the system.
 * Parameters:
 *    phoneNumber (const QString&) - Phone number to check
 * Returns:
 *    bool - true if phone number is unique, false otherwise
 */
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

QVariantList ControllerAdapter::getMyTheatres()
{
    QVariantList theatres;
    try {
        for (const Theatre* theatre : m_controller->getCurrentOwnerTheatres())
        {
            QVariantMap map;
            map["id"] = QString::fromStdString(theatre->getTheatreId());
            map["name"] = QString::fromStdString(theatre->getName());
            map["city"] = QString::fromStdString(theatre->getCity());
            map["address"] = QString::fromStdString(theatre->getAddress());
            map["phone"] = QString::fromStdString(theatre->getTheatrePhoneNumber());
            map["status"] = QString::fromStdString(Enums::getTheatreStatusString(theatre->getStatus()));
            theatres.append(map);
        }
        for (const Theatre* theatre : m_controller->getCurrentOwnerInactiveTheatres())
        {
            QVariantMap map;
            map["id"] = QString::fromStdString(theatre->getTheatreId());
            map["name"] = QString::fromStdString(theatre->getName());
            map["city"] = QString::fromStdString(theatre->getCity());
            map["address"] = QString::fromStdString(theatre->getAddress());
            map["phone"] = QString::fromStdString(theatre->getTheatrePhoneNumber());
            map["status"] = QString::fromStdString(Enums::getTheatreStatusString(theatre->getStatus()));
            theatres.append(map);
        }
    }
    catch(const std::exception &ex)
    {
        qDebug() << "Get theatres error:" << ex.what();
    }
    return theatres;
}

int ControllerAdapter::addTheatre(const QString& name, const QString& city, const QString& address, const QString& phone, const QString& email)
{
    std::string stdName = name.toStdString();
    std::string stdCity = city.toStdString();
    std::string stdAddress = address.toStdString();
    std::string stdPhone = phone.toStdString();
    std::string stdEmail = email.toStdString();
    try
    {
        //Uniqueness Check
        if (m_controller->isTheatreEmailUnique(stdEmail) == Enums::ProcessStatus::FAILED) {
            return static_cast<int>(EnumsAdapter::ProcessStatus::EMAIL_ALREADY_EXISTS);
        }
        if (m_controller->isTheatrePhoneNumberUnique(stdPhone) ==  Enums::ProcessStatus::FAILED) {
            return static_cast<int>(EnumsAdapter::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS);
        }
        if(m_controller->isTheatreUnique(stdName, stdCity, stdAddress, stdPhone, stdEmail) == Enums::ProcessStatus::FAILED)
        {
            return static_cast<int>(EnumsAdapter::ProcessStatus::ALREADY_EXISTS);
        }
        Enums::ProcessStatus status = m_controller->addTheatre(stdName, stdCity, stdAddress, stdPhone, stdEmail);
        return static_cast<int>(status);
    }
    catch(const std::exception &ex)
    {
        qDebug() << "Add theatre error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

int ControllerAdapter::updateTheatre(const QString& theatreId, const QString& name, const QString& city,
                                     const QString& address, const QString& phone, const QString& email)
{
    std::string stdId = theatreId.toStdString();
    std::string stdName = name.toStdString();
    std::string stdCity = city.toStdString();
    std::string stdAddress = address.toStdString();
    std::string stdPhone = phone.toStdString();
    std::string stdEmail = email.toStdString();
    bool allSuccess = true;
    try {
        // ONLY check uniqueness if QML sent a NEW email
        if (!stdEmail.empty()) {
            if (m_controller->isTheatreEmailUnique(stdEmail) == Enums::ProcessStatus::FAILED) {
                return static_cast<int>(EnumsAdapter::ProcessStatus::EMAIL_ALREADY_EXISTS);
            }
        }
        // ONLY check uniqueness if QML sent a NEW phone number
        if (!stdPhone.empty()) {
            if (m_controller->isTheatrePhoneNumberUnique(stdPhone) == Enums::ProcessStatus::FAILED) {
                return static_cast<int>(EnumsAdapter::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS);
            }
        }
        // Skip global uniqueness check and go straight to updating the fields!
        if (!stdName.empty())
            allSuccess &= (m_controller->setTheatreNameById(stdId, stdName) == Enums::ProcessStatus::SUCCESS);
        if (!stdCity.empty())
            allSuccess &= (m_controller->setTheatreCityById(stdId, stdCity) == Enums::ProcessStatus::SUCCESS);
        if (!stdAddress.empty())
            allSuccess &= (m_controller->setTheatreAddressById(stdId, stdAddress) == Enums::ProcessStatus::SUCCESS);
        if (!stdPhone.empty())
            allSuccess &= (m_controller->setTheatrePhoneNumberById(stdId, stdPhone) == Enums::ProcessStatus::SUCCESS);
        if (!stdEmail.empty())
            allSuccess &= (m_controller->setTheatreEmailById(stdId, stdEmail) == Enums::ProcessStatus::SUCCESS);
        if (allSuccess) {
            return static_cast<int>(EnumsAdapter::ProcessStatus::SUCCESS);
        }
    } catch(const std::exception &ex) {
        qDebug() << "Update theatre error:" << ex.what();
    }

    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

int ControllerAdapter::deactivateTheatre(const QString& theatreId)
{
    try {
        Enums::TheatreStatus status = Enums::TheatreStatus::INACTIVE;
        Enums::ProcessStatus resultStatus = m_controller->setTheatreStatusById(theatreId.toStdString(), status);
        return static_cast<int>(resultStatus);
    } catch(const std::exception &ex) {
        qDebug() << "Deactivate error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

int ControllerAdapter::reactivateTheatre(const QString& theatreId)
{
    try {
         Enums::TheatreStatus status = Enums::TheatreStatus::PENDING;
        Enums::ProcessStatus resultStatus = m_controller->setTheatreStatusById(theatreId.toStdString(), status);
        return static_cast<int>(resultStatus);
    } catch(const std::exception &ex) {
        qDebug() << "Reactivate error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

QVariantList ControllerAdapter::getAllTheatres()
{
    QVariantList theatres;
    try {
        for (const Theatre* theatre : m_controller->getAllTheatres())
        {
            QVariantMap map;
            map["id"] = QString::fromStdString(theatre->getTheatreId());
            map["name"] = QString::fromStdString(theatre->getName());
            map["city"] = QString::fromStdString(theatre->getCity());
            map["address"] = QString::fromStdString(theatre->getAddress());
            map["phone"] = QString::fromStdString(theatre->getTheatrePhoneNumber());
            map["status"] = QString::fromStdString(Enums::getTheatreStatusString(theatre->getStatus()));
            theatres.append(map);
        }
    }
    catch(const std::exception &ex)
    {
        qDebug() << "Get theatres error:" << ex.what();
    }
    return theatres;
}

int ControllerAdapter::approveTheatre(const QString& theatreId)
{
    std::string stdTheatreId = theatreId.toStdString();
    Enums::TheatreStatus status = Enums::TheatreStatus::ACTIVE;
    if(m_controller->setTheatreStatusById(stdTheatreId, status) == Enums::ProcessStatus::SUCCESS)
    {
        qInfo() << "Admin approved theatre:" << theatreId;
        return static_cast<int>(EnumsAdapter::ProcessStatus::SUCCESS);
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

int ControllerAdapter::rejectTheatre(const QString& theatreId)
{
    std::string stdTheatreId = theatreId.toStdString();
    Enums::TheatreStatus status = Enums::TheatreStatus::PENDING;
    if(m_controller->setTheatreStatusById(stdTheatreId, status) == Enums::ProcessStatus::SUCCESS)
    {
        qInfo() << "Admin rejected theatre:" << theatreId;
        return static_cast<int>(EnumsAdapter::ProcessStatus::SUCCESS);
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}


/*
 * Function: ControllerAdapter::getUnreadNotifications
 * Description: Retrieves a batch of unread notifications from the controller,
 *              wrapping each message into a QVariantMap for structured output.
 * Parameters:
 *    batchSize (const int) - Maximum number of notifications to fetch in one call
 * Returns:
 *    QVariantList - List of unread notifications, each represented as a QVariantMap
 *                   with a "message" field
 */
QVariantList ControllerAdapter::getUnreadNotifications(const int batchSize)
{
    QVariantList unreadedNotificationList;
    int remainingUnreadCount = 0;

    try {
        // Call core C++ controller which accepts (int, int&)
        std::vector<std::string> notifications = m_controller->getUnreadNotifications(batchSize, remainingUnreadCount);

        for (const std::string& msg : notifications) {
            QVariantMap notifMap;
            notifMap["message"] = QString::fromStdString(msg);
            unreadedNotificationList.append(notifMap);
        }
    }
    catch (const std::exception &ex) {
        qDebug() << "Notification fetch error:" << ex.what();
    }

    return unreadedNotificationList;
}