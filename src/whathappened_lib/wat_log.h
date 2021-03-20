#pragma once
#include "wat_base.h"
#include "wat_strings.h"

namespace wat
{
    class log_param
    {
        fixed_string<WAT_MAX_PARAM_SIZE> m_buf;
#ifdef WAT_USE_STD_STRINGS
        std::string m_param;
#endif
    public:
        log_param(const char * pData, int size);
        log_param(const def_param_string_type & str);
        log_param(int value, int radix = 10);
        log_param(unsigned long long value, long radix = 10);
        log_param(bool value);
        log_param(const char * pValue);

        const char * get_begin() const;
        size_t get_size() const;

#ifdef WAT_USE_STD_STRINGS
        std::string to_string() const { return std::string(get_begin(), get_begin() + get_size()); }
#else
        def_param_string_type to_string() const { return m_buf; }
#endif
    };

    template<class Type> 
    struct log_helper
    {
        Type m_value;
        long m_radix;
    public:
        log_helper(Type value, long radix)
            : m_value(value), m_radix(radix)
        {
        }
    };

    template<class Type> 
    inline log_helper<Type> Hex(const Type & obj) { return log_helper<Type>(obj, 16); }

    class log_param_ex:public log_param
    {
    public:
        log_param_ex(const char * pData, int size) : log_param(pData, size)  {   }
        log_param_ex(const std::string & str) : log_param(str)  {   }
        log_param_ex(int value, int radix = 10) : log_param(value, radix)  {   }
        log_param_ex(unsigned long long value) : log_param(value)  {   }
        log_param_ex(bool value) : log_param(value)  {   }
        log_param_ex(const char * pValue) : log_param(pValue)  {   }

#ifndef WAT_USE_STD_STRINGS
        log_param_ex(const def_param_string_type & str) : log_param(str)  {   }
#endif
    };


    struct log_text_processor
    {
        virtual ~log_text_processor(){}
        virtual void add_text(const def_funcname_string_type & function_name,
                              const wat::ptime_t & timeval, 
                              def_data_string_type & data) = 0;
    };

    class log_cached_item
    {
        log_text_processor * m_plog_text_processor;
        def_data_string_type m_data;
        const def_funcname_string_type m_function_name;
    public:
        log_cached_item(log_text_processor * plog_text_processor,
                        const log_param_ex & function_name);
        virtual ~log_cached_item();
        void log_single_param(const char * pData, size_t size);
        void log_single_param(const log_param_ex & param);
        void log_function_name(const log_param_ex & param);
    };


}
