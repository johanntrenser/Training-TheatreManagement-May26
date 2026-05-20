#include "Show.h"
#include "ShowSeatAvailability.h"

Show::Show()
    : m_showId(""),
    m_movie(nullptr),
    m_screen(nullptr),
    m_startTime(0),
    m_endTime(0),
    m_seatAvailability(nullptr),
    m_showStatus(0)
{}

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

const std::string Show::getShowId() const
{
    return m_showId;
}

Movie* Show::getMovie() const
{
    return m_movie;
}

Screen* Show::getScreen() const
{
    return m_screen;
}

time_t Show::getStartTime() const
{
    return m_startTime;
}

time_t Show::getEndTime() const
{
    return m_endTime;
}

ShowSeatAvailability* Show::getSeatAvailability() const
{
    return m_seatAvailability;
}

int Show::getShowStatus() const
{
    return m_showStatus;
}

void Show::setShowId(const std::string& showId)
{
    m_showId = showId;
}

void Show::setMovie(Movie* movie)
{
    m_movie = movie;
}

void Show::setScreen(Screen* screen)
{
    m_screen = screen;
}

void Show::setStartTime(time_t startTime)
{
    m_startTime = startTime;
}

void Show::setEndTime(time_t endTime)
{
    m_endTime = endTime;
}

void Show::setSeatAvailability(ShowSeatAvailability* seatAvailability)
{
    m_seatAvailability = seatAvailability;
}

void Show::setShowStatus(int showStatus)
{
    m_showStatus = showStatus;
}
