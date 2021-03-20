#pragma once

#include "wat_base.h"
#include "wat_log.h"

#ifdef WAT_DISABLE_LIBRARY
#define WAT_DEF_SCOPED_EVENT(event_name) 
#define WAT_END 
#define WAT_PARAM(value) 
#define WAT_LOG_EVENT_IMPL(text, flags) 
#define WAT_LOG_END
#else
#define WAT_END }
#define WAT_PARAM(value)  cached_item.log_single_param(value);
#define WAT_FUNCTION_NAME(value)  cached_item.log_function_name(value);
#define WAT_DEF_SCOPED_EVENT(event_name) wat::public_event event_name; { wat::log_cached_item cached_item(&event_name, __FUNCTION__); WAT_FUNCTION_NAME(#event_name)
#define WAT_LOG_EVENT_IMPL(text, flags) { wat::public_event log_event; log_event.set_flags(flags); { wat::log_cached_item cached_item(&log_event, __FUNCTION__); WAT_FUNCTION_NAME(text)
#define WAT_LOG_END }}
#endif

#define WAT_LOG_EVENT(text) WAT_LOG_EVENT_IMPL(text, wat::event_flag_just_info)
#define WAT_LOG_SET_THRED_TAG(text) WAT_LOG_EVENT_IMPL(text, wat::event_flag_just_info|wat::event_flag_set_thread_tag)

namespace wat
{

    void lib_init(const std::string & file_name);
    void lib_free();
    void lib_release();

    class lib_auto:wat::noncopyable_t
    {
        bool m_need_free;
    public:
        lib_auto(const std::string & file_name)
            :
                m_need_free(false)
        {
            lib_init(file_name);
            m_need_free = true;
        }
        ~lib_auto()
        {
            if (m_need_free)
            {
                lib_free();
            }
        }
        void release()
        {
            lib_release();
            m_need_free = false;
        }
    };

#define WAT_PUBLIC_EVENT_INITIALIZER_LIST  : m_started(false), m_flags(0)

    class wat_event;
    class public_event:public log_text_processor
    {
        WAT_INTRUSIVE_PTR<wat_event> m_event;
        bool m_started;
        int m_flags;
        virtual void add_text(const def_funcname_string_type & function_name,
                              const wat::ptime_t & timeval, 
                              def_data_string_type & data);
        bool no_library() const;
    public:
        // constructors
#include "wat_public_event_impl.inc"

        ~public_event();
        void set_flags(int flags);
    };

#undef WAT_PUBLIC_EVENT_INITIALIZER_LIST
}