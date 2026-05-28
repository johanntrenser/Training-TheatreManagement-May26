/*
 * File: Theatre.cpp
 * Description: Implements the Theatre class, which represents a theatre entity.
 *              Stores details such as theatre ID, name, city, address, contact
 *              information, owner, status, screens, and movies.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#include "Theatre.h"
#include "Screen.h"

 /*
  * Function: Theatre::Theatre
  * Description: Default constructor initializing theatre with default values.
  * Returns:
  *    Theatre object
  */
Theatre::Theatre()
    : m_theatreId(""), m_name(""), m_city(""), m_address(""),
    m_phoneNumber(""), m_email(""),
    m_theatreOwner(nullptr), m_status(Enums::TheatreStatus::PENDING),
    m_screens(), m_movies() {}

/*
 * Function: Theatre::Theatre
 * Description: Parameterized constructor initializing theatre details.
 * Parameters:
 *    const std::string& id - Unique theatre identifier
 *    const std::string& name - Theatre name
 *    const std::string& city - Theatre city
 *    const std::string& address - Theatre address
 *    const std::string& contactPhone - Theatre contact phone
 *    const std::string& contactEmail - Theatre contact email
 *    User* theatreOwner - Theatre owner object
 *    int status - Theatre status code
 *    const std::vector<Screen*>& screens - Screens in the theatre
 *    const std::vector<Movie*>& movies - Movies associated with the theatre
 * Returns:
 *    Theatre object
 */
Theatre::Theatre(const std::string& id,
    const std::string& name,
    const std::string& city,
    const std::string& address,
    const std::string& contactPhone,
    const std::string& contactEmail,
    User* theatreOwner)
    : m_theatreId(id), m_name(name), m_city(city), m_address(address),
    m_phoneNumber(contactPhone), m_email(contactEmail),
    m_theatreOwner(theatreOwner), m_status(Enums::TheatreStatus::PENDING),
    m_screens({}), m_movies({}) {
}

/*
 * Function: Theatre::getTheatreId
 * Description: Retrieves the unique theatre ID.
 * Returns:
 *    const std::string& - Theatre ID
 */
const std::string& Theatre::getTheatreId() const
{
    return m_theatreId;
}

/*
 * Function: Theatre::getName
 * Description: Retrieves the theatre name.
 * Returns:
 *    const std::string& - Theatre name
 */
const std::string& Theatre::getName() const
{
    return m_name;
}

/*
 * Function: Theatre::getCity
 * Description: Retrieves the theatre city.
 * Returns:
 *    const std::string& - Theatre city
 */
const std::string& Theatre::getCity() const
{
    return m_city;
}

/*
 * Function: Theatre::getAddress
 * Description: Retrieves the theatre address.
 * Returns:
 *    const std::string& - Theatre address
 */
const std::string& Theatre::getAddress() const
{
    return m_address;
}

/*
 * Function: Theatre::getTheatrePhoneNumber
 * Description: Retrieves the theatre contact phone.
 * Returns:
 *    const std::string& - Contact phone
 */
const std::string& Theatre::getTheatrePhoneNumber() const
{
    return m_phoneNumber;
}

/*
 * Function: Theatre::getTheatreEmail
 * Description: Retrieves the theatre contact email.
 * Returns:
 *    const std::string& - Contact email
 */
const std::string& Theatre::getTheatreEmail() const
{
    return m_email;
}

/*
 * Function: Theatre::getTheatreOwner
 * Description: Retrieves the theatre owner object.
 * Returns:
 *    User* - Pointer to the theatre owner
 */
User* Theatre::getTheatreOwner() const
{
    return m_theatreOwner;
}

/*
 * Function: Theatre::getStatus
 * Description: Retrieves the theatre status code.
 * Returns:
 *    int - Theatre status
 */
Enums::TheatreStatus Theatre::getStatus() const
{
    return m_status;
}

/*
 * Function: Theatre::getScreens
 * Description: Retrieves the screens in the theatre.
 * Returns:
 *    const std::vector<Screen*>& - Screens
 */
