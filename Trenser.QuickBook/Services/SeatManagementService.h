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
#include "NamedMutex.h"
#include "ScopedLock.h"
#include "LogManagementService.h"

class SeatManagementService
{
private:
    DataStore& m_dataStore;
    NamedMutex m_mutex;
    LogManagementService m_logManagementService;
public:
    SeatManagementService();
    Enums::ProcessStatus updateSeatLayout(const std::string& selectedScreenId, int newRows, int newColumns, double amount);
    void clearSeatGrid(std::vector<std::vector<Seat*>>& seatGrid);
    const std::vector<std::vector<Seat*>>& getSeatLayout(const std::string& selectedScreenId);
    Enums::ProcessStatus deactivateSeat(const std::string& selectedScreenId, const std::string& seatId);
    Enums::ProcessStatus deactivateSeats(const std::string& selectedScreenId);
    Enums::ProcessStatus hasActiveSeatBooking(Screen* screen, const std::string& seatId);
    Enums::ProcessStatus reactivateSeat(const std::string& selectedScreenId, const std::string& seatId);
    Enums::ProcessStatus reactivateSeats(const std::string& selectedScreenId);
    const std::vector<std::vector<std::string>> getShowsSeatLayout(const Show* show);
    std::string formatSeatDisplay(const Seat* seat, const std::map<std::string, Enums::BookingStatus>& availabilityMap);
    std::string generateSeatId();
};