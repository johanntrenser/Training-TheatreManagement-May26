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
#include <QDateTime>
#include <string>
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
            if(user==m_controller->getAuthenticatedUser() || user->getUserId()=="US001"){
                continue;
            }
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

/*
 * Function: ControllerAdapter::getMyTheatres
 * Description: Retrieves all theatres owned by the currently authenticated theatre owner,
 *              including both active and inactive theatres, and converts them into QVariantMap objects.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of theatres with details including id, name, city, address, phone, and status
 */
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

/*
 * Function: ControllerAdapter::addTheatre
 * Description: Adds a new theatre to the system after performing uniqueness checks
 *              on email, phone number, and theatre details.
 * Parameters:
 *    name (const QString&) - Name of the theatre
 *    city (const QString&) - City where the theatre is located
 *    address (const QString&) - Address of the theatre
 *    phone (const QString&) - Theatre's phone number
 *    email (const QString&) - Theatre's email address
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::updateTheatre
 * Description: Updates the details of an existing theatre by its unique identifier.
 *              Performs uniqueness checks only if new email or phone number is provided.
 * Parameters:
 *    theatreId (const QString&) - Unique identifier of the theatre
 *    name (const QString&) - Updated name (optional)
 *    city (const QString&) - Updated city (optional)
 *    address (const QString&) - Updated address (optional)
 *    phone (const QString&) - Updated phone number (optional)
 *    email (const QString&) - Updated email address (optional)
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::deactivateTheatre
 * Description: Deactivates a theatre by setting its status to INACTIVE.
 * Parameters:
 *    theatreId (const QString&) - Unique identifier of the theatre
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::reactivateTheatre
 * Description: Reactivates a previously deactivated theatre by setting its status to PENDING.
 * Parameters:
 *    theatreId (const QString&) - Unique identifier of the theatre
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::getAllTheatres
 * Description: Retrieves all theatres in the system and converts them into QVariantMap objects.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of theatres with details including id, name, city, address, phone, and status
 */
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

/*
 * Function: ControllerAdapter::approveTheatre
 * Description: Approves a theatre by setting its status to ACTIVE.
 * Parameters:
 *    theatreId (const QString&) - Unique identifier of the theatre
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::rejectTheatre
 * Description: Rejects a theatre by setting its status to PENDING.
 * Parameters:
 *    theatreId (const QString&) - Unique identifier of the theatre
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
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

/*
 * Function: ControllerAdapter::convertMovieToVariantMap
 * Description: Converts a Movie object into a QVariantMap containing its details.
 * Parameters:
 *    movie (const Movie*) - Pointer to the Movie object
 * Returns:
 *    QVariantMap - Map with movieId, title, language, genre, duration, and active status
 */
QVariantMap ControllerAdapter::convertMovieToVariantMap(const Movie* movie) const
{
    QVariantMap map;
    if (!movie)
    {
        return map;
    }

    map["movieId"] = QString::fromStdString(movie->getMovieId());
    map["title"] = QString::fromStdString(movie->getTitle());
    map["language"] = QString::fromStdString(movie->getLanguage());
    map["genre"] = QString::fromStdString(movie->getGenre());
    map["duration"] = movie->getDuration();
    map["active"] =
        (movie->getStatus() == Enums::MovieStatus::ACTIVE);

    return map;
}

/*
 * Function: ControllerAdapter::getAllMovies
 * Description: Retrieves all movies (both active and inactive) from the controller
 *              and converts them into QVariantMap objects for UI consumption.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of all movies with their details
 */
QVariantList ControllerAdapter::getAllMovies()
{
    QVariantList movieList;

    auto activeMovies = m_controller->getAllActiveMovies();
    auto inactiveMovies = m_controller->getAllInactiveMovies();

    qDebug() << "Active movies:" << activeMovies.size();
    qDebug() << "Inactive movies:" << inactiveMovies.size();

    for (const Movie* movie : activeMovies)
    {
        qDebug()
        << QString::fromStdString(movie->getMovieId())
        << QString::fromStdString(movie->getTitle())
        << (movie->getStatus() == Enums::MovieStatus::ACTIVE ? "ACTIVE" : "INACTIVE");

        movieList.append(convertMovieToVariantMap(movie));
    }

    for (const Movie* movie : inactiveMovies)
    {
        qDebug()
        << QString::fromStdString(movie->getMovieId())
        << QString::fromStdString(movie->getTitle())
        << (movie->getStatus() == Enums::MovieStatus::ACTIVE ? "ACTIVE" : "INACTIVE");

        movieList.append(convertMovieToVariantMap(movie));
    }

    return movieList;
}

