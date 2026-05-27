/*
 * File: ShowSeatAvailability.h
 * Description: Defines the ShowSeatAvailability class, which manages seat
 *              availability for a specific show. Stores details such as
 *              availability ID, associated show, and a map of seat IDs to
 *              seat objects.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include <map>
#include "Seat.h"

class Show;

class ShowSeatAvailability
{
private:
	std::string m_showSeatAvailabilityId;
	Show* m_show;
	std::map<std::string, Enums::BookingStatus> m_seatAvailabilityMap;
public:
	ShowSeatAvailability();
	ShowSeatAvailability(const std::string& showSeatAvailabilityId, Show* show, const std::map<std::string, Enums::BookingStatus>& seatMap);
	const std::string& getShowAvailabiltyId() const;
	Show* getShow() const;
	const std::map<std::string, Enums::BookingStatus>& getSeatAvailabilityMap() const;
	void setShowSeatAvailabilityId(const std::string& showSeatAvailabilityId);
	void setShow(Show* show);
	void setSeatAvailabilityMap(std::map<std::string, Enums::BookingStatus>& seatAvailabilityMap);
};