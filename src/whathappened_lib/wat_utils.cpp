#include "wat_internal_base.h"

namespace wat
{
#ifdef WAT_BOOST_BASED
#else

void split(std::vector<std::string> & ret,
    const std::string& str,
    const std::string& sep)
{
    ret.clear();
    for (size_t pos = 0; pos <= str.size();)
    {
        size_t new_pos = str.find(sep, pos);
        if (std::string::npos == new_pos)
            new_pos = str.size();
        ret.push_back(str.substr(pos, new_pos - pos));
        pos = new_pos + sep.size();
    }
}

ptime::ptime(std::chrono::system_clock::time_point std_time)
    :
        m_std_time(std_time)
{

}

ptime ptime_create(int dateval_year,
    int dateval_month,
    int dateval_day,
    int timeOfDay_hours,
    int timeOfDay_minutes,
    int timeOfDay_seconds,
    int timeOfDay_milliseconds)
{
    tm local_time = { 0 };
    local_time.tm_sec = timeOfDay_seconds;
    local_time.tm_min = timeOfDay_minutes;
    local_time.tm_hour = timeOfDay_hours;
    local_time.tm_mday = dateval_day;
    local_time.tm_mon = dateval_month-1;
    local_time.tm_year = dateval_year - 1900;
    local_time.tm_wday = 0;  // days since Sunday - [0, 6]
    local_time.tm_yday = 0;  // days since January 1 - [0, 365]
    local_time.tm_isdst = 0; // daylight savings time flag

    time_t posix_local_time = mktime(&local_time);
    std::chrono::system_clock::time_point std_time = std::chrono::system_clock::from_time_t(posix_local_time);
    return std_time + std::chrono::milliseconds(timeOfDay_milliseconds);
}

void time_to_string(const ptime & timeval, std::string * pstring)
{
    std::time_t time = std::chrono::system_clock::to_time_t(timeval.get());
    tm tm = { 0 };

#ifdef _WIN    
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    std::chrono::system_clock::time_point time_without_ms= std::chrono::system_clock::from_time_t(time);
    int milliseconds = (int)std::chrono::duration_cast<std::chrono::milliseconds>(timeval.get() - time_without_ms).count();

    char buffer[64];
    pstring->clear();
    buffer[0] = 0;
    sprintf(buffer,
        "[%4i/%02i/%02i %02i:%02i:%02i:%03i] ",
        (int)tm.tm_year + 1900,
        (int)tm.tm_mon + 1,
        (int)tm.tm_mday,
        (int)tm.tm_hour,
        (int)tm.tm_min,
        (int)tm.tm_sec,
        (int)milliseconds);
    pstring->append(buffer);
}
bool operator > (const ptime & time1, const ptime & time2)
{
    return time1.get() > time2.get();
}

time_duration::time_duration(def_duration_t duration)
    :
        m_duration(duration)
{
}
long long time_duration::total_milliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
}
time_duration operator - (const ptime & time1, const ptime & time2)
{
    def_duration_t diff = time1.get() - time2.get();
    return diff;
}
ptime get_local_time()
{
    return ptime(std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()));
}

#endif
}
