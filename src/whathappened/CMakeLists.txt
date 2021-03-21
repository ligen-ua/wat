project(whathappened)

include_directories("../whathappened_lib")

target_link_libraries(whathappened_lib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} pthread whathappened_lib)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

