#include "wat_internal_base.h"
namespace wat
{
#ifdef WAT_BOOST_BASED

thread_id_type get_current_thread_id()
{
    return boost::this_thread::get_id();
}

void sleep_for_milliseconds(boost::int_least64_t ms_to_wait)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ms_to_wait));
}

wat::ptime_t get_local_time()
{
    return boost::posix_time::microsec_clock::local_time();
}
wat::ptime_t ptime_create(int dateval_year,
    int dateval_month,
    int dateval_day,
    int timeOfDay_hours,
    int timeOfDay_minutes,
    int timeOfDay_seconds,
    int timeOfDay_milliseconds)
{
    return wat::ptime_t(boost::gregorian::date(dateval_year, dateval_month, dateval_day),
        boost::posix_time::hours(timeOfDay_hours) +
        boost::posix_time::minutes(timeOfDay_minutes) +
        wat::seconds_t(timeOfDay_seconds) +
        boost::posix_time::milliseconds(timeOfDay_milliseconds)
    );
}

#else
#ifdef _WIN32
thread_id_type get_current_thread_id()
{
    return ::GetCurrentThreadId();
}
#endif
#endif
}