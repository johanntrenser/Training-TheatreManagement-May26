/*
 * File: Show.h
 * Description: Defines the Show class, which represents a scheduled movie show
 *              in a theatre screen. Stores details such as show ID, associated
 *              movie, screen, start/end times, seat availability, and status.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#pragma once
#include <string>
#include <ctime>
#include "Movie.h"
#include "Screen.h"

class ShowSeatAvailability;

class Show
{
private:
    std::string m_showId;
    Movie* m_movie;
    Screen* m_screen;
    time_t m_startTime;
    time_t m_endTime;
    ShowSeatAvailability* m_seatAvailability;
    Enums::ShowStatus m_showStatus; // enum placeholder
public:
    Show();
    Show(const std::string showId,
        Movie* movie,
        Screen* screen,
        time_t startTime,
        time_t endTime,
        ShowSeatAvailability* seatAvailability);
    const std::string getShowId() const;
    Movie* getMovie() const;
    Screen* getScreen() const;
    time_t getStartTime() const;
    time_t getEndTime() const;
    ShowSeatAvailability* getSeatAvailability() const;
    Enums::ShowStatus getShowStatus() const;
    void setShowId(const std::string& showId);
    void setMovie(Movie* movie);
    void setScreen(Screen* screen);
    void setStartTime(time_t startTime);
    void setEndTime(time_t endTime);
    void setSeatAvailability(ShowSeatAvailability* seatAvailability);
    void setShowStatus(Enums::ShowStatus showStatus);
};