# WAT Library

Yet another C++ logging library for troubleshooting purposes.

## Currently supports: 
* Windows, Linux, MacOS
* Windows: VS 2008+
* Can use BOOST or cxx17 (see whathappened_lib/wat_config.h)

## How to use:

### 1. Instrument your code:

Init the library and use scoped and other events in the code:


```
        #include "wat_lib.h"
        
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
```

See do_selftest function from the sample how to fire the events:

    src\whathappened\main.cpp

### 2. How to disable the instumentation or to configure the library:

See:
     src\whathappened_lib\wat_config.h

### 3. How to analyze the logs

Run:

     whathappened /report <report file> <log file> <yyyy/mm/dd hh:mm:ss:SSS>

It shows the state of the thread on this point:
```
REPORT AT [2021/03/21 13:18:00:060]

THREAD 1ae2
[do_selftest|pool thread|very important]
       [00000002][2021/03/21 13:17:59:960]  [do_selftest|evt2|number=2] [100]
       [00000001][2021/03/21 13:17:59:960]  [do_selftest|number=1] [100]
```