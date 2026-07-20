/*
 * File: Notification.h
 * Description: Implements the Notification class, providing constructors,
 *              getters, and setters for notification attributes such as
 *              notification ID, receiver, message, status, and time.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <sstream>
#include "ApplicationConfig.h"
#include "TimeStamp.h"
#include "User.h"
#include "Factory.h"

class Notification
{
private:
    std::string m_notificationId;
    User* m_receiver;
    std::string m_message;
    Enums::NotificationStatus m_status;
    time_t m_time;
public:
    Notification();
    Notification(const std::string& notificationId,
        User* receiver,
        const std::string& message,
        time_t time);
    const std::string& getNotificationId() const;
    User* getReceiver() const;
    const std::string& getMessage() const;
    Enums::NotificationStatus getStatus() const;
    const time_t getTime() const;
    void setNotificationId(const std::string& id);
    void setReceiver(User* receiver);
    void setMessage(const std::string& message);
    void setStatus(Enums::NotificationStatus status);
    void setTime(const time_t time);
    SharedNotification serialize();
    static Notification* deserialize(const SharedNotification* sharedNotification);
};