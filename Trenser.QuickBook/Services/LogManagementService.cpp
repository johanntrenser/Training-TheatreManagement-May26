#include "LogManagementService.h"

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
    for (int index = 0; index < lines.size(); ++index)
    {
        Log* log = Log::deserialize(lines[index]);
        m_dataStore.addLog(log);
    }
}