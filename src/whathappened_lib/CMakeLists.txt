project(whathappened_lib)

if(WIN32)
endif()

if(LINUX) 
list(APPEND SYS_FILES_LIST wat_internal_base_linux.cpp)
endif()

if(APPLE)
list(APPEND SYS_FILES_LIST wat_internal_base_apple.cpp)
endif()

# Create shared library
add_library(${PROJECT_NAME} SHARED 
            wat_analyzer.cpp
            wat_data_analyzer.cpp
            wat_event_cache.cpp
            wat_hub.cpp
            wat_internal_base_boost.cpp            
            wat_lib.cpp
            wat_log.cpp
            wat_reporter.cpp
            wat_utils.cpp
            ${SYS_FILES_LIST}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

