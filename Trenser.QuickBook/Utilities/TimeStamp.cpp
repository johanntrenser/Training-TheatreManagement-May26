#include "TimeStamp.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

/*
    * Function: Timestamp
    * Description: Default constructor that initializes the Timestamp
    *              object with the current system time.
    * Parameters: None
    * Returns: None
    */
util::Timestamp::Timestamp()
{
    m_time = std::time(nullptr);
}

/*
    * Function: Timestamp (private)
    * Description: Constructs a Timestamp object from a given time value.
    * Parameters:
    *   - time: A std::time_t value representing the time.
    * Returns: None
    */
util::Timestamp::Timestamp(std::time_t timeValue)
{
    m_time = timeValue;
}

/*
    * Function: fromString
    * Description: Creates a Timestamp object from a string representation.
    * Parameters:
    *   - timeString: A string representing the time (format-dependent).
    * Returns: A Timestamp object initialized with the parsed time.
    */
util::Timestamp util::Timestamp::fromString(const std::string& timeString)
{
    std::tm timeStruct = {};
    std::istringstream inputStream(timeString);
    inputStream >> std::get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    if (inputStream.fail())
    {
        throw std::runtime_error("Invalid time format");
    }
    std::time_t parsedTimestamp = std::mktime(&timeStruct);
    return Timestamp(parsedTimestamp);
}

/*
    * Function: toString
    * Description: Converts the Timestamp object to a string representation.
    * Parameters: None
    * Returns: A string representing the time.
    */
std::string util::Timestamp::toString() const
{
    std::tm timeStruct = {};
    localtime_s(&timeStruct, &m_time);
    std::ostringstream outputStream;
    outputStream << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    return outputStream.str();
}

/*
    * Function: getDurationInSeconds
    * Description: Calculates the duration between two Timestamp objects
    *              in seconds.
    * Parameters:
    *   - start: Starting Timestamp.
    *   - end: Ending Timestamp.
    * Returns: Duration in seconds as a double.
    */
double util::Timestamp::getDurationInSeconds(const Timestamp& startTimestamp, const Timestamp& endTimestamp)
{
    return std::difftime(endTimestamp.m_time, startTimestamp.m_time);
}

/*
    * Function: getDateAsInt
    * Description: Retrieves the date portion of the Timestamp as an integer.
    * Parameters: None
    * Returns: Integer representation of the date (format-dependent).
    */
int util::Timestamp::getDateAsInt() const
{
    std::tm timeStruct{};
    localtime_s(&timeStruct, &m_time);
    int year = timeStruct.tm_year + 1900;
    int month = timeStruct.tm_mon + 1;
    int day = timeStruct.tm_mday;
    return year * 10000 + month * 100 + day;
}

/*
    * Function: getDurationInMinutes
    * Description: Calculates the duration between two Timestamp objects
    *              in minutes.
    * Parameters:
    *   - start: Starting Timestamp.
    *   - end: Ending Timestamp.
    * Returns: Duration in minutes as a double.
    */
double util::Timestamp::getDurationInMinutes(const Timestamp& startTimestamp, const Timestamp& endTimestamp)
{
    return getDurationInSeconds(startTimestamp, endTimestamp) / 60.0;
}

/*
    * Function: getDurationInHours
    * Description: Calculates the duration between two Timestamp objects
    *              in hours.
    * Parameters:
    *   - start: Starting Timestamp.
    *   - end: Ending Timestamp.
    * Returns: Duration in hours as a double.
    */
double util::Timestamp::getDurationInHours(const Timestamp& startTimestamp, const Timestamp& endTimestamp)
{
    return getDurationInSeconds(startTimestamp, endTimestamp) / 3600.0;
}

/*
    * Function: operator<
    * Description: Compares two Timestamp objects to check if the left-hand
    *              side is less than the right-hand side.
    * Parameters:
    *   - other: Timestamp to compare against.
    * Returns: True if lhs < rhs, false otherwise.
    */
bool util::Timestamp::operator<(const Timestamp& other) const
{
    return m_time < other.m_time;
}

/*
    * Function: operator>
    * Description: Compares two Timestamp objects to check if the left-hand
    *              side is greater than the right-hand side.
    * Parameters:
    *   - other: Timestamp to compare against.
    * Returns: True if lhs > rhs, false otherwise.
    */
bool util::Timestamp::operator>(const Timestamp& other) const
{
    return m_time > other.m_time;
}

/*
    * Function: operator<=
    * Description: Compares two Timestamp objects to check if the left-hand
    *              side is less than or equal to the right-hand side.
    * Parameters:
    *   - other: Timestamp to compare against.
    * Returns: True if lhs <= rhs, false otherwise.
    */
bool util::Timestamp::operator<=(const Timestamp& other) const
{
    return m_time <= other.m_time;
}

/*
    * Function: operator>=
    * Description: Compares two Timestamp objects to check if the left-hand
    *              side is greater than or equal to the right-hand side.
    * Parameters:
    *   - other: Timestamp to compare against.
    * Returns: True if lhs >= rhs, false otherwise.
    */
bool util::Timestamp::operator>=(const Timestamp& other) const
{
    return m_time >= other.m_time;
}

/*
    * Function: operator==
    * Description: Compares two Timestamp objects to check if they are equal.
    * Parameters:
    *   - other: Timestamp to compare against.
    * Returns: True if lhs == rhs, false otherwise.
    */
bool util::Timestamp::operator==(const Timestamp& other) const
{
    return m_time == other.m_time;
}

/*
 * Function: createTime
 * Description: Constructs a time_t value from individual date and time components.
 *              Adjusts year and month to tm struct conventions (year since 1900,
 *              zero-indexed month) before converting using mktime.
 * Parameters:
 *    year   - Full year (e.g. 2025)
 *    month  - Month as 1-12
 *    day    - Day of the month as 1-31
 *    hour   - Hour as 0-23
 *    minute - Minute as 0-59
 * Returns:
 *    time_t representing the specified date and time in local time,
 *    or -1 if the conversion fails.
 */
time_t util::createTime(int year, int month, int day, int hour, int minute)
{
    tm time = {};
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_sec = 0;
    return mktime(&time);
}

/*
    * Function: serializeTime
    * Description: Converts a time_t value into a formatted string "YYYY-MM-DD HH:MM"
    * Parameters:
    *    t - time_t value to convert
    * Returns:
    *    Formatted date-time string
 */
std::string util::serializeTime(time_t time)
{
    struct tm timeInfo = {};
    localtime_s(&timeInfo, &time);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &timeInfo);
    return std::string(buffer);
}

/*
    * Function: deserializeTime
    * Description: Parses a formatted string "YYYY-MM-DD HH:MM" into a time_t value
    * Parameters:
    *    timeStr - date-time string to parse
    * Returns:
    *    Converted time_t value, or -1 if parsing fails
 */
time_t util::deserializeTime(const std::string& timeString)
{
    struct tm timeInfo = {};
    std::istringstream ss(timeString);
    ss >> std::get_time(&timeInfo, "%Y-%m-%d %H:%M");
    if (ss.fail())
    {

        return -1;
    }
    return mktime(&timeInfo);
}
