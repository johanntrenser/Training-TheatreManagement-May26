#include <sstream>
#include <iomanip>
#include "ShowManagementService.h"
#include "TheatreManagementService.h"
#include "TimeStamp.h"
#include "Factory.h"

/*
 * Function: ShowManagementService::generateShowId
 * Description: Generates a unique identifier for a new show based on
 *              the current number of shows in the system.
 * Parameters: None
 * Returns:
 *    std::string - Generated unique show ID
 */
const std::string ShowManagementService::generateShowId()
{
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    int idNumber = static_cast<int>(shows.size()) + 1;
    std::ostringstream buffer;
    buffer << "SHOW" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: ShowManagementService::generateShowSeatAvailabilityId
 * Description: Generates a unique identifier for a new ShowSeatAvailability object.
 * Returns:
 *    const std::string - Generated ShowSeatAvailability ID (e.g., "SSA001")
 */
const std::string ShowManagementService::generateShowSeatAvailabilityId()
{
    const std::map<std::string, ShowSeatAvailability*>& showSeatAvailabilitys = m_dataStore.getShowSeatAvailabilitys();
    int idNumber = static_cast<int>(showSeatAvailabilitys.size()) + 1;
    std::ostringstream buffer;
    buffer << "SSA" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: ShowManagementService::isMovieInTheatre
 * Description: Checks whether a given movie exists in the specified theatre.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the movie exists in the theatre,
 *                           FAILED otherwise
 */
Enums::ProcessStatus ShowManagementService::isMovieInTheatre(const std::string& movieId, const std::string& theatreId)
{
    TheatreManagementService theatreManagementService;
    const std::vector<const Movie*> movies =  theatreManagementService.getMoviesFromTheatre(theatreId);
    for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
    {
        if ((*iterator)->getMovieId() == movieId)
        {
            return Enums::ProcessStatus::SUCCESS;
        }
    }
    return Enums::ProcessStatus::FAILED;
}

/*
 * Function: ShowManagementService::getScreensFromTheatre
 * Description: Retrieves all screens available in the specified theatre.
 * Parameters:
 *    theatreId (const std::string&) - Unique identifier of the theatre
 * Returns:
 *    std::vector<const Screen*> - List of screens in the theatre
 */
const std::vector<const Screen*> ShowManagementService::getScreensFromTheatre(const std::string& theatreId)
{
    std::vector<const Screen*> resultantScreens;
    Theatre* theatre = m_dataStore.getTheatreById(theatreId);
    if (theatre == nullptr)
    {
        return {};
    }
    const std::vector<Screen*>& screens = theatre->getScreens();
    for (std::vector<Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        resultantScreens.push_back((*iterator));
    }
    return resultantScreens;
}

/*
 * Function: ShowManagementService::isShowTimeConflicting
 * Description: Checks whether a proposed show timing overlaps with any
 *              existing show on the same screen.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 *    screenId (const std::string&) - Unique identifier of the screen
 *    year (int) - Year of the show
 *    month (int) - Month of the show
 *    day (int) - Day of the show
 *    startTimeHour (int) - Hour component of show start time
 *    startTimeMinute (int) - Minute component of show start time
 * Returns:
 *    Enums::ProcessStatus - FAILED if a conflict exists,
 *                           SUCCESS otherwise
 */
Enums::ProcessStatus ShowManagementService::isShowTimeConflicting(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute)
{
    Movie* movie = m_dataStore.getMovieById(movieId);
    if (movie == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    int duration = movie->getDuration();
    time_t startTime = util::createTime(year, month, day, startTimeHour, startTimeMinute);
    int buffer = 10;
    time_t endTime = startTime + (duration + buffer) * 60;
    if (startTime >= endTime)
    {
        return Enums::ProcessStatus::FAILED;
    }
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    for (std::map<std::string, Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
    {
        if (iterator->second->getScreen() && ((iterator->second->getScreen()->getScreenId()) == screenId))
        {
            time_t existingStart = iterator->second->getStartTime();
            time_t existingEnd = iterator->second->getEndTime();
            if (startTime < existingEnd && endTime > existingStart)
            {
                return Enums::ProcessStatus::FAILED;
            }
        }
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: ShowManagementService::addShow
 * Description: Creates and stores a new show after validating inputs,
 *              ensuring no time conflicts, and computing end time based
 *              on movie duration.
 * Parameters:
 *    movieId (const std::string&) - Unique identifier of the movie
 *    screenId (const std::string&) - Unique identifier of the screen
 *    year (int) - Year of the show
 *    month (int) - Month of the show
 *    day (int) - Day of the show
 *    startTimeHour (int) - Hour component of show start time
 *    startTimeMinute (int) - Minute component of show start time
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the show is added successfully,
 *                           FAILED otherwise
 */
Enums::ProcessStatus ShowManagementService::addShow(const std::string& movieId, const std::string& screenId, int year, int month, int day, int startTimeHour, int startTimeMinute)
{
    std::string showId = generateShowId();
    Movie* movie = m_dataStore.getMovieById(movieId);
    Screen* screen = m_dataStore.getScreenById(screenId);
    if ((movie == nullptr) || (screen == nullptr))
    {
        return Enums::ProcessStatus::FAILED;
    }
    time_t startTime = util::createTime(year, month, day, startTimeHour, startTimeMinute);
    if (startTime == -1)
    {
        return Enums::ProcessStatus::FAILED;
    }
    time_t now = time(0);
    if (difftime(startTime, now) <= 0)
    {
        return Enums::ProcessStatus::FAILED;
    }
    int buffer = 10;
    time_t endTime = startTime + (movie->getDuration() + buffer) * 60;
    Show* show = Factory::getObject<Show>(showId, movie, screen, startTime, endTime, new ShowSeatAvailability());
    const std::vector<std::vector<Seat*>>& seats = screen->getSeatGrid();
    std::map<std::string, Enums::BookingStatus> seatAvailabilityMap;
    for (std::vector<std::vector<Seat*>>::const_iterator row = seats.begin(); row != seats.end(); ++row)
    {
        std::vector<Seat*> rows = *(row);
        for (std::vector<Seat*>::iterator seat = rows.begin(); seat != rows.end(); ++seat)
        {
            seatAvailabilityMap[(*seat)->getSeatId()] = Enums::BookingStatus::PENDING;
        }
    }
    ShowSeatAvailability* showSeatAvailability = Factory::getObject<ShowSeatAvailability>(generateShowSeatAvailabilityId(), show, seatAvailabilityMap);
    if (showSeatAvailability != nullptr)
    {
        m_dataStore.addShowSeatAvailability(showSeatAvailability);
    }
    else
    {
        return Enums::ProcessStatus::FAILED;
    }
    show->setSeatAvailability(showSeatAvailability);
    if (show != nullptr)
    {
        m_dataStore.addShow(show);
        return Enums::ProcessStatus::SUCCESS;
    }
    return Enums::ProcessStatus::FAILED;
}
