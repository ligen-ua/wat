#include "wat_internal_base.h"
#include <unistd.h>
#include <sys/types.h>
namespace wat
{

thread_id_type get_current_thread_id()
{
    return gettid();
}

}