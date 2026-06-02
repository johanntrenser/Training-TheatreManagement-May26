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

/*
 * Function: LogManagementService::getLogsByType
 * Description: Retrieves all logs from the DataStore that match the specified
 *              log type. Iterates through the stored logs and filters them
 *              into a vector of immutable log pointers.
 * Parameters:
 *    const Enums::LogType logType - The type of logs to filter (e.g., ERROR, SYSTEM_ACTIVITY, UNKNOWN)
 * Returns:
 *    const std::vector<const Log*> - Vector containing pointers to logs of the specified type
 */
const std::vector<const Log*> LogManagementService::getLogsByType(const Enums::LogType logType)
{
	std::vector<const Log*> filteredLogs;
	const std::map<std::string, Log*> logs = m_dataStore.getLogs();
	for (std::map<std::string, Log*>::const_iterator iterator = logs.begin(); iterator != logs.end(); ++iterator)
	{
		if (iterator->second->getLogType() == logType)
		{
			filteredLogs.push_back(iterator->second);
		}
	}
	return filteredLogs;
}

/*
 * Function: LogManagementService::saveLogData
 * Description: Saves all log data from the DataStore into a CSV file.
 *              Includes log details such as Log ID and description.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void LogManagementService::saveLogData()
{
	std::vector<std::string> lines;
	lines.push_back(config::Header::LOG_HEADER);
	const std::map<std::string, Log*>& logs = m_dataStore.getLogs();
	for (std::map<std::string, Log*>::const_iterator iterator = logs.begin(); iterator != logs.end(); ++iterator)
	{
		lines.push_back((iterator->second)->serialize());
	}
	FileManagement::writeLines(std::string(config::File::LOG_FILEPATH), lines);
}

/*
 * Function: LogManagementService::loadLogData
 * Description: Loads all log entries from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Log object via Log::deserialize,
 *              and adds the reconstructed Log to the DataStore.
 *              This ensures that system logs are restored and available
 *              for auditing or tracking purposes.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void LogManagementService::loadLogData()
{
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 1; index < lines.size(); ++index)
	{
		Log* log = Log::deserialize(lines[index]);
		m_dataStore.addLog(log);
	}
}