/*
 * Function: ControllerAdapter::isMovieUnique
 * Description: Checks whether a movie with the given attributes already exists in the system.
 * Parameters:
 *    title (const QString&) - Title of the movie
 *    language (const QString&) - Language of the movie
 *    genre (const QString&) - Genre of the movie
 *    duration (int) - Duration of the movie in minutes
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
int ControllerAdapter::isMovieUnique(const QString& title,
                                     const QString& language,
                                     const QString& genre,
                                     int duration)
{
    Enums::ProcessStatus status;

    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "Movie uniqueness check failed: Backend not initialized!";
            return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
        }

        status = m_controller->isMovieUnique(
            title.toStdString(),
            language.toStdString(),
            genre.toStdString(),
            duration);

    }
    catch (const std::exception& ex)
    {
        qDebug() << "Movie uniqueness check error:" << ex.what();
        return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
    }

    return static_cast<int>(status);
}

/*
 * Function: ControllerAdapter::addMovie
 * Description: Adds a new movie to the system with the provided details.
 * Parameters:
 *    title (const QString&) - Title of the movie
 *    language (const QString&) - Language of the movie
 *    genre (const QString&) - Genre of the movie
 *    duration (int) - Duration of the movie in minutes
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
int ControllerAdapter::addMovie(const QString& title,
                                const QString& language,
                                const QString& genre,
                                int duration)
{
    Enums::ProcessStatus status;

    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "Add movie failed: Backend not initialized!";
            return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
        }

        status = m_controller->addMovie(
            title.toStdString(),
            language.toStdString(),
            genre.toStdString(),
            duration);

        if (status == Enums::ProcessStatus::SUCCESS)
        {
            qInfo() << "Movie added:" << title << "Status Code:" << static_cast<int>(status);
        }
        else
        {
            qWarning() << "Movie could not be added:" << title;
        }
    }
    catch (const std::exception& ex)
    {
        qDebug() << "Add movie error:" << ex.what();
        return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
    }

    return static_cast<int>(status);
}

/*
 * Function: ControllerAdapter::updateMovie
 * Description: Updates the details of an existing movie if changes are detected.
 *              Ensures uniqueness before applying updates to title, language,
 *              genre, or duration.
 * Parameters:
 *    movieId (const QString&) - Unique identifier of the movie
 *    currentTitle (const QString&) - Current title of the movie
 *    currentLanguage (const QString&) - Current language of the movie
 *    currentGenre (const QString&) - Current genre of the movie
 *    currentDuration (int) - Current duration of the movie
 *    updatedTitle (const QString&) - Updated title of the movie
 *    updatedLanguage (const QString&) - Updated language of the movie
 *    updatedGenre (const QString&) - Updated genre of the movie
 *    updatedDuration (int) - Updated duration of the movie
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
int ControllerAdapter::updateMovie(
    const QString& movieId,
    const QString& currentTitle,
    const QString& currentLanguage,
    const QString& currentGenre,
    int currentDuration,
    const QString& updatedTitle,
    const QString& updatedLanguage,
    const QString& updatedGenre,
    int updatedDuration)
{
    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "Movie update failed: Backend not initialized!";
            return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
        }

        const std::string movieIdString = movieId.toStdString();

        const std::string currentTitleString = currentTitle.toStdString();
        const std::string currentLanguageString = currentLanguage.toStdString();
        const std::string currentGenreString = currentGenre.toStdString();

        const std::string updatedTitleString = updatedTitle.toStdString();
        const std::string updatedLanguageString = updatedLanguage.toStdString();
        const std::string updatedGenreString = updatedGenre.toStdString();

        const bool movieDetailsChanged =
            currentTitleString != updatedTitleString ||
            currentLanguageString != updatedLanguageString ||
            currentGenreString != updatedGenreString ||
            currentDuration != updatedDuration;

        if (!movieDetailsChanged)
        {
            return static_cast<int>(EnumsAdapter::ProcessStatus::SUCCESS);
        }

        Enums::ProcessStatus processStatus =
            m_controller->isMovieUnique(
                updatedTitleString,
                updatedLanguageString,
                updatedGenreString,
                updatedDuration);

        if (processStatus == Enums::ProcessStatus::FAILED)
        {
            return static_cast<int>(EnumsAdapter::ProcessStatus::ALREADY_EXISTS);
        }

        if (currentTitleString != updatedTitleString)
        {
            processStatus = m_controller->setMovieTitleByID(
                movieIdString,
                updatedTitleString);

            if (processStatus != Enums::ProcessStatus::SUCCESS)
            {
                return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
            }
        }

        if (currentLanguageString != updatedLanguageString)
        {
            processStatus = m_controller->setMovieLanguageByID(
                movieIdString,
                updatedLanguageString);

            if (processStatus != Enums::ProcessStatus::SUCCESS)
            {
                return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
            }
        }

        if (currentGenreString != updatedGenreString)
        {
            processStatus = m_controller->setMovieGenreByID(
                movieIdString,
                updatedGenreString);

            if (processStatus != Enums::ProcessStatus::SUCCESS)
            {
                return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
            }
        }

        if (currentDuration != updatedDuration)
        {
            processStatus = m_controller->setMovieDurationByID(
                movieIdString,
                updatedDuration);

            if (processStatus != Enums::ProcessStatus::SUCCESS)
            {
                return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
            }
        }

        return static_cast<int>(EnumsAdapter::ProcessStatus::SUCCESS);
    }
    catch (const std::exception& exception)
    {
        qCritical() << "Movie update failed:" << exception.what();
    }
    catch (...)
    {
        qCritical() << "Unknown exception occurred while updating movie.";
    }

    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::deactivateMovie
 * Description: Deactivates a movie in the system by its unique identifier.
 * Parameters:
 *    movieId (const QString&) - Unique identifier of the movie
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
int ControllerAdapter::deactivateMovie(const QString& movieId)
{
    Enums::ProcessStatus processStatus;

    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "Movie deactivation failed: Backend not initialized!";
            return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
        }

        processStatus = m_controller->deactivateMovie(
            movieId.toStdString());

        if (processStatus == Enums::ProcessStatus::SUCCESS)
        {
            qInfo() << "Movie deactivated successfully:" << movieId;
        }
        else
        {
            qWarning() << "Failed to deactivate movie:" << movieId;
        }

        return static_cast<int>(processStatus);
    }
    catch (const std::exception& exception)
    {
        qCritical() << "Movie deactivation failed:" << exception.what();
    }
    catch (...)
    {
        qCritical() << "Unknown exception occurred while deactivating movie.";
    }

    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::reactivateMovie
 * Description: Reactivates a previously deactivated movie in the system.
 * Parameters:
 *    movieId (const QString&) - Unique identifier of the movie
 * Returns:
 *    int - Process status code (Enums::ProcessStatus)
 */
int ControllerAdapter::reactivateMovie(const QString& movieId)
{
    Enums::ProcessStatus processStatus;

    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "Movie reactivation failed: Backend not initialized!";
            return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
        }

        processStatus = m_controller->reactivateMovie(
            movieId.toStdString());

        if (processStatus == Enums::ProcessStatus::SUCCESS)
        {
            qInfo() << "Movie reactivated successfully:" << movieId;
        }
        else
        {
            qWarning() << "Failed to reactivate movie:" << movieId;
        }

        return static_cast<int>(processStatus);
    }
    catch (const std::exception& exception)
    {
        qCritical() << "Movie reactivation failed:" << exception.what();
    }
    catch (...)
    {
        qCritical() << "Unknown exception occurred while reactivating movie.";
    }

    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::getLog
 * Description: Retrieves logs of a specified type from the controller and converts
 *              them into QVariantMap objects for UI consumption. Each log entry
 *              includes timestamp, logId, type, and details.
 * Parameters:
 *    type (const QString&) - Type of log to fetch (mapped via Enums::getLogType)
 * Returns:
 *    QVariantList - List of logs, each represented as a QVariantMap with fields:
 *                   "timeStamp", "logId", "type", and "details"
 */
