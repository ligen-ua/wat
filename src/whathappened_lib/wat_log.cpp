#include "wat_log.h"

namespace wat
{


const int g_iLogParamMaxBufSize = WAT_MAX_PARAM_SIZE - 1;
log_param::log_param(const char * pData, int size)
{
#ifdef WAT_USE_STD_STRINGS
    try
    {
        if (size > g_iLogParamMaxBufSize)
        {
            m_param.assign(pData, pData + size);
            return;
        }
    }
    catch(std::bad_alloc &)
    {
        // pass
    }
#endif
    m_buf.assign(pData, size);
}
log_param::log_param(const def_param_string_type & str)
{
#ifdef WAT_USE_STD_STRINGS
    try
    {
        if (str.size() > g_iLogParamMaxBufSize)
        {
            m_param.assign(str.c_str(), str.size());
            return;
        }
    }
    catch(std::bad_alloc &)
    {
        // pass
    }
#endif
    m_buf.assign(str.c_str(), str.size());
}
log_param::log_param(const char * pValue)
{
    size_t size = strlen(pValue);

#ifdef WAT_USE_STD_STRINGS
    try
    {
        if (size > g_iLogParamMaxBufSize)
        {
            m_param.assign(pValue, size);
            return;
        }
    }
    catch(std::bad_alloc &)
    {
        // pass
    }
#endif
    m_buf.assign(pValue, size);
}

log_param::log_param(unsigned long long value, long radix)
{
    m_buf.init(value, radix);
}

log_param::log_param(int value, int radix)
{
    m_buf.init(value, radix);
}

log_param::log_param(bool value)
{
    m_buf.init(value);
}

const char * log_param::get_begin() const
{
#ifdef WAT_USE_STD_STRINGS
    if (!m_param.empty())
        return m_param.c_str();
#endif
    return m_buf.c_str();
}
size_t log_param::get_size() const
{
#ifdef WAT_USE_STD_STRINGS
    if (!m_param.empty())
        return m_param.size();
#endif
    return m_buf.size();
}

log_cached_item::log_cached_item(log_text_processor * plog_text_processor,
                                 const log_param_ex & function_name)
    :   
        m_plog_text_processor(plog_text_processor),
            m_function_name(function_name.to_string())
{

#ifdef WAT_USE_STD_STRINGS
    try
    {
        m_data.reserve(WAT_MAX_EVENT_SIZE);
    }
    catch(const std::exception & ex)
    {
        WAT_LIB_LOG(ex);
    }
#endif
        
}
log_cached_item::~log_cached_item()
{
    try
    {
        wat::ptime_t timeval = wat::get_local_time();
        m_plog_text_processor->add_text(m_function_name, timeval, m_data);
    }
    catch(const std::exception & ex)
    {
        WAT_LIB_LOG(ex);
    }
}
void log_cached_item::log_function_name(const log_param_ex & param)
{
    log_single_param(param);
    char buf[] = WAT_PARAM_SEPARATOR_STR;
    m_data.append(WAT_PARAM_SEPARATOR_STR, sizeof(buf)/sizeof(buf[0])-1);
}

void log_cached_item::log_single_param(const log_param_ex & param)
{
    log_single_param(param.get_begin(), param.get_size());
}
static void escape_item(char * it, char * it_end)
{
    for(;
        it_end != it;
        ++it)
    {
        switch (*it)
        {
        case ']':
            *it = '}';    
            continue;
        case '[': 
            *it = '{';    
            continue;
        case 10:
        case 13:
        case WAT_PARAM_SEPARATOR:
            *it = ' ';
            continue;
        }
    }
}
void log_cached_item::log_single_param(const char * pData, size_t size)
{
    try
    {
        int position = (int)m_data.size();
        m_data.append(pData, size);
        escape_item(&m_data[position], &m_data[position + size]);
    }
    catch(const std::exception & ex)
    {
        WAT_LIB_LOG(ex);
    }
}

}
