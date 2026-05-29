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
#include <fstream>
#include <map>
#include <sstream>
#include "FileManagement.h"
#include "DataStore.h"
#include "ApplicationConfig.h"

class SeatManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::SEAT_FILEPATH;
public:
	SeatManagementService();
	void saveSeatData();
};