QVariantList ControllerAdapter::getLog(const QString& type)
{
    QVariantList completeLogs;
    try{
        std::vector<const Log*> inComingLog = m_controller->getLogsByType(Enums::getLogType((type.toStdString())));
        for(const Log* log : inComingLog)
        {
            QVariantMap logMap;
            logMap["timeStamp"]=QString::fromStdString(log->getTimestamp().toString());
            logMap["logId"]=QString::fromStdString(log->getLogId());
            logMap["type"]=QString::fromStdString(Enums::getLogTypeString(log->getLogType()));
            logMap["details"]=QString::fromStdString(log->getDescription());
            completeLogs.append(logMap);
        }
    }
    catch (const std::exception& exception)
    {
        qCritical() << "Log error:" << exception.what();
    }

    return completeLogs;
}

/*
 * Function: ControllerAdapter::bookingToMap
 * Description: Converts a Booking domain model object into a QVariantMap containing
 *              booking, show, movie, and theatre details for QML UI integration.
 * Parameters:
 *    booking (const Booking*) - Pointer to the Booking object to be converted
 * Returns:
 *    QVariantMap - Map containing key-value pairs of booking details for QML,
 *                  or an empty map if the booking pointer is null
 */
QVariantMap ControllerAdapter::bookingToMap(const Booking* booking) const {
    QVariantMap map;
    if (!booking) {
        return map;
    }
    map["bookingId"] = QString::fromStdString(booking->getBookingId());
    map["seatsCount"] = static_cast<int>(booking->getBookedSeats().size());
    map["status"] = QString::fromStdString(Enums::getBookingStatusString(booking->getStatus()));
    const Show* show = booking->getShow();
    if (show) {
        map["dateTime"] = displayTimeAndDate(show->getStartTime());
        const Movie* movie = show->getMovie();
        if (movie) {
            map["movieName"] = QString::fromStdString(movie->getTitle());
        }
        const Screen* screen = show->getScreen();
        if (screen) {
            const Theatre* theatre = screen->getTheatre();
            if (theatre) {
                map["theaterId"] = QString::fromStdString(theatre->getTheatreId());
                map["theaterName"] = QString::fromStdString(theatre->getName());
            }
        }
    }
    return map;
}

/*
 * Function: ControllerAdapter::displayTimeAndDate
 * Description: Converts a Unix timestamp into a formatted date and time string for display in the UI.
 * Parameters:
 *    time (time_t) - The timestamp in seconds since epoch to be formatted
 * Returns:
 *    QString - The formatted date and time string in "yyyy-MM-dd HH:mm:ss" format
 */
QString ControllerAdapter::displayTimeAndDate(time_t time) const
{
    return QDateTime::fromSecsSinceEpoch(static_cast<qint64>(time))
        .toString("yyyy-MM-dd HH:mm:ss");
}

/*
 * Function: ControllerAdapter::loadBookings
 * Description: Fetches all bookings from the backend controller, filters them based on
 *              the authenticated user's role (Customer or Theatre Owner), converts them
 *              to QVariantMap objects, and notifies the QML layer of the updated list.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void ControllerAdapter::loadBookings() {
    try {
        m_bookings.clear();
        if (!m_controller) {
            qWarning() << "loadBookings failed: Controller is null!";
            emit bookingsChanged();
            return;
        }
        const User* authUser = m_controller->getAuthenticatedUser();
        if (!authUser) {
            qWarning() << "loadBookings failed: No authenticated user!";
            emit bookingsChanged();
            return;
        }
        Enums::UserType userType = authUser->getUserType();
        std::vector<const Booking*> allBookings = m_controller->getAllBookings();
        if (userType == Enums::UserType::CUSTOMER) {
            for (const Booking* booking : allBookings) {
                if (!booking) {
                    continue;
                }
                const User* customer = booking->getCustomer();
                if (customer && customer->getUserId() == authUser->getUserId()) {
                    m_bookings.append(bookingToMap(booking));
                }
            }
        }
        else if (userType == Enums::UserType::THEATRE_OWNER) {
            std::vector<const Theatre*> ownerTheatres = m_controller->getCurrentOwnerTheatres();
            std::unordered_set<std::string> ownerTheatreIds;
            for (const Theatre* theatre : ownerTheatres) {
                if (theatre) {
                    ownerTheatreIds.insert(theatre->getTheatreId());
                }
            }
            for (const Booking* booking : allBookings) {
                if (!booking) {
                    continue;
                }
                const Show* show = booking->getShow();
                if (!show) {
                    continue;
                }
                const Screen* screen = show->getScreen();
                if (!screen) {
                    continue;
                }
                const Theatre* theatre = screen->getTheatre();
                if (theatre && ownerTheatreIds.count(theatre->getTheatreId()) > 0) {
                    m_bookings.append(bookingToMap(booking));
                }
            }
        }
    }
    catch (const std::exception &ex) {
        qCritical() << "Exception in loadBookings:" << ex.what();
    }
    catch (...) {
        qCritical() << "Unknown exception in loadBookings!";
    }
    emit bookingsChanged();
}

/*
 * Function: ControllerAdapter::paymentToMap
 * Description: Converts a Payment domain model object into a QVariantMap containing
 *              payment, amount, status, timestamp, and associated booking details for QML UI integration.
 * Parameters:
 *    payment (const Payment*) - Pointer to the Payment object to be converted
 * Returns:
 *    QVariantMap - Map containing key-value pairs of payment details for QML,
 *                  or an empty map if the payment pointer is null
 */
