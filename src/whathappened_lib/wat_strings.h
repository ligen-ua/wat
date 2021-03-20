#pragma once
#include "wat_base.h"

namespace wat
{

template<int c_max_size>
class fixed_string
{
    char m_data[c_max_size];
    int m_current_size;
public:
    fixed_string()
        :
            m_current_size(0)
    {
    }
    template<int c_other_max_size>
    fixed_string(fixed_string<c_other_max_size> & other)
    {
        assign(other.m_data, other.m_current_size);
    }
    fixed_string(const char * data)
    {
        size_t size = strlen(data);
        assign(data, size);
    }
    fixed_string(const std::string & data)
    {
        assign(data.c_str(), data.size());
    }
    char & operator [] (size_t pos)
    {
        return m_data[pos];
    }
    const char & operator [] (size_t pos) const 
    {
        return m_data[pos];
    }
    void append(const char * data, size_t size)
    {
        int free_size = c_max_size - 1 - m_current_size;
        int size_to_append = std::min(free_size, (int)size);
        memcpy(m_data + m_current_size, data, size_to_append);
        m_current_size += size_to_append;
        m_data[m_current_size] = 0;
    }
    bool empty() const
    {
        return m_current_size == 0;
    }
    void clear()
    {
        m_current_size = 0;
    }
    const char * c_str() const
    {
        return m_data;
    }
    size_t size() const
    {
        return m_current_size;
    }
    void init(unsigned long long value, long radix)
    {
        if (radix == 16)
        {
            m_current_size = sprintf(m_data, "%llx",value);
            return;
        }
        m_current_size = sprintf(m_data, "%llu",value);
    }
    void init(int value, long radix)
    {
        if (radix == 16)
        {
            m_current_size = sprintf(m_data, "%x",value);
            return;
        }
        m_current_size = sprintf(m_data, "%i",value);
    }
    void init(bool value)
    {
        if (value)
        {
            strcpy(m_data, "true");
            m_current_size = 4;
        }
        else
        {
            strcpy(m_data, "false");
            m_current_size = 5;
        }
    }
    void assign(const char * data, size_t size)
    {
        int common_size = std::min(c_max_size-1, (int)size);
        memcpy(m_data, data, common_size);
        m_data[common_size] = 0;
        m_current_size = common_size;
    }
    template<int c_other_max_size>
    void move_from(fixed_string<c_other_max_size> & other)
    {
        assign(other.m_data, other.m_current_size);
    }
};

#ifdef WAT_USE_STD_STRINGS
typedef std::string def_data_string_type;
typedef std::string def_funcname_string_type;
typedef std::string def_param_string_type;

#define WAT_MOVE(X, Y) X.swap(Y);

#else
typedef fixed_string<WAT_MAX_EVENT_SIZE>         def_data_string_type;
typedef fixed_string<WAT_MAX_FUNCTION_NAME_SIZE> def_funcname_string_type;
typedef fixed_string<WAT_MAX_PARAM_SIZE>         def_param_string_type;

#define WAT_MOVE(X, Y) X.move_from(Y);
#endif
}