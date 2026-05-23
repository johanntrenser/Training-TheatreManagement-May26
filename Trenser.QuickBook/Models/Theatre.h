/*
 * File: Theatre.h
 * Description: Defines the Theatre class, which represents a cinema theatre.
 *              Stores details such as theatre ID, name, city, address,
 *              contact information, owner, status, and associated screens
 *              and movies.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "Movie.h"

class Screen;

class Theatre
{
private:
    std::string m_theatreId;
    std::string m_name;
    std::string m_city;
    std::string m_address;
    std::string m_contactPhone;
    std::string m_contactEmail;
    User* m_theatreOwner;
    Enums::TheatreStatus m_status; // enum can be defined separately
    std::vector<Screen*> m_screens;
    std::vector<Movie*> m_movies;
public:
    Theatre();
    Theatre(const std::string& theatreId,
        const std::string& name,
        const std::string& city,
        const std::string& address,
        const std::string& contactPhone,
        const std::string& contactEmail,
        User* theatreOwner,
        const std::vector<Screen*>& screens,
        const std::vector<Movie*>& movies);
    const std::string& getTheatreId() const;
    const std::string& getName() const;
    const std::string& getCity() const;
    const std::string& getAddress() const;
    const std::string& getContactPhone() const;
    const std::string& getContactEmail() const;
    User* getTheatreOwner() const;
    Enums::TheatreStatus getStatus() const;
    const std::vector<Screen*>& getScreens() const;
    const std::vector<Movie*>& getMovies() const;
    void setTheatreId(const std::string& id);
    void setName(const std::string& name);
    void setCity(const std::string& city);
    void setAddress(const std::string& address);
    void setContactPhone(const std::string& contactPhone);
    void setContactEmail(const std::string& contactEmail);
    void setTheatreOwner(User* theatreOwner);
    void setStatus(Enums::TheatreStatus status);
    void setScreens(const std::vector<Screen*>& screens);
    void setMovies(const std::vector<Movie*>& movies);
    void setScreen(Screen* screen);
    std::vector<Screen*>& getScreensForUpdation();
};