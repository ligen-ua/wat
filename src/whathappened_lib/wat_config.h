#pragma once

// WAT_DISABLE_LIBRARY
// Disables the library, you can keep the instumentation in code with zero overhead
// #define WAT_DISABLE_LIBRARY

// WAT_FORCE_BOOST
// Forces library to use boost instead of std
//#define WAT_FORCE_BOOST

#if (_MSC_VER < 1910 && __cplusplus<201703L) || defined(WAT_FORCE_BOOST)
#define WAT_BOOST_BASED
#endif

// WAT_DEFAULT_POLL_MILLISECONDS_COUNT
// Defines wait interval of Consumer Thread 
#define WAT_DEFAULT_POLL_MILLISECONDS_COUNT   10
// (see SetPoolWaitMilliseconds also)

// WAT_DEFAULT_EVENT_CACHE_COUNT
// Defines max size of events cache
#define WAT_DEFAULT_EVENT_CACHE_COUNT         20

// WAT_USE_STD_STRINGS
// Enables the use of standard strings
// #define WAT_USE_STD_STRINGS

#define WAT_MAX_FUNCTION_NAME_SIZE            64
#define WAT_MAX_PARAM_SIZE                    64
#define WAT_MAX_EVENT_SIZE                  1024

#if __cplusplus < 201103L 
#define WAT_AUTO_PTR std::auto_ptr
#else
#define WAT_AUTO_PTR std::unique_ptr
#endif


#ifdef _WIN32


#ifdef WAT_BOOST_BASED
#define WAT_THREAD_ID_TYPE   boost::thread::id
#else
#define WAT_THREAD_ID_TYPE   unsigned int
#endif

#else

#ifdef __APPLE__

#define WAT_THREAD_ID_TYPE   uint64_t

#else

// Linux

#define WAT_THREAD_ID_TYPE   pid_t
#endif
#endif



// WAT_LIB_LOG: the integration with the other log system:
#define WAT_LIB_LOG(ex) &ex;


// Out config
#define WAT_PARAM_SEPARATOR '|'
#define WAT_PARAM_SEPARATOR_STR "|"

// Platform specific
#define WAT_PLATFORM_ENDLN "\n"