const std::vector<Screen*>& Theatre::getScreens() const
{
    return m_screens;
}

/*
 * Function: Theatre::getMovies
 * Description: Retrieves the movies associated with the theatre.
 * Returns:
 *    const std::vector<Movie*>& - Movies
 */
const std::vector<Movie*>& Theatre::getMovies() const
{
    return m_movies;
}

/*
 * Function: Theatre::setTheatreId
 * Description: Sets the unique theatre ID.
 * Parameters:
 *    const std::string& id - New theatre ID
 * Returns:
 *    void
 */
void Theatre::setTheatreId(const std::string& id)
{
    m_theatreId = id;
}

/*
 * Function: Theatre::setName
 * Description: Sets the theatre name.
 * Parameters:
 *    const std::string& name - New theatre name
 * Returns:
 *    void
 */
void Theatre::setName(const std::string& name)
{
    m_name = name;
}

/*
* Function Name : setScreen
* Description   : Adds a screen to the theatre.
* Parameters    :
*                  screen - Screen to be added
* Return Type   : void
*/
void Theatre::setScreen(Screen* screen)
{
    m_screens.push_back(screen);
}

/*
 * Function: Theatre::setCity
 * Description: Sets the theatre city.
 * Parameters:
 *    const std::string& city - New theatre city
 * Returns:
 *    void
 */
void Theatre::setCity(const std::string& city)
{
    m_city = city;
}

/*
 * Function: Theatre::setAddress
 * Description: Sets the theatre address.
 * Parameters:
 *    const std::string& address - New theatre address
 * Returns:
 *    void
 */
void Theatre::setAddress(const std::string& address)
{
    m_address = address;
}

/*
 * Function: Theatre::setTheatrePhoneNumber
 * Description: Sets the theatre contact phone.
 * Parameters:
 *    const std::string& contactPhone - New contact phone
 * Returns:
 *    void
 */
void Theatre::setTheatrePhoneNumber(const std::string& contactPhone)
{
    m_phoneNumber = contactPhone;
}

/*
 * Function: Theatre::setTheatreEmail
 * Description: Sets the theatre contact email.
 * Parameters:
 *    const std::string& contactEmail - New contact email
 * Returns:
 *    void
 */
void Theatre::setTheatreEmail(const std::string& contactEmail)
{
    m_email = contactEmail;
}

/*
 * Function: Theatre::setTheatreOwner
 * Description: Sets the theatre owner object.
 * Parameters:
 *    User* theatreOwner - Pointer to the theatre owner
 * Returns:
 *    void
 */
void Theatre::setTheatreOwner(User* theatreOwner)
{
    m_theatreOwner = theatreOwner;
}

/*
 * Function: Theatre::setStatus
 * Description: Sets the theatre status code.
 * Parameters:
 *    int status - New theatre status
 * Returns:
 *    void
 */
void Theatre::setStatus(Enums::TheatreStatus status)
{
    m_status = status;
}

/*
 * Function: Theatre::setScreens
 * Description: Sets the screens in the theatre.
 * Parameters:
 *    const std::vector<Screen*>& screens - New screens
 * Returns:
 *    void
 */
void Theatre::setScreens(const std::vector<Screen*>& screens)
{
    m_screens = screens;
}

/*
 * Function: Theatre::setMovies
 * Description: Sets the movies associated with the theatre.
 * Parameters:
 *    const std::vector<Movie*>& movies - New movies
 * Returns:
 *    void
 */
void Theatre::setMovies(const std::vector<Movie*>& movies)
{
    m_movies = movies;
}

/*
* Function Name : getScreensForUpdation
* Description   : Returns the list of screens for modification.
* Parameters    : None
* Return Type   : std::vector<Screen*>&
*/
std::vector<Screen*>& Theatre::getScreensForUpdation()
{
    return m_screens;
}

void Theatre::addMovieToTheatre(Movie* movie)
{
    m_movies.push_back(movie);
}
