/*
 * File: ScreenManagementService.h
 * Description: Declares the ScreenManagementService class, which provides
 *              functionality for managing screens within the system.
 *              This service will handle operations such as adding,
 *              configuring, retrieving, and deleting screen records.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include "DataStore.h"
#include "LogManagementService.h"
#include "NamedMutex.h"
#include "ScopedLock.h"

class ScreenManagementService
{
private:
    DataStore& m_dataStore;
    LogManagementService m_logManagementService;
    NamedMutex m_screenMutex;
    NamedMutex m_seatMutex;
public:
    ScreenManagementService();
    Enums::ProcessStatus addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount);
    std::vector<std::vector<Seat*>> generateSeatGrid(Screen* screen, int seatRows, int seatColumns, double seatAmount);
    void cleanupSeatGrid(std::vector<std::vector<Seat*>>& seatGrid, std::vector<Seat*>& seatRow);
    std::string generateScreenId();
    Enums::ProcessStatus updateScreenName(const std::string& theatreId, const std::string& screenId, const std::string& name);
    Enums::ProcessStatus deactivateScreen(const std::string& theatreId, const std::string& screenId);
    Enums::ProcessStatus hasActiveShows(const std::string& theatreId, const std::string& screenId);
    Enums::ProcessStatus reactivateScreen(const std::string& theatreId, const std::string& screenId);
    const std::vector<const Screen*> viewTheatreScreens(const std::string& theatreId);
    Enums::UserType getAuthenticatedUserType();
    std::string generateSeatId();
};
