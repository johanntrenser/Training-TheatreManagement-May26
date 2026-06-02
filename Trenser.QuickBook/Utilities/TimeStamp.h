/*
 * File: Timestamp.h
 * Description: Declares the Timestamp class, which provides functionality
 *              for representing and manipulating time values. Includes
 *              methods for converting to/from strings, calculating durations,
 *              retrieving date values, and performing comparison operations.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <ctime>
#include <string>

namespace util
{
    class Timestamp
    {
    private:
        std::time_t m_time;
        Timestamp(std::time_t time);
    public:
        Timestamp();
        static Timestamp fromString(const std::string& timeString);
        std::string toString() const;
        static double getDurationInHours(const Timestamp&, const Timestamp&);
        static double getDurationInMinutes(const Timestamp&, const Timestamp&);
        static double getDurationInSeconds(const Timestamp&, const Timestamp&);
        int getDateAsInt() const;
        bool operator>(const Timestamp&) const;
        bool operator<(const Timestamp&) const;
        bool operator>=(const Timestamp&) const;
        bool operator<=(const Timestamp&) const;
        bool operator==(const Timestamp&) const;
    };

    time_t createTime(int year, int month, int day, int hour, int minute);
    std::string serializeTime(time_t time);
    time_t deserializeTime(const std::string& timeString);
}