/*
 * File: Show.cpp
 * Description: Implements the Show class, which represents a movie show
 *              scheduled in a theatre screen. Stores details such as show ID,
 *              associated movie, screen, start/end times, seat availability,
 *              and status.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#include "Show.h"
#include "ShowSeatAvailability.h"

 /*
  * Function: Show::Show
  * Description: Default constructor initializing show with default values.
  * Returns:
  *    Show object
  */
Show::Show()
    : m_showId(""),
    m_movie(nullptr),
    m_screen(nullptr),
    m_startTime(0),
    m_endTime(0),
    m_seatAvailability(nullptr),
    m_showStatus(0)
{}

/*
 * Function: Show::Show
 * Description: Parameterized constructor initializing show details.
 * Parameters:
 *    const std::string showId - Unique show identifier
 *    Movie* movie - Associated movie object
 *    Screen* screen - Associated screen object
 *    time_t startTime - Show start time
 *    time_t endTime - Show end time
 *    ShowSeatAvailability* seatAvailability - Seat availability object
 *    int showStatus - Show status code
 * Returns:
 *    Show object
 */
Show::Show(const std::string showId,
    Movie* movie,
    Screen* screen,
    time_t startTime,
    time_t endTime,
    ShowSeatAvailability* seatAvailability,
    int showStatus)
    : m_showId(showId),
    m_movie(movie),
    m_screen(screen),
    m_startTime(startTime),
    m_endTime(endTime),
    m_seatAvailability(seatAvailability),
    m_showStatus(showStatus)
{}

/*
 * Function: Show::getShowId
 * Description: Retrieves the unique show ID.
 * Returns:
 *    const std::string - Show ID
 */
const std::string Show::getShowId() const
{
    return m_showId;
}

/*
 * Function: Show::getMovie
 * Description: Retrieves the associated movie object.
 * Returns:
 *    Movie* - Pointer to the movie
 */
Movie* Show::getMovie() const
{
    return m_movie;
}

/*
 * Function: Show::getScreen
 * Description: Retrieves the associated screen object.
 * Returns:
 *    Screen* - Pointer to the screen
 */
Screen* Show::getScreen() const
{
    return m_screen;
}

/*
 * Function: Show::getStartTime
 * Description: Retrieves the show start time.
 * Returns:
 *    time_t - Start time
 */
time_t Show::getStartTime() const
{
    return m_startTime;
}

/*
 * Function: Show::getEndTime
 * Description: Retrieves the show end time.
 * Returns:
 *    time_t - End time
 */
time_t Show::getEndTime() const
{
    return m_endTime;
}

/*
 * Function: Show::getSeatAvailability
 * Description: Retrieves the seat availability object.
 * Returns:
 *    ShowSeatAvailability* - Pointer to seat availability
 */
ShowSeatAvailability* Show::getSeatAvailability() const
{
    return m_seatAvailability;
}

/*
 * Function: Show::getShowStatus
 * Description: Retrieves the show status code.
 * Returns:
 *    int - Show status
 */
int Show::getShowStatus() const
{
    return m_showStatus;
}

/*
 * Function: Show::setShowId
 * Description: Sets the unique show ID.
 * Parameters:
 *    const std::string& showId - New show ID
 * Returns:
 *    void
 */
void Show::setShowId(const std::string& showId)
{
    m_showId = showId;
}

/*
 * Function: Show::setMovie
 * Description: Sets the associated movie object.
 * Parameters:
 *    Movie* movie - Pointer to the movie
 * Returns:
 *    void
 */
void Show::setMovie(Movie* movie)
{
    m_movie = movie;
}

/*
 * Function: Show::setScreen
 * Description: Sets the associated screen object.
 * Parameters:
 *    Screen* screen - Pointer to the screen
 * Returns:
 *    void
 */
void Show::setScreen(Screen* screen)
{
    m_screen = screen;
}

/*
 * Function: Show::setStartTime
 * Description: Sets the show start time.
 * Parameters:
 *    time_t startTime - New start time
 * Returns:
 *    void
 */
void Show::setStartTime(time_t startTime)
{
    m_startTime = startTime;
}

/*
 * Function: Show::setEndTime
 * Description: Sets the show end time.
 * Parameters:
 *    time_t endTime - New end time
 * Returns:
 *    void
 */
void Show::setEndTime(time_t endTime)
{
    m_endTime = endTime;
}

/*
 * Function: Show::setSeatAvailability
 * Description: Sets the seat availability object.
 * Parameters:
 *    ShowSeatAvailability* seatAvailability - Pointer to seat availability
 * Returns:
 *    void
 */
void Show::setSeatAvailability(ShowSeatAvailability* seatAvailability)
{
    m_seatAvailability = seatAvailability;
}

/*
 * Function: Show::setShowStatus
 * Description: Sets the show status code.
 * Parameters:
 *    int showStatus - New show status
 * Returns:
 *    void
 */
void Show::setShowStatus(int showStatus)
{
    m_showStatus = showStatus;
}
