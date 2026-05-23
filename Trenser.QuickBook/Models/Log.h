/*
 * File: Log.h
 * Description: Implements the Log class, providing constructors, getters,
 *              and setters for log attributes such as log ID and description.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include "Enums.h"
#include "TimeStamp.h"

class Log
{
private:
    std::string m_logId;
    std::string m_description;
    Enums::LogType m_logType;
    util::Timestamp m_timestamp;
public:
    Log();
    Log(const std::string& logId, const std::string& description, const Enums::LogType logType);
    const std::string& getLogId() const;
    const std::string& getDescription() const;
    const Enums::LogType getLogType() const;
    const util::Timestamp& getTimestamp() const;
    void setLogId(const std::string& id);
    void setDescription(const std::string& description);
    void setLogType(const Enums::LogType logType);
    void setTimestamp(const util::Timestamp& timestamp);
    std::string toString();
};


