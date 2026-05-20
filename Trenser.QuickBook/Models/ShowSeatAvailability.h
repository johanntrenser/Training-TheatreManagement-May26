#pragma once
#include <map>
#include "Seat.h"

class Show;

class ShowSeatAvailability
{
private:
	std::string m_showSeatAvailabilityId;
	Show* m_show;
	std::map<std::string, Seat*> m_seatAvailabilityMap;
public: 
	ShowSeatAvailability();
	ShowSeatAvailability(const std::string& showSeatAvailabilityId, Show* show);
	const std::string& getShowAvailabiltyId() const;
	Show* getShow() const;
	const std::map<std::string, Seat*>& getSeatAvailabilityMap() const;
	void setShowSeatAvailability(const std::string& showSeatAvailabilityId);
	void setShow(Show* show);
	void setSeatAvailabilityMap(std::map<std::string, Seat*>& seatAvailabilityMap);
};

