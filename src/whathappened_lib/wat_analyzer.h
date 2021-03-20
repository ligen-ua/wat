#pragma once
#include "wat_data_analyzer.h"

namespace wat
{

class analyzer
{
    std::string m_file_hint;
    int m_buffer_size;
    std::vector<char> m_buffer;
    std::string m_current_filename, m_tmp;
    data_analyzer m_data_analyzer;

    bool try_scan(const std::string & file_name);
    bool parse_line(int line_number, line_info * pline);
    void parse_common(line_info::line_type line_type, line_info * pline, const char *& pblock_start, const char *& pblock_end);
    void parse_start_line(line_info * pline);
    void parse_stop_line(line_info * pline);
    void parse_thread_tag(line_info * pline);

    void read_next_block(const char *& pdate_start, const char *& pdate_end, std::string * presult);
public:
    analyzer(const std::string & out_file,
             const std::string & file_hint,
             const wat::ptime_t & start_time,
             int seconds);
    void do_analysis();
};

}