QVariantMap ControllerAdapter::paymentToMap(const Payment* payment) const {
    QVariantMap map;
    if (!payment) {
        return map;
    }
    map["paymentId"] = QString::fromStdString(payment->getPaymentId());
    map["amount"] = QString::number(payment->getAmount(), 'f', 2);
    map["paymentMethod"] = QString::fromStdString(Enums::getPaymentMethodString(payment->getPaymentMethod()));
    QString statusStr = QString::fromStdString(Enums::getPaymentStatusString(payment->getStatus()));
    map["paymentStatus"] = statusStr;
    map["status"] = statusStr;
    map["timeStamp"] = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(payment->getTimeStamp()))
                           .toString("yyyy-MM-dd HH:mm:ss");
    const Booking* booking = payment->getBooking();
    if (booking) {
        map["bookingId"] = QString::fromStdString(booking->getBookingId());
    } else {
        map["bookingId"] = "N/A";
    }
    return map;
}

/*
 * Function: ControllerAdapter::loadPayments
 * Description: Fetches all payment records from the backend controller, filters them based on
 *              the authenticated user's role (Customer or Theatre Owner), converts them
 *              to QVariantMap objects, and notifies the QML layer of the updated list.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void ControllerAdapter::loadPayments() {
    try {
        m_payments.clear();
        if (!m_controller) {
            qWarning() << "loadPayments failed: Controller is null!";
            emit paymentsChanged();
            return;
        }
        const User* authUser = m_controller->getAuthenticatedUser();
        if (!authUser) {
            qWarning() << "loadPayments failed: No authenticated user!";
            emit paymentsChanged();
            return;
        }
        Enums::UserType userType = authUser->getUserType();
        const std::vector<Payment*> allPayments = m_controller->getAllPayments();
        if (userType == Enums::UserType::CUSTOMER) {
            for (const Payment* payment : allPayments) {
                if (!payment) {
                    continue;
                }
                const Booking* booking = payment->getBooking();
                if (!booking) {
                    continue;
                }
                const User* customer = booking->getCustomer();
                if (customer && customer->getUserId() == authUser->getUserId()) {
                    m_payments.append(paymentToMap(payment));
                }
            }
        }
        else if (userType == Enums::UserType::THEATRE_OWNER) {
            std::vector<const Theatre*> ownerTheatres = m_controller->getCurrentOwnerTheatres();
            std::unordered_set<std::string> ownerTheatreIds;
            for (const Theatre* theatre : ownerTheatres) {
                if (theatre) {
                    ownerTheatreIds.insert(theatre->getTheatreId());
                }
            }
            for (const Payment* payment : allPayments) {
                if (!payment) {
                    continue;
                }
                const Booking* booking = payment->getBooking();
                if (!booking) {
                    continue;
                }
                const Show* show = booking->getShow();
                if (!show) {
                    continue;

                }
                const Screen* screen = show->getScreen();
                if (!screen) {
                    continue;
                }
                const Theatre* theatre = screen->getTheatre();
                if (theatre && ownerTheatreIds.count(theatre->getTheatreId()) > 0) {
                    m_payments.append(paymentToMap(payment));
                }
            }
        }
    }
    catch (const std::exception &ex) {
        qCritical() << "Exception in loadPayments:" << ex.what();
    }
    catch (...) {
        qCritical() << "Unknown exception in loadPayments!";
    }
    emit paymentsChanged();
}

/*
 * Function: ControllerAdapter::refundToMap
 * Description: Converts a Refund domain model object into a QVariantMap containing
 *              refund ID, amount, status, timestamp, and associated ticket details for QML UI integration.
 * Parameters:
 *    refund (const Refund*) - Pointer to the Refund object to be converted
 * Returns:
 *    QVariantMap - Map containing key-value pairs of refund details formatted for QML,
 *                  or an empty map if the refund pointer is null
 */
QVariantMap ControllerAdapter::refundToMap(const Refund* refund) const {
    QVariantMap map;
    if (!refund) {
        return map;
    }
    map["refundId"] = QString::fromStdString(refund->getRefundId());
    map["refundAmount"] = QString::number(refund->getRefundAmount(), 'f', 2);
    map["amount"] = map["refundAmount"];
    QString statusStr = QString::fromStdString(Enums::getRefundStatusString(refund->getStatus()));
    map["refundStatus"] = statusStr;
    map["status"] = statusStr;
    QString formattedTime = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(refund->getTime()))
                                .toString("yyyy-MM-dd HH:mm:ss");
    map["refundTime"] = formattedTime;
    map["timeStamp"] = formattedTime;
    const Ticket* ticket = refund->getBookedTicket();
    if (ticket) {
        map["ticketId"] = QString::fromStdString(ticket->getTicketId());
    } else {
        map["ticketId"] = "N/A";
    }
    return map;
}

/*
 * Function: ControllerAdapter::loadRefunds
 * Description: Fetches all refund records from the backend controller, filters them based on
 *              the authenticated user's role (Customer or Theatre Owner), converts them
 *              to QVariantMap objects, and notifies the QML layer of the updated list.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void ControllerAdapter::loadRefunds() {
    try {
        m_refunds.clear();
        if (!m_controller) {
            qWarning() << "loadRefunds failed: Controller is null!";
            emit refundsChanged();
            return;
        }
        const User* authUser = m_controller->getAuthenticatedUser();
        if (!authUser) {
            qWarning() << "loadRefunds failed: No authenticated user!";
            emit refundsChanged();
            return;
        }
        Enums::UserType userType = authUser->getUserType();
        const std::vector<Refund*> allRefunds = m_controller->getRefunds();
        if (userType == Enums::UserType::CUSTOMER) {
            for (const Refund* refund : allRefunds) {
                if (!refund) {
                    continue;
                }
                const Ticket* ticket = refund->getBookedTicket();
                if (!ticket) {
                    continue;
                }
                const Booking* booking = ticket->getPayment()->getBooking();
                if (!booking) {
                    continue;
                }
                const User* customer = booking->getCustomer();
                if (customer && customer->getUserId() == authUser->getUserId()) {
                    m_refunds.append(refundToMap(refund));
                }
            }
        }
        else if (userType == Enums::UserType::THEATRE_OWNER) {
            std::vector<const Theatre*> ownerTheatres = m_controller->getCurrentOwnerTheatres();
            std::unordered_set<std::string> ownerTheatreIds;
            for (const Theatre* theatre : ownerTheatres) {
                if (theatre) {
                    ownerTheatreIds.insert(theatre->getTheatreId());
                }
            }
            for (const Refund* refund : allRefunds) {
                if (!refund) {
                    continue;
                }
                const Ticket* ticket = refund->getBookedTicket();
                if (!ticket) {
                    continue;
                }
                const Booking* booking = ticket->getPayment()->getBooking();
                if (!booking) {
                    continue;
                }
                const Show* show = booking->getShow();
                if (!show) {
                    continue;
                }
                const Screen* screen = show->getScreen();
                if (!screen) {
                    continue;
                }
                const Theatre* theatre = screen->getTheatre();
                if (theatre && ownerTheatreIds.count(theatre->getTheatreId()) > 0) {
                    m_refunds.append(refundToMap(refund));
                }
            }
        }
    }
    catch (const std::exception &ex) {
        qCritical() << "Exception in loadRefunds:" << ex.what();
    }
    catch (...) {
        qCritical() << "Unknown exception in loadRefunds!";
    }
    emit refundsChanged();
}

/*
 * Function: ControllerAdapter::cancelBooking
 * Description: Validates and cancels a customer booking by ID, triggers the backend controller's
 *              cancellation process, reloads related data models (bookings, payments, refunds),
 *              and returns the status result to the QML UI.
 * Parameters:
 *    bookingId (const QString&) - Unique identifier of the booking to be cancelled
 * Returns:
 *    QVariantMap - Map containing boolean "success" status and descriptive "message" string for QML feedback
 */
