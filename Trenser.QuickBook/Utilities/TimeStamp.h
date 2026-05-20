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
}