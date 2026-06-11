/*
 * File: User.cpp
 * Description: Implements the User class, providing constructors, getters,
 *              and setters for user attributes such as ID, name, email,
 *              password, phone number, type, and status.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <sstream>
#include "ApplicationConfig.h"
#include "Enums.h"
#include "Factory.h"

class User
{
private:
    std::string m_userId;
    std::string m_userName;
    std::string m_email;
    std::string m_password;
    std::string m_phoneNumber;
    Enums::UserType m_userType; 
    Enums::UserStatus m_status;  
public:
    User();
    User(const std::string& userId,
        const std::string& userName,
        const std::string& email,
        const std::string& password,
        const std::string& phoneNumber,
        Enums::UserType type);
    const std::string& getUserId() const;
    const std::string& getUserName() const;
    const std::string& getEmail() const;
    const std::string& getPassword() const;
    const std::string& getPhoneNumber() const;
    Enums::UserType getUserType() const;
    Enums::UserStatus getStatus() const;
    void setUserId(const std::string& userId);
    void setUserName(const std::string& userName);
    void setEmail(const std::string& email);
    void setPassword(const std::string& password);
    void setPhoneNumber(const std::string& phoneNumber);
    void setUserType(Enums::UserType type);
    void setStatus(Enums::UserStatus status);
    static void reverseString(std::string& password);
    std::string& encryption(std::string& password);
    static std::string decryption(std::string& password);
    std::string serialize();
    static User* deserialize(const std::string& line);
};