WAT Library

1. How to instrument your code:

Init the library and arrange scoped and other events to the code:

        #nclude "wat_lib.h"
        
        // how to init the lib
        wat::lib_auto lib(file_name_hint);

        // version 1
        // declare scoped event
        WAT_DEF_SCOPED_EVENT(evt2)
                WAT_PARAM("number=") 
                WAT_PARAM((unsigned long long)42)
            WAT_END;

        // version 2
        wat::public_event evt(__FUNCTION__, "number=",1);


See do_selftest function from the sample how to fire the events:

    src\whathappened\main.cpp

2. How to disable the instumentation or to configure the library:

See:
     src\whathappened_lib\wat_config.h

3. How to analyze the logs

Run:

     whathappened <report file> <log file> <yyyy/mm/dd hh:mm:ss:SSS>;