QVariantMap ControllerAdapter::cancelBooking(const QString& bookingId)
{
    QVariantMap cancellationResponseMap;
    cancellationResponseMap["success"] = false;
    cancellationResponseMap["message"] = "Failed to cancel booking.";
    try
    {
        if (m_controller == nullptr)
        {
            qWarning() << "cancelBooking failed: Controller instance is null!";
            cancellationResponseMap["message"] = "Internal system error: Controller unavailable.";
            return cancellationResponseMap;
        }
        const std::string targetBookingId = bookingId.toStdString();
        if (targetBookingId.empty())
        {
            qWarning() << "cancelBooking failed: Provided bookingId is empty.";
            cancellationResponseMap["message"] = "Invalid Booking ID provided.";
            return cancellationResponseMap;
        }
        const std::vector<const Booking*> cancellableBookingsList = m_controller->getCancellableCustomerBookings();
        bool isBookingEligibleForCancellation = false;
        for (const Booking* currentBooking : cancellableBookingsList)
        {
            if (currentBooking != nullptr)
            {
                if (currentBooking->getBookingId() == targetBookingId)
                {
                    isBookingEligibleForCancellation = true;
                    break;
                }
            }
        }
        if (!isBookingEligibleForCancellation)
        {
            qWarning() << "cancelBooking failed: Booking ID" << bookingId << "is not eligible for cancellation.";
            cancellationResponseMap["message"] = "Booking cannot be cancelled or Booking ID is invalid.";
            return cancellationResponseMap;
        }
        Enums::ProcessStatus cancellationProcessStatus = m_controller->cancelBooking(targetBookingId);
        if (cancellationProcessStatus == Enums::ProcessStatus::SUCCESS)
        {
            cancellationResponseMap["success"] = true;
            cancellationResponseMap["message"] = "Booking cancelled successfully and payment refunded!";
            loadBookings();
            loadPayments();
            loadRefunds();
        }
        else
        {
            cancellationResponseMap["message"] = "Failed to cancel booking! Please try again later.";
        }
    }
    catch (const std::exception& exceptionContext)
    {
        qCritical() << "Standard exception caught in cancelBooking:" << exceptionContext.what();
        cancellationResponseMap["message"] = QString("An error occurred: ") + exceptionContext.what();
    }
    catch (...)
    {
        qCritical() << "Unknown exception caught in cancelBooking!";
        cancellationResponseMap["message"] = "An unknown error occurred while processing cancellation.";
    }
    return cancellationResponseMap;
}

/*
 * Function: ControllerAdapter::getMoviesInTheatre
 * Description: Retrieves all movies assigned to a specific theatre from the controller
 *              and converts them into QVariantMap objects for UI consumption.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 * Returns:
 *    QVariantList - List of movies with their details (id, title, genre)
 */
QVariantList ControllerAdapter::getMoviesInTheatre(const QString& theatreId)
{
    QVariantList list;
    try {
        auto movies = m_controller->getMoviesFromTheatre(theatreId.toStdString());

        for (const Movie* movie : movies) {
            QVariantMap map;
            map["id"] = QString::fromStdString(movie->getMovieId());
            map["title"] = QString::fromStdString(movie->getTitle());
            map["genre"] = QString::fromStdString(movie->getGenre());
            list.append(map);
        }
    } catch(const std::exception &ex) {
        qDebug() << "Error fetching movies for theatre:" << ex.what();
    }
    return list;
}

/*
 * Function: ControllerAdapter::getActiveMovies
 * Description: Retrieves all currently active movies from the controller
 *              and converts them into QVariantMap objects for UI consumption.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of active movies with their details (id, title)
 */
QVariantList ControllerAdapter::getActiveMovies()
{
    QVariantList list;
    try {
        auto activeMovies = m_controller->getAllActiveMovies();
        for (const Movie* movie : activeMovies) {
            QVariantMap map;
            map["id"] = QString::fromStdString(movie->getMovieId());
            map["title"] = QString::fromStdString(movie->getTitle());
            list.append(map);
        }
    } catch(const std::exception &ex) {
        qDebug() << "Error fetching active movies:" << ex.what();
    }
    return list;
}

/*
 * Function: ControllerAdapter::addMovieToTheatre
 * Description: Assigns an existing movie to a specific theatre.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 *    movieId   - QString representing the unique ID of the movie
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::addMovieToTheatre(const QString& theatreId, const QString& movieId)
{
    try {
        Enums::ProcessStatus status = m_controller->addMovieToTheatre(theatreId.toStdString(), movieId.toStdString());
        return static_cast<int>(status);
    } catch(...) {
        return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
    }
}

/*
 * Function: ControllerAdapter::removeMovieFromTheatre
 * Description: Removes an assigned movie from a specific theatre.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 *    movieId   - QString representing the unique ID of the movie
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::removeMovieFromTheatre(const QString& theatreId, const QString& movieId)
{
    try {
        Enums::ProcessStatus status = m_controller->removeMovieFromTheatre(theatreId.toStdString(), movieId.toStdString());
        return static_cast<int>(status);
    } catch(...) {
        return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
    }
}

/*
 * Function: ControllerAdapter::getScreensInTheatre
 * Description: Retrieves all screens belonging to a specific theatre from the controller
 *              and converts them into QVariantMap objects for UI consumption.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 * Returns:
 *    QVariantList - List of screens with details (id, name, rows, cols, capacity, status, price)
 */
