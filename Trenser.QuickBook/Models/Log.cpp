/*
 * File: Log.cpp
 * Description: Implements the Log class, providing constructors, getters,
 *              and setters for log attributes such as log ID and description.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <sstream>
#include "Log.h"
#include "TimeStamp.h"

 /*
  * Function: Log::Log
  * Description: Default constructor. Initializes log attributes to empty strings.
  * Returns: None
  */
Log::Log()
    : m_logId(""),
    m_description(""),
    m_logType(Enums::LogType::UNKNOWN)
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
    const std::string& description,
    const Enums::LogType logType)
    : m_logId(logId),
    m_description(description),
    m_logType(logType),
    m_timestamp(util::Timestamp())
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

const Enums::LogType Log::getLogType() const
{
    return m_logType;
}

const util::Timestamp& Log::getTimestamp() const
{
    return m_timestamp;
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

void Log::setLogType(const Enums::LogType logType)
{
    m_logType = logType;
}

void Log::setTimestamp(const util::Timestamp& timestamp)
{
    m_timestamp = timestamp;
}

std::string Log::toString()
{
    std::ostringstream buffer;
    buffer << "<" << m_timestamp.toString() << "> ";
    buffer << Enums::getLogTypeString(m_logType) << ": ";
    buffer << m_logId << " - ";
    buffer << m_description << std::endl;
    return buffer.str();
}

