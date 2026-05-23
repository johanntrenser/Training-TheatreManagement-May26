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

/*
 * Function: LogManagementService::generateLogId
 * Description: Generates a unique log ID based on the current number of logs
 *              in the DataStore. The ID is formatted as "LOG" followed by a
 *              zero-padded number (e.g., LOG001).
 * Parameters: None
 * Returns:
 *    const std::string - Generated log ID
 */
const std::string LogManagementService::generateLogId()
{
	const std::map<std::string, Log*>& logs = m_dataStore.getLogs();
	int idNumber = static_cast<int>(logs.size()) + 1;
	std::ostringstream buffer;
	buffer << "LOG" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

/*
 * Function: LogManagementService::addLog
 * Description: Creates and adds a new log entry to the DataStore using the
 *              provided description and log type. The log ID is generated
 *              automatically.
 * Parameters:
 *    const std::string& description - Description of the log entry
 *    const Enums::LogType logType   - Type of the log entry
 * Returns: None
 */
void LogManagementService::addLog(const std::string& description, const Enums::LogType logType)
{
	Log* log = Factory::getObject<Log>(generateLogId(), description, logType);
	if (log != nullptr)
	{
		m_dataStore.addLog(log);
	}
}


