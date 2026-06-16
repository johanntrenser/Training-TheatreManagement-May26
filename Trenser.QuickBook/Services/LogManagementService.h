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
#include "DataStore.h"
#include "FileManagement.h"
#include "NamedMutex.h"
#include "ScopedLock.h"

class LogManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::LOG_FILEPATH;
	NamedMutex m_mutex;
public:
	LogManagementService();
	const std::string generateLogId();
	void addLog(const std::string& description, const Enums::LogType logType);
	const std::vector<const Log*> getLogsByType(const Enums::LogType logType);
};