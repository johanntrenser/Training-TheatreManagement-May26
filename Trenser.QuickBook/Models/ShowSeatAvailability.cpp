
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
{}

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