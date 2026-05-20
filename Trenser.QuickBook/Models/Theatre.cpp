#include "Theatre.h"
#include "Screen.h"

Theatre::Theatre()
    : m_theatreId(""), m_name(""), m_city(""), m_address(""),
    m_contactPhone(""), m_contactEmail(""),
    m_theatreOwner(nullptr), m_status(0),
    m_screens(), m_movies() {}

Theatre::Theatre(const std::string& id,
    const std::string& name,
    const std::string& city,
    const std::string& address,
    const std::string& contactPhone,
    const std::string& contactEmail,
    User* theatreOwner,
    int status,
    const std::vector<Screen*>& screens,
    const std::vector<Movie*>& movies)
    : m_theatreId(id), m_name(name), m_city(city), m_address(address),
    m_contactPhone(contactPhone), m_contactEmail(contactEmail),
    m_theatreOwner(theatreOwner), m_status(status),
    m_screens(screens), m_movies(movies) {}

const std::string& Theatre::getTheatreId() const
{
    return m_theatreId;
}

const std::string& Theatre::getName() const
{
    return m_name;
}

const std::string& Theatre::getCity() const
{
    return m_city;
}

const std::string& Theatre::getAddress() const
{
    return m_address;
}

const std::string& Theatre::getContactPhone() const
{
    return m_contactPhone;
}

const std::string& Theatre::getContactEmail() const
{
    return m_contactEmail;
}

User* Theatre::getTheatreOwner() const
{
    return m_theatreOwner;
}

int Theatre::getStatus() const
{
    return m_status;
}

const std::vector<Screen*>& Theatre::getScreens() const
{
    return m_screens;
}

const std::vector<Movie*>& Theatre::getMovies() const
{
    return m_movies;
}

void Theatre::setTheatreId(const std::string& id)
{
    m_theatreId = id;
}

void Theatre::setName(const std::string& name)
{
    m_name = name;
}

void Theatre::setCity(const std::string& city)
{
    m_city = city;
}

void Theatre::setAddress(const std::string& address)
{
    m_address = address;
}

void Theatre::setContactPhone(const std::string& contactPhone)
{
    m_contactPhone = contactPhone;
}

void Theatre::setContactEmail(const std::string& contactEmail)
{
    m_contactEmail = contactEmail;
}

void Theatre::setTheatreOwner(User* theatreOwner)
{
    m_theatreOwner = theatreOwner;
}

void Theatre::setStatus(int status)
{
    m_status = status;
}

void Theatre::setScreens(const std::vector<Screen*>& screens)
{
    m_screens = screens;
}

void Theatre::setMovies(const std::vector<Movie*>& movies)
{
    m_movies = movies;
}
