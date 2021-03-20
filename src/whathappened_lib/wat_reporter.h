#pragma once

#include "wat_hub.h"

namespace wat
{

class wat_event;
class wat_reporter:wat::noncopyable_t
{
    bool generic_write(const std::string & text);
    void report_action(WAT_INTRUSIVE_PTR<wat_event> pevent,
                       const wat::ptime_t & timeval,
                       const char * prefix,
                       bool calc_duration);

    int m_current_number;
    std::string m_base_name;
    std::ofstream m_stream;

    std::string m_buffer;
public:
    wat_reporter(const std::string & file_name);
    ~wat_reporter();

    void report_info(WAT_INTRUSIVE_PTR<wat_event> pevent);
    void report_start(WAT_INTRUSIVE_PTR<wat_event> pevent);
    void report_stop(WAT_INTRUSIVE_PTR<wat_event> pevent);
    void report_thread_tag(WAT_INTRUSIVE_PTR<wat_event> pevent);
};

bool time_from_string(const std::string & str, wat::ptime_t * ptimeval);
void time_to_string(const wat::ptime_t & timeval, std::string * pstring);
bool read_until(const std::string & str, char character, size_t & position, std::string * presult);
bool read_until(const std::string & str, char character, size_t & position, bool hex, int * presult);
bool read_until(const std::string & str, char character, size_t & position, bool hex, long long * presult);
std::string generate_new_name(const std::string & base, int num);

}