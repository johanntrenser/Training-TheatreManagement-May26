/*
 * File: ShowManagementService.h
 * Description: Declares the ShowManagementService class, which provides
 *              functionality for managing shows within the system.
 *              This service will handle operations such as creating,
 *              updating, retrieving, and deleting show records, as well
 *              as managing show schedules and related details.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <fstream>
#include <map>
#include <ctime>
#include "DataStore.h"
#include "ApplicationConfig.h"
class ShowManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::SHOW_FILEPATH;
public:
	ShowManagementService() : m_dataStore(DataStore::getInstance()) {}
	void saveShowData();
};

