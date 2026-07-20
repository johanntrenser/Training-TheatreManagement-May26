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
 * Function: ShowSeatAvailability::serialize
 * Description: Serializes the ShowSeatAvailability object into a SharedShowSeatAvailability
 *              struct suitable for shared memory storage. Copies seat IDs and their
 *              statuses into fixed-size arrays.
 * Parameters:
 *    sharedSeatAvailability (SharedShowSeatAvailability&) -
 *        Reference to a struct to be populated with serialized data
 * Returns:
 *    void
 */
void ShowSeatAvailability::serialize(SharedShowSeatAvailability& sharedSeatAvailability)
{
    strncpy_s(sharedSeatAvailability.availabilityId, sizeof(sharedSeatAvailability.availabilityId), m_showSeatAvailabilityId.c_str(), _TRUNCATE);
    strncpy_s(sharedSeatAvailability.showId, sizeof(sharedSeatAvailability.showId), m_show->getShowId().c_str(), _TRUNCATE);
    sharedSeatAvailability.seatCount = static_cast<int>(m_seatAvailabilityMap.size());
    int seatIndex = 0;
    for (std::map<std::string, Enums::BookingStatus>::iterator iterator = m_seatAvailabilityMap.begin();
        iterator != m_seatAvailabilityMap.end(); ++iterator)
    {
        std::string seatId = iterator->first;
        strncpy_s(sharedSeatAvailability.seatIds[seatIndex], sizeof(sharedSeatAvailability.seatIds[seatIndex]), seatId.c_str(), _TRUNCATE);
        sharedSeatAvailability.seatStatuses[seatIndex] = static_cast<int>(iterator->second);
        seatIndex++;
    }
}

/*
 * Function: ShowSeatAvailability::deserialize
 * Description: Reconstructs a ShowSeatAvailability object from a SharedShowSeatAvailability
 *              struct stored in shared memory. Restores seat IDs and their booking statuses
 *              into a map. Associations with Show are left unlinked and can be restored later.
 * Parameters:
 *    sharedSeatAvailability (const SharedShowSeatAvailability*) -
 *        Pointer to a struct containing serialized seat availability data
 * Returns:
 *    ShowSeatAvailability* - Pointer to a newly constructed object if successful,
 *                            nullptr if input is invalid
 */
ShowSeatAvailability* ShowSeatAvailability::deserialize(const SharedShowSeatAvailability* sharedSeatAvailability)
{
    if (sharedSeatAvailability == nullptr)
    {
        return nullptr;
    }
    std::map<std::string, Enums::BookingStatus> seatAvailabilityMap;
    for (int index = 0; index < sharedSeatAvailability->seatCount; ++index)
    {
        seatAvailabilityMap[sharedSeatAvailability->seatIds[index]] = static_cast<Enums::BookingStatus>(sharedSeatAvailability->seatStatuses[index]);
    }
    ShowSeatAvailability* seatAvailability = Factory::getObject<ShowSeatAvailability>(sharedSeatAvailability->availabilityId, nullptr, seatAvailabilityMap);
    return seatAvailability;
}