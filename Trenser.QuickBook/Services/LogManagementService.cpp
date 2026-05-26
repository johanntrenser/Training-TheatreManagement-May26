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
    const std::map<std::string, Log*>& logs = m_dataStore.getLogs();
    std::ofstream logFile(PATH, std::ios::trunc);
    if (!logFile.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    logFile << "LOG ID,DESCRIPTION\n";
    for (std::map<std::string, Log*>::const_iterator iterator = logs.begin(); iterator != logs.end(); ++iterator)
    {
        logFile << (iterator->second)->getLogId() << ","
            << (iterator->second)->getDescription() << "\n";
    }
    logFile.close();
}