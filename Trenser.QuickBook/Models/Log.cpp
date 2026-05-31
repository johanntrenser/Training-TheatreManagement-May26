/*
 * File: Log.cpp
 * Description: Implements the Log class, providing constructors, getters,
 *              and setters for log attributes such as log ID and description.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "Log.h"

 /*
  * Function: Log::Log
  * Description: Default constructor. Initializes log attributes to empty strings.
  * Returns: None
  */
Log::Log()
    : m_logId(""),
    m_description("")
{}

/*
 * Function: Log::Log
 * Description: Parameterized constructor. Initializes log attributes with
 *              provided values.
 * Parameters:
 *    const std::string& logId - Unique log identifier
 *    const std::string& description - Description of the log entry
 * Returns: None
 */
Log::Log(const std::string& logId,
    const std::string& description)
    : m_logId(logId),
    m_description(description)
{}

/*
 * Function: Log::getLogId
 * Description: Retrieves the log ID.
 * Returns:
 *    const std::string& - Log ID
 */
const std::string& Log::getLogId() const
{
    return m_logId;
}

/*
 * Function: Log::getDescription
 * Description: Retrieves the log description.
 * Returns:
 *    const std::string& - Log description
 */
const std::string& Log::getDescription() const
{
    return m_description;
}

/*
 * Function: Log::setLogId
 * Description: Updates the log ID.
 * Parameters:
 *    const std::string& id - New log ID
 * Returns: None
 */
void Log::setLogId(const std::string& id)
{
    m_logId = id;
}

/*
 * Function: Log::setDescription
 * Description: Updates the log description.
 * Parameters:
 *    const std::string& description - New log description
 * Returns: None
 */
void Log::setDescription(const std::string& description)
{
    m_description = description;
}

/*
 * Function: serialize
 * Description: Converts Log object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string Log::serialize()
{
    return m_logId + config::delimeter::comma +
        m_description;
}