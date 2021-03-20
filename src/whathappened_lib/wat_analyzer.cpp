#include "wat_analyzer.h"
#include "wat_reporter.h"

namespace wat
{

analyzer::analyzer(const std::string & out_file,
                   const std::string & file_hint,
                   const wat::ptime_t & start_time,
                   int seconds)
    :
        m_file_hint(file_hint),
        m_data_analyzer(start_time, seconds, out_file),
        m_buffer_size(0)
{
}
void analyzer::parse_common(line_info::line_type line_type, line_info * pline, const char *& pblock_start, const char *& pblock_end)
{
    pline->type = line_type;
    pline->parameters.clear();
    pline->message_id = 0;
    pline->thread_id.clear();
    pline->function.clear();

    const char * pbuffer = &m_buffer.front();
    const char * pbuffer_end = pbuffer + m_buffer_size;

    // prepare vars
    pblock_start = pbuffer + 1;
    pblock_end = pbuffer_end;

    // read first block
    read_next_block(pblock_start, pblock_end, &m_tmp);
    size_t position = 0;
    if (!read_until(m_tmp, WAT_PARAM_SEPARATOR, position, true, &pline->message_id))
        throw wat_error("Invalid message id");
    if (!read_until(m_tmp, 0, position, &pline->thread_id))
        throw wat_error("Invalid message id");

    // read date
    read_next_block(pblock_start, pblock_end, &m_tmp);
    if (!time_from_string(m_tmp, &pline->timeval))
        throw wat_error("Can't parse date");

}

void analyzer::read_next_block(const char *& pdate_start, const char *& pdate_end, std::string * presult)
{
    const char * pbuffer = &m_buffer.front();
    const char * pbuffer_end = pbuffer + m_buffer_size;
    pdate_start = std::find(pdate_start, pbuffer_end, '[');
    if (pdate_start == pbuffer_end)
        throw wat_error("Can't find date");
    ++pdate_start;

    pdate_end = std::find(pdate_start, pbuffer_end, ']');
    if (pdate_end == pbuffer_end)
        throw wat_error("Can't find the end of the date");

    presult->assign(pdate_start, pdate_end);
}
void analyzer::parse_start_line(line_info * pline)
{
    const char * pblock_start = 0, * pblock_end = 0;
    parse_common(line_info::lt_start, pline, pblock_start, pblock_end);
    // read function
    read_next_block(pblock_start, pblock_end, &m_tmp);
    size_t position = 0;
    if (read_until(m_tmp, WAT_PARAM_SEPARATOR, position, &pline->function))
    {
        if (!read_until(m_tmp, 0, position, &pline->parameters))
            throw wat_error("Invalid message id");
    }
    else
    {
        if (!read_until(m_tmp, 0, position, &pline->function))
            throw wat_error("Invalid message id");
    }
}
void analyzer::parse_thread_tag(line_info * pline)
{
    parse_start_line(pline);
    pline->type = line_info::lt_thread_tag;
}

void analyzer::parse_stop_line(line_info * pline)
{
    const char * pblock_start = 0, * pblock_end = 0;
    parse_common(line_info::lt_stop, pline, pblock_start, pblock_end);
    size_t position = 0;
    read_next_block(pblock_start, pblock_end, &m_tmp);
    if (!read_until(m_tmp, 0, position, &pline->function))
        throw wat_error("Invalid message id");
}

bool analyzer::parse_line(int line_number, line_info * pline)
{
    try
    {
        switch(m_buffer[0])
        {
        case '+':
        {
            parse_start_line(pline);
            return true;
        }
        case '-':
        {
            parse_stop_line(pline);
            return true;
        }
        case 'T':
        {
            parse_thread_tag(pline);
            return true;
        }
        }
        return false;
    }
    catch(const std::exception & ex)
    {
        std::stringstream result;
        result<<"Error: ["<<ex.what()<<"] file: ["<<m_current_filename<<"], line "<<line_number;
        throw wat_error(result.str());
    }
}
bool analyzer::try_scan(const std::string & file_name)
{
    m_current_filename = file_name;
    std::ifstream file(file_name.c_str(), std::ios_base::in);
    if (!file)
        return false;

    const int page_size = 1024*4;
    m_buffer.resize(page_size);
    
    line_info line;
    int line_pos = 1;
    for(;;++line_pos)
    {
        if (!file.getline(&m_buffer.front(), page_size))
        {
            break;
        }
        if (m_buffer[0] == 0 || m_buffer[0] == 10 || m_buffer[0] == 13)
        {
            continue;
        }
        m_buffer_size = (int)strlen(&m_buffer.front());
        if (parse_line(line_pos, &line))
        {
            if (!m_data_analyzer.process_info(line))
            {
                return false;
            }
        }
    }
    m_data_analyzer.report_once();
    return true;
}
void analyzer::do_analysis()
{
    std::vector<std::string> parts;
#ifdef WAT_BOOST_BASED
    boost::split(parts, m_file_hint, boost::is_any_of("."));
#else
    wat::split(parts, m_file_hint, ".");
#endif
    int last_value = 0;
    switch(parts.size())
    {
    default:
        throw wat_error("Incorrect file name, should have two dots");
    case 3:
        {
            size_t position = 0;
            if (!read_until(parts[1], 0, position, true, &last_value))
            {
                throw wat_error("Incorrect file name, can't parse digits");
            }
        }
    case 2:;
    }

    std::string pattern = parts[0];
    for(;;)
    {
        std::string file_name = generate_new_name(pattern, last_value);

        if (!try_scan(file_name))
            return;

        ++last_value;
    }
}

}