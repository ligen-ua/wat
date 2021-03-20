#include "wat_reporter.h"
#include "wat_hub.h"

namespace wat
{

const int g_max_files_count = 5;
const int g_max_file_size = 64*1024*1024;


std::string generate_new_name(const std::string & base, int num)
{
    std::stringstream result;
    result<<base<<".";
    result<<std::setfill('0');
    result<<std::setw(3)<<num;
    result<<".txt";
    return result.str();
}

wat_reporter::wat_reporter(const std::string & file_name)
    :   
        m_current_number(0),
        m_base_name(file_name),
        m_stream(generate_new_name(m_base_name, 0).c_str(), std::ios_base::out)
{
}
wat_reporter::~wat_reporter()
{
}
void wat_reporter::report_thread_tag(WAT_INTRUSIVE_PTR<wat_event> pevent)
{
    report_action(pevent, 
                  pevent->get_start_time(), 
                  "T",
                  false);
}
void wat_reporter::report_info(WAT_INTRUSIVE_PTR<wat_event> pevent)
{
    report_action(pevent, 
                  pevent->get_start_time(), 
                  " ",
                  false);
}
void wat_reporter::report_start(WAT_INTRUSIVE_PTR<wat_event> pevent)
{
    report_action(pevent, 
                  pevent->get_start_time(), 
                  "+",
                  false);
}
void wat_reporter::report_stop(WAT_INTRUSIVE_PTR<wat_event> pevent)
{
    report_action(pevent, 
                  pevent->get_stop_time(), 
                  "-",
                  true);
}

bool wat_reporter::generic_write(const std::string & text)
{
    if (m_stream.fail())
    {
        m_stream.open(generate_new_name(m_base_name, 0).c_str());
        if (m_stream.fail())
        {
            return false;
        }
    }
    int currpos = (int)m_stream.tellp();
    if (currpos > g_max_file_size)
    {
        m_stream.close();
        ++m_current_number;
        m_stream.open(generate_new_name(m_base_name, m_current_number).c_str());
        if (m_current_number >= g_max_files_count)
        {
            std::string old_name = generate_new_name(m_base_name, m_current_number - g_max_files_count);
            WAT_FS_ERROR ec;
            WAT_FS_NS::remove(old_name, ec);
        }
    }

    m_stream<<text;
    return true;
}


#ifdef WAT_BOOST_BASED
void time_to_string(const wat::ptime_t & timeval, std::string * pstring)
{
    char buffer[64];
    boost::gregorian::date dateval = timeval.date();
    boost::posix_time::microsec_clock::time_duration_type timeOfDay = timeval.time_of_day();
    pstring->clear();
    int digits_count = timeOfDay.num_fractional_digits();
    int fractional_seconds = (int)timeOfDay.fractional_seconds();
    int milliseconds = fractional_seconds;
    for(int i = 3; i < digits_count; ++i)
    {
        milliseconds/=10;
    }
    buffer[0]=0;
    sprintf(buffer, 
    "[%4i/%02i/%02i %02i:%02i:%02i:%03i] ",
    (int)dateval.year(),
    (int)dateval.month(),
    (int)dateval.day(),
    (int)timeOfDay.hours(),
    (int)timeOfDay.minutes(),
    (int)timeOfDay.seconds(),
    (int)milliseconds);
    pstring->append(buffer);
}
#endif
void wat_reporter::report_action(WAT_INTRUSIVE_PTR<wat_event> pevent,
                                 const wat::ptime_t & timeval,
                                 const char * prefix,
                                 bool calc_duration)
{
    m_buffer.clear();

    // pack type
    m_buffer.append(prefix);
    // message id
    char buffer[64];
    {
        std::stringstream res_stream;
        buffer[0]=0;
        sprintf(buffer, 
                "[%08llx" WAT_PARAM_SEPARATOR_STR,
                pevent->get_message_id());
        std::hex(res_stream);
        res_stream<<buffer;
        res_stream<<pevent->get_thread_id();
        res_stream<<"] ";
        m_buffer.append(res_stream.str());
    }

    {
        std::string string_time;
        time_to_string(timeval, &string_time);
        m_buffer.append(string_time);
    }
    m_buffer.append(" [");

    const def_funcname_string_type & fnc_name = pevent->get_function_name();
    m_buffer.append(fnc_name.c_str(), fnc_name.size());
    if (!pevent->get_data().empty())
    {
        m_buffer.append(WAT_PARAM_SEPARATOR_STR);

        const def_data_string_type & data = pevent->get_data();
        m_buffer.append(data.c_str(), data.size());
    }
    m_buffer.append("] ");
    {
        // add duration
        if (calc_duration)
        {
            wat::time_duration_t msdiff = pevent->get_stop_time() - pevent->get_start_time();
            long long total_milliseconds = msdiff.total_milliseconds();
            std::stringstream res_stream;
            res_stream<<"["<<total_milliseconds<<"]";
            m_buffer.append(res_stream.str());
        }
    }
    m_buffer.append(WAT_PLATFORM_ENDLN);
    generic_write(m_buffer);
}


bool read_until(const std::string & str, char character, size_t & position, std::string * presult)
{
    if (position >= str.size())
        return false;

    size_t newposition = 0;
    if (character)
    {
        newposition = str.find(character, position);
        if (newposition == str.npos)
            return false;
    }
    else
    {
        newposition = str.size();
    }

    presult->assign(str.begin() + position, str.begin() + newposition);
    position = newposition + 1;
    return true;
}

template<class result_type>
bool read_until_impl(const std::string & str, char character, size_t & position, bool hex, result_type * presult)
{
    std::string value;
    size_t newposition = position;
    if (!read_until(str, character, newposition, &value))
    {
        return false;
    }
    // parse
    std::stringstream res;
    res<<value;
    if (hex)
    {
        std::hex(res);
    }
    res>>*presult;
    if (res.fail())
    {
        return false;
    }
    position = newposition;
    return true;
}
bool read_until(const std::string & str, char character, size_t & position, bool hex, int * presult)
{
    return read_until_impl(str, character, position, hex, presult);
}
bool read_until(const std::string & str, char character, size_t & position, bool hex, long long * presult)
{
    return read_until_impl(str, character, position, hex, presult);
}

bool time_from_string(const std::string & str, wat::ptime_t * ptimeval)
{
    int dateval_year = 0,
        dateval_month = 0,
        dateval_day = 0,
        timeOfDay_hours = 0,
        timeOfDay_minutes = 0,
        timeOfDay_seconds = 0,
        timeOfDay_milliseconds = 0;

    size_t position = 0;
    // "%i/%i/%i %i:%i:%i:%i"
    if (!read_until(str, '/', position, false, &dateval_year))
        return false;
    if (!read_until(str, '/', position, false, &dateval_month))
        return false;
    if (!read_until(str, ' ', position, false, &dateval_day))
        return false;
    if (!read_until(str, ':', position, false, &timeOfDay_hours))
        return false;
    if (!read_until(str, ':', position, false, &timeOfDay_minutes))
        return false;
    if (!read_until(str, ':', position, false, &timeOfDay_seconds))
        return false;
    if (!read_until(str, 0, position, false, &timeOfDay_milliseconds))
        return false;

    
    *ptimeval = wat::ptime_create(dateval_year, dateval_month, dateval_day,
        timeOfDay_hours,
        timeOfDay_minutes,
        timeOfDay_seconds,
        timeOfDay_milliseconds);
    return true;
    
}


}