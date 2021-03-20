#pragma once

#include "wat_base.h"

#ifdef WAT_BOOST_BASED

// go with boost
#include "boost/thread.hpp"
#include "boost/atomic.hpp"
#include "boost/filesystem.hpp"
#include "boost/intrusive_ptr.hpp"
#include "boost/algorithm/string.hpp"

#else

#include <chrono>
#include <atomic>   
#include <filesystem>
#include <thread>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "wat_utils.h"
#endif

#include "wat_list_and_locks.h"
#include "wat_strings.h"

namespace wat
{

typedef WAT_THREAD_ID_TYPE  thread_id_type;
thread_id_type  get_current_thread_id();

}