QVariantList ControllerAdapter::getScreensInTheatre(const QString& theatreId)
{
    QVariantList list;
    try {
        // Calls Controller::viewTheatreScreens(theatreId)
        const std::vector<const Screen*> screens = m_controller->viewTheatreScreens(theatreId.toStdString());

        for (const Screen* screen : screens) {
            if (!screen)
            {
                continue;
            }
            QVariantMap map;
            map["id"] = QString::fromStdString(screen->getScreenId());
            map["name"] = QString::fromStdString(screen->getName());
            map["rows"] = screen->getTotalRows();
            map["cols"] = screen->getTotalColumns();
            map["capacity"] = screen->getTotalRows() * screen->getTotalColumns();
            map["status"] = static_cast<int>(screen->getScreenStatus());
            double seatPrice = 0.0;
            const auto& grid = screen->getSeatGrid();
            if (!grid.empty() && !grid[0].empty() && grid[0][0] != nullptr)
            {
                seatPrice = grid[0][0]->getSeatAmount();
            }
            map["price"] = seatPrice;
            list.append(map);
        }
    } catch(const std::exception &ex) {
        qDebug() << "Error fetching screens:" << ex.what();
    }
    return list;
}

/*
 * Function: ControllerAdapter::addScreenToTheatre
 * Description: Adds a new screen to a specific theatre with given seat layout and price.
 * Parameters:
 *    theatreId  - QString representing the unique ID of the theatre
 *    screenName - QString name of the new screen
 *    seatRows   - int number of seat rows
 *    seatColumns- int number of seat columns
 *    seatAmount - double price per seat
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::addScreenToTheatre(const QString& theatreId, const QString& screenName, int seatRows, int seatColumns, double seatAmount)
{
    try {
        Enums::ProcessStatus status = m_controller->addScreen(
            theatreId.toStdString(),
            screenName.toStdString(),
            seatRows,
            seatColumns,
            seatAmount
            );
        return static_cast<int>(status);
    } catch(const std::exception &ex) {
        qDebug() << "Add screen error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::updateScreenName
 * Description: Updates the name of an existing screen in a theatre.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 *    screenId  - QString representing the unique ID of the screen
 *    newName   - QString new name for the screen
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, ALREADY_EXISTS, etc.)
 */
