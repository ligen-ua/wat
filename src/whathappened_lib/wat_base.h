#pragma once

#ifdef _MSC_VER
#pragma warning(disable:4996)
#define _WIN32_WINNT 0x0501
#endif 


#include "wat_config.h"


#ifdef WAT_BOOST_BASED

#include "boost/noncopyable.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/intrusive_ptr.hpp"
#include "boost/atomic.hpp"

namespace wat
{
    typedef boost::posix_time::ptime ptime_t;
    typedef boost::noncopyable noncopyable_t;
    typedef boost::posix_time::seconds seconds_t;
    typedef boost::posix_time::milliseconds milliseconds_t;
    typedef boost::posix_time::time_duration time_duration_t;

#define WAT_SHARED_PTR boost::shared_ptr
#define WAT_ATOMIC boost::atomic
#define WAT_INTRUSIVE_PTR boost::intrusive_ptr
#define WAT_FUNCTION boost::function
#define WAT_EXBOOST_NS boost
#define WAT_FS_NS boost::filesystem
#define WAT_FS_ERROR boost::system::error_code

    void sleep_for_milliseconds(boost::int_least64_t ms_to_wait);
    wat::ptime_t get_local_time();
    wat::ptime_t ptime_create(int dateval_year,
        int dateval_month,
        int dateval_day,
        int timeOfDay_hours,
        int timeOfDay_minutes,
        int timeOfDay_seconds,
        int timeOfDay_milliseconds);

}
#else

#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <functional>
#include <thread>
#include <map>
#include <vector>
#include <atomic>
#include <algorithm>

#include "wat_utils.h"

#pragma once


#ifdef __cpp_lib_experimental_filesystem
#define WAT_FS_NS std::experimental::filesystem
#else
#define WAT_FS_NS std::filesystem
#endif
#define WAT_FS_ERROR std::error_code

#define WAT_EXBOOST_NS std
#define WAT_SHARED_PTR std::shared_ptr
#define WAT_ATOMIC std::atomic
#define WAT_INTRUSIVE_PTR wat::intrusive_ptr
#define WAT_FUNCTION std::function

namespace wat
{
    inline void sleep_for_milliseconds(long long ms_to_wait)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_to_wait));
    }

    typedef std::chrono::seconds seconds_t;
    typedef std::chrono::milliseconds milliseconds_t;
}
#endif
namespace wat
{

class wat_error:public std::runtime_error
{
public:
    wat_error(const std::string & text)
        :
            std::runtime_error(text)
    {
    }
};


// event flags
static const int event_flag_just_info = 1;
static const int event_flag_set_thread_tag = 2;

}

