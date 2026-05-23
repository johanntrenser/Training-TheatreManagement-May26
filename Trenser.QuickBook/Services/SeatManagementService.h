/*
 * File: SeatManagementService.h
 * Description: Declares the SeatManagementService class, which provides
 *              functionality for managing seats within the system.
 *              This service will handle operations such as adding,
 *              updating, retrieving, and deleting seat records, as well
 *              as managing seat availability for shows.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <vector>
#include "DataStore.h"

class SeatManagementService
{
private:
	DataStore& m_dataStore;
public:
	SeatManagementService();
	Enums::ProcessStatus updateSeatLayout(Screen* screen, const int newRows, const int newColumns);
	const std::vector<std::vector<Seat*>>& getSeatLayout(const Screen* screen) const;
};