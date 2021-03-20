#include "wat_data_analyzer.h"
#include "wat_reporter.h"
namespace wat
{

data_analyzer::data_analyzer(const wat::ptime_t & start_time,
                             int seconds,
                             const std::string & report_file)
    :
        m_start_time(start_time),
        m_seconds(seconds),
        m_report_file(report_file)
{
}
data_analyzer::~data_analyzer()
{
}
bool data_analyzer::query_thread(const std::string & thread_id, bool force, threads_map_type::iterator * pit)
{
    if (force)
    {
        *pit = m_threads.insert(std::make_pair(thread_id, thread_info())).first;
        return true;
    }
    threads_map_type::iterator it = m_threads.find(thread_id);
    return it != m_threads.end();
}
void data_analyzer::report_call(call_info & info)
{
    // message id
    m_buffer.clear();
    m_buffer.append("       ");
    char buffer[64];
    {
        std::stringstream res_stream;
        buffer[0]=0;
        sprintf(buffer, 
                "[%08llx]",
                info.m_info.message_id);
        m_buffer.append(buffer);
    }

    {
        std::string string_time;
        time_to_string(info.m_info.timeval, &string_time);
        m_buffer.append(string_time);
    }
    m_buffer.append(" [");
    m_buffer.append(info.m_info.function);
    if (!info.m_info.parameters.empty())
    {
        m_buffer.append(WAT_PARAM_SEPARATOR_STR);
        m_buffer.append(info.m_info.parameters);
    }
    m_buffer.append("] ");
    {
        // add duration
        wat::time_duration_t msdiff = m_start_time - info.m_info.timeval;
        long long total_milliseconds = msdiff.total_milliseconds();
        std::stringstream res_stream;
        res_stream<<"["<<total_milliseconds<<"]";
        m_buffer.append(res_stream.str());
    }
    m_buffer.append(WAT_PLATFORM_ENDLN);
}
void data_analyzer::report_once()
{
    std::ofstream res_stream(m_report_file.c_str(), std::ios_base::out);

    {
        std::string string_time;
        time_to_string(m_start_time, &string_time);
        res_stream<<"REPORT AT "<<string_time<<WAT_PLATFORM_ENDLN<<WAT_PLATFORM_ENDLN;
    }

    for(threads_map_type::iterator it = m_threads.begin(), it_end = m_threads.end();
                                   it != it_end;
                                   ++it)
    {
        res_stream<<"THREAD "<<it->first<<WAT_PLATFORM_ENDLN;
        line_info & tag = it->second.m_tag;
        if (!tag.function.empty())
        {
            res_stream<<"[";
            res_stream<<tag.function;
            if (!tag.parameters.empty())
            {
                res_stream<<WAT_PARAM_SEPARATOR_STR;
                res_stream<<tag.parameters;
            }
            res_stream<<"] "<<WAT_PLATFORM_ENDLN;
        }
        calls_vec_type & calls = it->second.m_calls;
        for(calls_vec_type::reverse_iterator cit = calls.rbegin(), cit_end = calls.rend();
                                             cit != cit_end;
                                             ++cit)
        {
            report_call(*cit);
            res_stream<<m_buffer;
        }
        res_stream<<WAT_PLATFORM_ENDLN;
    }
}
bool data_analyzer::process_info(line_info & line)
{
    threads_map_type::iterator thread_it;
    query_thread(line.thread_id, true, &thread_it);

    if (line.type == line_info::lt_thread_tag)
    {
        thread_it->second.m_tag = line;
        return true;
    }

    if ((line.timeval + milliseconds_t(1))> m_start_time)
    {
        report_once();
        return false;
    }

    if (line.type == line_info::lt_start)
    {
        thread_it->second.m_calls.push_back(line);
        return true;
    }

    if (line.type == line_info::lt_stop)
    {
        if (!thread_it->second.m_calls.empty())
        {
            call_info & info = thread_it->second.m_calls.back();
            if (info.m_info.message_id != line.message_id)
            {
                throw wat_error("Invalid call sequence");
            }
            thread_it->second.m_calls.pop_back();
        }
    }
    return true;
}

}