int ControllerAdapter::updateScreenName(const QString& theatreId, const QString& screenId, const QString& newName)
{
    try {
        Enums::ProcessStatus status = m_controller->updateScreenName(
            theatreId.toStdString(),
            screenId.toStdString(),
            newName.toStdString()
            );
        return static_cast<int>(status);
    } catch(const std::exception &ex) {
        qDebug() << "Update screen name error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::deactivateScreen
 * Description: Deactivates a screen in a specific theatre, making it unavailable for use.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 *    screenId  - QString representing the unique ID of the screen
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::deactivateScreen(const QString& theatreId, const QString& screenId)
{
    try {
        Enums::ProcessStatus status = m_controller->deactivateScreen(
            theatreId.toStdString(),
            screenId.toStdString()
            );
        return static_cast<int>(status);
    } catch(const std::exception &ex) {
        qDebug() << "Deactivate screen error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::reactivateScreen
 * Description: Reactivates a previously deactivated screen in a specific theatre.
 * Parameters:
 *    theatreId - QString representing the unique ID of the theatre
 *    screenId  - QString representing the unique ID of the screen
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::reactivateScreen(const QString& theatreId, const QString& screenId)
{
    try {
        Enums::ProcessStatus status = m_controller->reactivateScreen(
            theatreId.toStdString(),
            screenId.toStdString()
            );
        return static_cast<int>(status);
    } catch(const std::exception &ex) {
        qDebug() << "Reactivate screen error:" << ex.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::isShowTimeConflicting
 * Description: Checks whether a proposed show time conflicts with existing shows
 *              for the given movie and screen.
 * Parameters:
 *    movieId (const QString&) - Unique identifier of the movie
 *    screenId (const QString&) - Unique identifier of the screen
 *    year (const int) - Year of the show
 *    month (const int) - Month of the show
 *    day (const int) - Day of the show
 *    hour (const int) - Hour of the show
 *    minute (const int) - Minute of the show
 * Returns:
 *    bool - true if a conflict exists, false otherwise
 */
bool ControllerAdapter::isShowTimeConflicting(const QString& movieId,const QString& screenId,const int year,const int month,const int day,const int hour,const int minute)
{
    try{
        return m_controller->isShowTimeConflicting(movieId.toStdString(),screenId.toStdString(),year,month,day,hour,minute)==Enums::ProcessStatus::FAILED;
    }
    catch(const std::exception &ex) {
        qDebug() << "Show Time Conflicting error:" << ex.what();
    }
    return false;
}

/*
 * Function: ControllerAdapter::addShow
 * Description: Adds a new show for the specified movie and screen at the given time.
 * Parameters:
 *    movieId (const QString&) - Unique identifier of the movie
 *    screenId (const QString&) - Unique identifier of the screen
 *    year (const int) - Year of the show
 *    month (const int) - Month of the show
 *    day (const int) - Day of the show
 *    hour (const int) - Hour of the show
 *    minute (const int) - Minute of the show
 * Returns:
 *    bool - true if the show was successfully added, false otherwise
 */
bool ControllerAdapter::addShow(const QString& movieId,const QString& screenId,const int year,const int month,const int day,const int hour,const int minute)
{
    try{
        return m_controller->addShow(movieId.toStdString(),screenId.toStdString(),year,month,day,hour,minute)==Enums::ProcessStatus::SUCCESS;
    }
    catch(const std::exception &ex) {
        qDebug() << "Add show error:" << ex.what();
    }
    return false;
}

/*
 * Function: ControllerAdapter::getActiveShows
 * Description: Retrieves all active shows from the controller and converts them
 *              into QVariantMap objects for UI consumption.
 * Parameters:
 *    None
 * Returns:
 *    QVariantList - List of active shows with details including id, movie, theatre,
 *                   screen, starting time, ending time, and status
 */
QVariantList ControllerAdapter::getActiveShows()
{
    QVariantList list;
    try{
        const std::vector<const Show*> shows = m_controller->getActiveShows();
        for(const Show* show:shows)
        {
            if(!show)
            {
                continue;
            }
            QVariantMap showMap;
            showMap["id"]=QString::fromStdString(show->getShowId());
            showMap["movie"]=QString::fromStdString(show->getMovie()->getTitle());
            showMap["theatre"]=QString::fromStdString(show->getScreen()->getTheatre()->getName());
            showMap["screen"]=QString::fromStdString(show->getScreen()->getName());
            showMap["startingTime"]=displayTimeAndDate(show->getStartTime());
            showMap["endingTime"]=displayTimeAndDate(show->getEndTime());
            showMap["status"]=QString::fromStdString(Enums::getShowStatusString(show->getShowStatus()));
            list.append(showMap);
        }
    }
    catch(const std::exception &ex) {
        qDebug() << "Error fetching Show:" << ex.what();
    }
    return list;
}

/*
 * Function: ControllerAdapter::updateShow
 * Description: Updates the scheduled time of an existing show.
 * Parameters:
 *    year (int) - Updated year
 *    month (int) - Updated month
 *    day (int) - Updated day
 *    hour (int) - Updated hour
 *    minute (int) - Updated minute
 *    showId (const QString&) - Unique identifier of the show
 * Returns:
 *    bool - true if the show was successfully updated, false otherwise
 */
bool ControllerAdapter::updateShow(int year, int month, int day, int hour, int minute,const QString& showId)
{
    try{
        return m_controller->updateShow(createTime(year,month,day,hour,minute),showId.toStdString())==Enums::ProcessStatus::SUCCESS;
    }
    catch(const std::exception &ex) {
        qDebug() << "Update Show error:" << ex.what();
    }
    return false;
}

/*
 * Function: ControllerAdapter::createTime
 * Description: Creates a time_t object from the provided date and time components.
 * Parameters:
 *    year (int) - Year
 *    month (int) - Month
 *    day (int) - Day
 *    hour (int) - Hour
 *    minute (int) - Minute
 * Returns:
 *    time_t - Constructed time object
 */
time_t ControllerAdapter::createTime(int year, int month, int day, int hour, int minute)
{
    tm time = {};
    try{
        tm time = {};
        time.tm_year = year - 1900;
        time.tm_mon = month - 1;
        time.tm_mday = day;
        time.tm_hour = hour;
        time.tm_min = minute;
        time.tm_sec = 0;
    }
    catch(const std::exception &ex) {
        qDebug() << "Create time error:" << ex.what();
    }
    return mktime(&time);
}

/*
 * Function: ControllerAdapter::cancelShow
 * Description: Cancels an existing show by setting its status to CANCELLED.
 * Parameters:
 *    showId (const QString&) - Unique identifier of the show
 * Returns:
 *    bool - true if the show was successfully cancelled, false otherwise
 */
bool ControllerAdapter::cancelShow(const QString& showId)
{
    try{
        return Enums::ProcessStatus::SUCCESS==m_controller->setShowStatusById(showId.toStdString(),Enums::ShowStatus::CANCELLED);
    }
    catch(const std::exception &ex) {
        qDebug() << "Cancel show error:" << ex.what();
    }
    return false;
}

/*
 * name        : loadTickets
 * description : Fetches all tickets from the backend controller, converts them into QVariantMap
 *               objects containing ticket, customer, payment, booking, and status details, and
 *               updates the QML layer with the refreshed list.
 * parameter   : None
 * return type : void
 */
void ControllerAdapter::loadTickets() {
    m_tickets.clear();
    try {
        if (!m_controller) {
            emit ticketsChanged();
            return;
        }
        for (const Ticket* ticket : m_controller->viewAllTickets()) {
            if (!ticket) continue;
            QVariantMap map;
            map["ticketId"] = QString::fromStdString(ticket->getTicketId());
            map["customerName"] = ticket->getCustomer()
                                      ? QString::fromStdString(ticket->getCustomer()->getUserName()) : "N/A";
            if (ticket->getPayment()) {
                map["paymentId"] = QString::fromStdString(ticket->getPayment()->getPaymentId());
                map["amount"] = ticket->getPayment()->getAmount();
                map["bookingId"] = ticket->getPayment()->getBooking()
                                       ? QString::fromStdString(ticket->getPayment()->getBooking()->getBookingId()) : "N/A";
            } else {
                map["paymentId"] = "N/A";
                map["amount"] = 0.0;
                map["bookingId"] = "N/A";
            }
            map["status"] = QString::fromStdString(Enums::getTicketStatusString(ticket->getTicketStatus()));
            m_tickets.append(map);
        }
    } catch (const std::exception &ex) {
        qDebug() << "loadTickets error:" << ex.what();
    }
    emit ticketsChanged();
}

/*
 * Function: ControllerAdapter::ticketToMap
 * Description: Converts a Ticket* into a QVariantMap for QML consumption.
 *              Shared by loadTickets() (admin/all), loadActiveTickets(), and
 *              loadTicketHistory() so the field layout stays identical across
 *              all three ticket views. Always includes customerName, even
 *              for the customer's own tickets — unlike the console UI, which
 *              only prints the customer name column for Admin. Harmless for
 *              a customer viewing their own name, but worth knowing this
 *              differs from the original console behavior.
 * Parameters:
 *    ticket (const Ticket*) - Ticket to convert
 * Returns:
 *    QVariantMap - ticketId, customerName, paymentId, amount, bookingId, status
 */
QVariantMap ControllerAdapter::ticketToMap(const Ticket* ticket) const
{
    QVariantMap map;
    if (!ticket) {
        return map;
    }
    map["ticketId"] = QString::fromStdString(ticket->getTicketId());
    if (ticket->getCustomer()) {
        map["customerName"] = QString::fromStdString(ticket->getCustomer()->getUserName());
    } else {
        map["customerName"] = "N/A";
    }
    map["movieTitle"] = "N/A";
    map["showDateAndTime"] = "N/A";
    if (ticket->getPayment()) {
        map["paymentId"] = QString::fromStdString(ticket->getPayment()->getPaymentId());
        map["amount"] = ticket->getPayment()->getAmount();
        const Booking* booking = ticket->getPayment()->getBooking();
        if (booking) {
            map["bookingId"] = QString::fromStdString(booking->getBookingId());
            const Show* show = booking->getShow();
            if (show) {
                map["showDateAndTime"] = displayTimeAndDate(show->getStartTime());
                const Movie* movie = show->getMovie();
                if (movie) {
                    map["movieTitle"] = QString::fromStdString(movie->getTitle());
                }
            }
        } else {
            map["bookingId"] = "N/A";
        }
    } else {
        map["paymentId"] = "N/A";
        map["amount"] = 0.0;
        map["bookingId"] = "N/A";
    }
    map["status"] = QString::fromStdString(Enums::getTicketStatusString(ticket->getTicketStatus()));
    return map;
}

/*
 * name        : loadActiveTickets
 * description : Fetches active tickets from the backend controller, converts them into QVariantMap
 *               objects using ticketToMap(), and updates the QML layer with the refreshed list.
 *               Handles initialization checks and logs errors if backend is unavailable.
 * parameter   : None
 * return type : void
 */
void ControllerAdapter::loadActiveTickets()
{
    m_activeTickets.clear();
    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "loadActiveTickets failed: Backend not initialized!";
            emit activeTicketsChanged();
            return;
        }
        const std::vector<const Ticket*> tickets = m_controller->viewTicketDetails();
        for (const Ticket* ticket : tickets)
        {
            if (!ticket) {
                continue;
            }
            m_activeTickets.append(ticketToMap(ticket));
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "loadActiveTickets error:" << ex.what();
    }
    emit activeTicketsChanged();
}

/*
 * name        : loadTicketHistory
 * description : Fetches historical ticket records from the backend controller, converts them into
 *               QVariantMap objects using ticketToMap(), and updates the QML layer with the refreshed list.
 *               Performs initialization checks and logs errors if backend is unavailable.
 * parameter   : None
 * return type : void
 */
void ControllerAdapter::loadTicketHistory()
{
    m_ticketHistory.clear();
    try
    {
        if (!m_initialized || !m_controller)
        {
            qWarning() << "loadTicketHistory failed: Backend not initialized!";
            emit ticketHistoryChanged();
            return;
        }

        const std::vector<const Ticket*> tickets = m_controller->viewTicketHistory();
        for (const Ticket* ticket : tickets)
        {
            if (!ticket) {
                continue;
            }
            m_ticketHistory.append(ticketToMap(ticket));
        }
    }
    catch (const std::exception &ex)
    {
        qDebug() << "loadTicketHistory error:" << ex.what();
    }
    emit ticketHistoryChanged();
}

/*
 * Function: ControllerAdapter::getSeatLayout
 * Description: Retrieves the seat layout grid for a given screen from the controller
 *              and converts it into a QVariantList of rows containing seat details.
 * Parameters:
 *    selectedScreenId - QString representing the unique ID of the screen
 * Returns:
 *    QVariantList - Nested list of seat rows, each containing seat maps (id, row, column, amount, status)
 */
QVariantList ControllerAdapter::getSeatLayout(const QString& selectedScreenId)
{
    QVariantList gridList;
    try {
        const std::vector<std::vector<Seat*>>& seatGrid = m_controller->getSeatLayout(selectedScreenId.toStdString());
        for (const auto& rowVector : seatGrid) {
            QVariantList rowList;
            for (const Seat* seatPointer : rowVector) {
                if (seatPointer != nullptr) {
                    QVariantMap seatMap;
                    seatMap["id"] = QString::fromStdString(seatPointer->getSeatId());
                    seatMap["row"] = QString(seatPointer->getSeatRow());
                    seatMap["column"] = seatPointer->getSeatColumn();
                    seatMap["amount"] = const_cast<Seat*>(seatPointer)->getSeatAmount();
                    seatMap["status"] = static_cast<int>(seatPointer->getSeatStatus());
                    qDebug() << seatPointer->getSeatId();
                    rowList.append(seatMap);
                }
            }
            gridList.append(rowList);
        }
    } catch (const std::exception& exception) {
        qDebug() << "Error retrieving seat layout:" << exception.what();
    }
    return gridList;
}

/*
 * Function: ControllerAdapter::deactivateSeat
 * Description: Deactivates a specific seat in a given screen, making it unavailable for booking.
 * Parameters:
 *    selectedScreenId - QString representing the unique ID of the screen
 *    seatId           - QString representing the unique ID of the seat
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::deactivateSeat(const QString& selectedScreenId, const QString& seatId)
{
    try {
        Enums::ProcessStatus processStatus = m_controller->deactivateSeat(selectedScreenId.toStdString(), seatId.toStdString());
        return static_cast<int>(processStatus);
    } catch (const std::exception& exception) {
        qDebug() << "Error deactivating seat:" << exception.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}

/*
 * Function: ControllerAdapter::reactivateSeat
 * Description: Reactivates a previously deactivated seat in a given screen, making it available again.
 * Parameters:
 *    selectedScreenId - QString representing the unique ID of the screen
 *    seatId           - QString representing the unique ID of the seat
 * Returns:
 *    int - ProcessStatus code (SUCCESS, FAILED, etc.)
 */
int ControllerAdapter::reactivateSeat(const QString& selectedScreenId, const QString& seatId)
{
    try {
        Enums::ProcessStatus processStatus = m_controller->reactivateSeat(selectedScreenId.toStdString(), seatId.toStdString());
        return static_cast<int>(processStatus);
    } catch (const std::exception& exception) {
        qDebug() << "Error reactivating seat:" << exception.what();
    }
    return static_cast<int>(EnumsAdapter::ProcessStatus::FAILED);
}