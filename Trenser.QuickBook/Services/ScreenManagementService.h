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
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include "FileManagement.h"
#include "DataStore.h"
#include "ApplicationConfig.h"

class ScreenManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::SCREEN_FILEPATH;
public:
	ScreenManagementService();
	void saveScreenData();
};