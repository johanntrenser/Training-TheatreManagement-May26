/*
 * File: LogManagementService.h
 * Description: Declares the LogManagementService class, which provides
 *              functionality for managing system logs. The class interacts
 *              with the shared DataStore instance to record, retrieve, and
 *              maintain log entries for auditing and monitoring purposes.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <fstream>
#include <map>
#include <sstream>
#include "FileManagement.h"
#include "ApplicationConfig.h"
#include "DataStore.h"

class LogManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::LOG_FILEPATH;
public:
	LogManagementService();
	void saveLogData();
};

