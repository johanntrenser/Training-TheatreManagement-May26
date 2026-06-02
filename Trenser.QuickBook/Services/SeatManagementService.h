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
#include "FileManagement.h"

class SeatManagementService
{
private:
    DataStore& m_dataStore;
    const std::string& PATH_SEAT = config::File::SEAT_FILEPATH;
    const std::string& PATH_SHOW_SEAT = config::File::SHOW_SEAT_FILEPATH;
public:
    SeatManagementService();
    Enums::ProcessStatus updateSeatLayout(Screen* screen, int newRows, int newColumns, double amount);
    void clearSeatGrid(std::vector<std::vector<Seat*>>& seatGrid);
    const std::vector<std::vector<Seat*>>& getSeatLayout(const Screen* screen) const;
    Enums::ProcessStatus deactivateSeat(Screen* screen, const std::string& seatId);
    Enums::ProcessStatus hasActiveSeatBooking(Screen* screen, const std::string& seatId);
    Enums::ProcessStatus reactivateSeat(Screen* screen, const std::string& seatId);
    const std::vector<std::vector<std::string>> getShowsSeatLayout(const Show* show);
    std::string formatSeatDisplay(const Seat* seat, const std::map<std::string, Enums::BookingStatus>& availabilityMap);
    void saveSeatData();
    void loadSeatData();
    void saveShowSeatAvailabilityData();
    void loadShowSeatAvailabilityData();
    std::string generateSeatId();
};