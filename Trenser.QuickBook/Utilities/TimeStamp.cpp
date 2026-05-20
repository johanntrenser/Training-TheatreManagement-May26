#include "TimeStamp.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

util::Timestamp::Timestamp()
{
    m_time = std::time(nullptr);
}

util::Timestamp::Timestamp(std::time_t timeValue)
{
    m_time = timeValue;
}

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

std::string util::Timestamp::toString() const
{
    std::tm timeStruct = {};
    localtime_s(&timeStruct, &m_time);
    std::ostringstream outputStream;
    outputStream << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    return outputStream.str();
}

double util::Timestamp::getDurationInSeconds(const Timestamp& startTimestamp, const Timestamp& endTimestamp)
{
    return std::difftime(endTimestamp.m_time, startTimestamp.m_time);
}

int util::Timestamp::getDateAsInt() const
{
    std::tm timeStruct{};
    localtime_s(&timeStruct, &m_time);
    int year = timeStruct.tm_year + 1900;
    int month = timeStruct.tm_mon + 1;
    int day = timeStruct.tm_mday;
    return year * 10000 + month * 100 + day;
}

double util::Timestamp::getDurationInMinutes(const Timestamp& startTimestamp, const Timestamp& endTimestamp)
{
    return getDurationInSeconds(startTimestamp, endTimestamp) / 60.0;
}

double util::Timestamp::getDurationInHours(const Timestamp& startTimestamp, const Timestamp& endTimestamp)
{
    return getDurationInSeconds(startTimestamp, endTimestamp) / 3600.0;
}

bool util::Timestamp::operator<(const Timestamp& other) const
{
    return m_time < other.m_time;
}

bool util::Timestamp::operator>(const Timestamp& other) const
{
    return m_time > other.m_time;
}

bool util::Timestamp::operator<=(const Timestamp& other) const
{
    return m_time <= other.m_time;
}

bool util::Timestamp::operator>=(const Timestamp& other) const
{
    return m_time >= other.m_time;
}

bool util::Timestamp::operator==(const Timestamp& other) const
{
    return m_time == other.m_time;
}
