#include <sstream>
#include <iomanip>
#include "LogManagementService.h"
#include "Factory.h"

/*
 * Function: LogManagementService
 * Description: Default constructor that initializes the log management service
 *              with a reference to the shared DataStore instance.
 * Parameters: None
 * Returns: None
 */
LogManagementService::LogManagementService() :
    m_dataStore(DataStore::getInstance())
{
}

const std::string LogManagementService::generateLogId()
{
	const std::map<std::string, Log*>& logs = m_dataStore.getLogs();
	int idNumber = static_cast<int>(logs.size()) + 1;
	std::ostringstream buffer;
	buffer << "LOG" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

void LogManagementService::addLog(const std::string& description, const Enums::LogType logType)
{
	Log* log = Factory::getObject<Log>(generateLogId(), description, logType);
	if (log != nullptr)
	{
		m_dataStore.addLog(log);
	}
}


