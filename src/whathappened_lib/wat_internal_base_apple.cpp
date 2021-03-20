#include "wat_internal_base.h"
namespace wat
{

thread_id_type get_current_thread_id()
{
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return tid;
}

}