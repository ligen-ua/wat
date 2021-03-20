#pragma once
#include "wat_internal_base.h"

namespace wat
{
struct line_info
{
    typedef enum {lt_unknown, lt_start, lt_stop, lt_thread_tag} line_type;
    line_type type;
    wat::ptime_t timeval;
    std::string function;
    std::string parameters;
    long long message_id;
    std::string thread_id;
    line_info()
        :
            type(lt_unknown),
            message_id(0)
    {
    }
};
class data_analyzer
{
    wat::ptime_t m_start_time;
    wat::seconds_t m_seconds;
    std::string m_report_file;
    std::string m_buffer;

    struct call_info
    {
        line_info m_info;
        call_info(const line_info & info)
            :   
                m_info(info)
        {
        }
    };
    typedef std::vector<call_info> calls_vec_type;
    struct thread_info
    {
        calls_vec_type m_calls;
        line_info m_tag;
    };
    typedef std::map<std::string, thread_info> threads_map_type;

    threads_map_type m_threads;

    bool query_thread(const std::string & thread_id, bool force, threads_map_type::iterator * pit);
    void report_call(call_info & info);
public:
    data_analyzer(const wat::ptime_t & start_time,
                   int seconds,
                   const std::string & report_file);
    ~data_analyzer();
    bool process_info(line_info & line);
    void report_once();
};

}