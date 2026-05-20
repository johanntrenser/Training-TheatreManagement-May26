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
    int m_showStatus; // enum placeholder
public:
    Show();
    Show(const std::string showId,
        Movie* movie,
        Screen* screen,
        time_t startTime,
        time_t endTime,
        ShowSeatAvailability* seatAvailability,
        int showStatus);
    const std::string getShowId() const;
    Movie* getMovie() const;
    Screen* getScreen() const;
    time_t getStartTime() const;
    time_t getEndTime() const;
    ShowSeatAvailability* getSeatAvailability() const;
    int getShowStatus() const;
    void setShowId(const std::string& showId);
    void setMovie(Movie* movie);
    void setScreen(Screen* screen);
    void setStartTime(time_t startTime);
    void setEndTime(time_t endTime);
    void setSeatAvailability(ShowSeatAvailability* seatAvailability);
    void setShowStatus(int showStatus);
};



