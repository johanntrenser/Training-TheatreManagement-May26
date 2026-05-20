#include "ShowSeatAvailability.h"
#include "Show.h"

ShowSeatAvailability::ShowSeatAvailability() :
	m_showSeatAvailabilityId(""),
	m_show(nullptr)
{}

ShowSeatAvailability::ShowSeatAvailability(const std::string& showSeatAvailabilityId, Show* show) :
	m_showSeatAvailabilityId(showSeatAvailabilityId), m_show(show)
{}

const std::string& ShowSeatAvailability::getShowAvailabiltyId() const
{
	return m_showSeatAvailabilityId;
}

Show* ShowSeatAvailability::getShow() const
{
	return m_show;
}

const std::map<std::string, Seat*>& ShowSeatAvailability::getSeatAvailabilityMap() const
{
	return m_seatAvailabilityMap;
}

void ShowSeatAvailability::setShowSeatAvailability(const std::string& showSeatAvailabilityId)
{
	m_showSeatAvailabilityId = showSeatAvailabilityId;
}

void ShowSeatAvailability::setShow(Show* show)
{
	m_show = show;
}

void ShowSeatAvailability::setSeatAvailabilityMap(std::map<std::string, Seat*>& seatAvailabilityMap)
{
	m_seatAvailabilityMap = seatAvailabilityMap;
}