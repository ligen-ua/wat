#pragma once

namespace wat
{

// try GCC
template<class Type>
Type * compare_and_swap(Type ** ppDestination, Type * pNewData, Type * pComparand)
{
    // returns old value from *ppDestination
    return (Type *)__sync_val_compare_and_swap((void **)ppDestination,  (void *)pComparand, (void *)pNewData); 
}


template<class Type>
Type * exchange(Type ** data, Type * value)
{
    intptr_t * data2 = (intptr_t *)data;
    intptr_t value2 = (intptr_t)value;
    Type * pRes =  (Type *)__sync_lock_test_and_set (data2, value2);
    __sync_synchronize();
    return pRes;
}


}

