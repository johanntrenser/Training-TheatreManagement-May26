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

/*
 * Function: Log::getLogType
 * Description: Retrieves the log type.
 * Returns:
 *    const Enums::LogType - Log type enumeration value
 */
const Enums::LogType Log::getLogType() const
{
    return m_logType;
}

/*
 * Function: Log::getTimestamp
 * Description: Retrieves the timestamp of the log entry.
 * Returns:
 *    const util::Timestamp& - Timestamp object
 */
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

/*
 * Function: Log::setLogType
 * Description: Updates the log type.
 * Parameters:
 *    const Enums::LogType logType - New log type
 * Returns: None
 */
void Log::setLogType(const Enums::LogType logType)
{
    m_logType = logType;
}

/*
 * Function: Log::setTimestamp
 * Description: Updates the timestamp of the log entry.
 * Parameters:
 *    const util::Timestamp& timestamp - New timestamp
 * Returns: None
 */
void Log::setTimestamp(const util::Timestamp& timestamp)
{
    m_timestamp = timestamp;
}

/*
 * Function: Log::toString
 * Description: Converts the log entry to a formatted string representation.
 * Returns:
 *    std::string - String containing timestamp, log type, ID, and description
 */
std::string Log::toString() const
{
    std::ostringstream buffer;
    buffer << "<" << m_timestamp.toString() << "> ";
    buffer << Enums::getLogTypeString(m_logType) << ": ";
    buffer << m_logId << " - ";
    buffer << m_description << std::endl;
    return buffer.str();
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
        m_description+config::delimeter::comma+
        Enums::getLogTypeString(m_logType)+config::delimeter::comma+
        m_timestamp.toString();
}

/*
 * Function: Log::deserialize
 * Description: Converts a single CSV-formatted line into a Log object.
 *              Extracts fields such as logId and logDescription from the line.
 *              The Log object is constructed with these values, while any
 *              additional associations (e.g., with User or System events)
 *              can be restored later by higher-level services.
 * Parameters:
 *    lines - reference to a CSV-formatted string containing log data
 * Returns:
 *    Pointer to a newly constructed Log object
 */
Log* Log::deserialize(const std::string& lines)
{
    std::string logId, logDescription,type,time;
    std::stringstream lineStream(lines);
    getline(lineStream, logId, ',');
    getline(lineStream, logDescription, ',');
    getline(lineStream, type, ',');
    getline(lineStream, time, ',');
    Enums::LogType logType = Enums::getLogType(type);
    Log* log = Factory::getObject<Log>(logId, logDescription, logType);
    if (!time.empty())
    {
        util::Timestamp timeStamp = util::Timestamp::fromString(time);
        log->setTimestamp(timeStamp);
    }
    return log;
}