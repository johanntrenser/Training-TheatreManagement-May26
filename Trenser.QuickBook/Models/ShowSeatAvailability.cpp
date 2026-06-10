/*
 * File: ShowSeatAvailability.cpp
 * Description: Implements the ShowSeatAvailability class, which represents
 *              seat availability for a specific show. Stores details such as
 *              availability ID, associated show, and seat availability map.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#include "ShowSeatAvailability.h"
#include "Show.h"

 /*
  * Function: ShowSeatAvailability::ShowSeatAvailability
  * Description: Default constructor initializing seat availability with default values.
  * Returns:
  *    ShowSeatAvailability object
  */
ShowSeatAvailability::ShowSeatAvailability() :
	m_showSeatAvailabilityId(""),
	m_show(nullptr)
{
}

/*
 * Function: ShowSeatAvailability::ShowSeatAvailability
 * Description: Parameterized constructor initializing seat availability details.
 * Parameters:
 *    const std::string& showSeatAvailabilityId - Unique seat availability identifier
 *    Show* show - Associated show object
 *	  const std::map<std::string, Enums::BookingStatus>& seatMap - map of seat ids with booking status
 * Returns:
 *    ShowSeatAvailability object
 */
ShowSeatAvailability::ShowSeatAvailability(const std::string& showSeatAvailabilityId, Show* show, const std::map<std::string, Enums::BookingStatus>& seatMap) :
	m_showSeatAvailabilityId(showSeatAvailabilityId), m_show(show), m_seatAvailabilityMap(seatMap)
{}

/*
 * Function: ShowSeatAvailability::getShowAvailabiltyId
 * Description: Retrieves the unique seat availability ID.
 * Returns:
 *    const std::string& - Seat availability ID
 */
const std::string& ShowSeatAvailability::getShowAvailabiltyId() const
{
	return m_showSeatAvailabilityId;
}

/*
 * Function: ShowSeatAvailability::getShow
 * Description: Retrieves the associated show object.
 * Returns:
 *    Show* - Pointer to the show
 */
Show* ShowSeatAvailability::getShow() const
{
	return m_show;
}

/*
 * Function: ShowSeatAvailability::getSeatAvailabilityMap
 * Description: Retrieves the seat availability map.
 * Returns:
 *    const std::map<std::string, Enums::BookingStatus>& - Map of seat IDs to Seat objects
 */
const std::map<std::string, Enums::BookingStatus>& ShowSeatAvailability::getSeatAvailabilityMap() const
{
	return m_seatAvailabilityMap;
}

/*
 * Function: ShowSeatAvailability::setShowSeatAvailability
 * Description: Sets the unique seat availability ID.
 * Parameters:
 *    const std::string& showSeatAvailabilityId - New seat availability ID
 * Returns:
 *    void
 */
void ShowSeatAvailability::setShowSeatAvailabilityId(const std::string& showSeatAvailabilityId)
{
	m_showSeatAvailabilityId = showSeatAvailabilityId;
}

/*
 * Function: ShowSeatAvailability::setShow
 * Description: Sets the associated show object.
 * Parameters:
 *    Show* show - Pointer to the show
 * Returns:
 *    void
 */
void ShowSeatAvailability::setShow(Show* show)
{
	m_show = show;
}

/*
 * Function: ShowSeatAvailability::setSeatAvailabilityMap
 * Description: Sets the seat availability map.
 * Parameters:
 *    std::map<std::string, Enums::BookingStatus>& seatAvailabilityMap - New seat availability map
 * Returns:
 *    void
 */
void ShowSeatAvailability::setSeatAvailabilityMap(std::map<std::string, Enums::BookingStatus>& seatAvailabilityMap)
{
	m_seatAvailabilityMap = seatAvailabilityMap;
}

/*
 * Function: serialize
 * Description: Converts Theatre object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string ShowSeatAvailability::serialize()
{
    std::string result = m_showSeatAvailabilityId + config::delimeter::comma;
    if (m_show)
    {
        result += m_show->getShowId() + config::delimeter::comma;
    }
    else
    {
        result += config::delimeter::comma;
    }
    if (!m_seatAvailabilityMap.empty())
    {
        for (std::map<std::string, Enums::BookingStatus>::const_iterator iterator = m_seatAvailabilityMap.begin(); iterator != m_seatAvailabilityMap.end(); ++iterator)
        {
            result += iterator->first + config::delimeter::colon + Enums::getBookingStatusString(iterator->second);
            if (std::next(iterator) != m_seatAvailabilityMap.end())
            {
                result += config::delimeter::verticalBar;
            }
        }
    }
    else
    {
        result += config::delimeter::comma;
    }
    return result;
}

/*
 * Function: Theatre::deserialize
 * Description: Converts a single CSV-formatted line into a Theatre object.
 *              Extracts fields such as theatreId, name, city, address,
 *              phoneNumber, email, theatreOwnerId, status, screenIds, and movieIds.
 *              The TheatreOwner pointer and associations with Screens and Movies
 *              are initialized to nullptr or left empty, and can be restored later
 *              by higher-level services.
 * Parameters:
 *    line - reference to a CSV-formatted string containing theatre data
 * Returns:
 *    Pointer to a newly constructed Theatre object
 */
ShowSeatAvailability* ShowSeatAvailability::deserialize(const std::string& line)
{
    std::string showSeatAvailabilityId, showId, seatBlock;
    std::stringstream lineStream(line);
    getline(lineStream, showSeatAvailabilityId, ',');
    getline(lineStream, showId, ',');
    getline(lineStream, seatBlock, ',');
    std::map<std::string, Enums::BookingStatus> emptySeatAvailabilityMap;
    ShowSeatAvailability* showSeatAvailability = Factory::getObject<ShowSeatAvailability>(showSeatAvailabilityId,nullptr, emptySeatAvailabilityMap);
    if (!seatBlock.empty())
    {
        std::stringstream seatStream(seatBlock);
        std::string seatEntry;
        while (getline(seatStream, seatEntry, config::delimeter::verticalBar[0]))
        {
            int pos = int(seatEntry.find(config::delimeter::colon));
            if (pos != std::string::npos)
            {
                std::string seatId = seatEntry.substr(0, pos);
                std::string statusStr = seatEntry.substr(pos + 1);
                showSeatAvailability->m_seatAvailabilityMap[seatId] = Enums::getBookingStatus(statusStr);
            }
        }
    }
    return showSeatAvailability;
}