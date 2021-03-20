#define WAT_CHECK_LIBRARY if (no_library()) return 

     public_event(const log_param_ex & function_name)
     
		WAT_PUBLIC_EVENT_INITIALIZER_LIST
    {
		WAT_CHECK_LIBRARY;
        log_cached_item cached_item(this, function_name);
    }
     public_event(const log_param_ex & function_name,
                      const log_param_ex & param)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST
    {
		WAT_CHECK_LIBRARY;
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param.get_begin(), param.get_size());
    }
     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
    }
     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
    }    
     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
    }
     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
    }
     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5,
                      const log_param_ex & param6)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
        cached_item.log_single_param(param6.get_begin(), param6.get_size());
    }

     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5,
                      const log_param_ex & param6,
                      const log_param_ex & param7)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
        cached_item.log_single_param(param6.get_begin(), param6.get_size());
        cached_item.log_single_param(param7.get_begin(), param7.get_size());
    }

     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5,
                      const log_param_ex & param6,
                      const log_param_ex & param7,
                      const log_param_ex & param8)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
        cached_item.log_single_param(param6.get_begin(), param6.get_size());
        cached_item.log_single_param(param7.get_begin(), param7.get_size());
        cached_item.log_single_param(param8.get_begin(), param8.get_size());
    }

     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5,
                      const log_param_ex & param6,
                      const log_param_ex & param7,
                      const log_param_ex & param8,
                      const log_param_ex & param9)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
        cached_item.log_single_param(param6.get_begin(), param6.get_size());
        cached_item.log_single_param(param7.get_begin(), param7.get_size());
        cached_item.log_single_param(param8.get_begin(), param8.get_size());
        cached_item.log_single_param(param9.get_begin(), param9.get_size());
    }

     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5,
                      const log_param_ex & param6,
                      const log_param_ex & param7,
                      const log_param_ex & param8,
                      const log_param_ex & param9,
                      const log_param_ex & param10)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
        cached_item.log_single_param(param6.get_begin(), param6.get_size());
        cached_item.log_single_param(param7.get_begin(), param7.get_size());
        cached_item.log_single_param(param8.get_begin(), param8.get_size());
        cached_item.log_single_param(param9.get_begin(), param9.get_size());
        cached_item.log_single_param(param10.get_begin(), param10.get_size());
    }

     public_event(const log_param_ex & function_name,
                      const log_param_ex & param1, 
                      const log_param_ex & param2,
                      const log_param_ex & param3,
                      const log_param_ex & param4,
                      const log_param_ex & param5,
                      const log_param_ex & param6,
                      const log_param_ex & param7,
                      const log_param_ex & param8,
                      const log_param_ex & param9,
                      const log_param_ex & param10,
                      const log_param_ex & param11)
        WAT_PUBLIC_EVENT_INITIALIZER_LIST                      
    {
		WAT_CHECK_LIBRARY;    
        log_cached_item cached_item(this, function_name);
        cached_item.log_single_param(param1.get_begin(), param1.get_size());
        cached_item.log_single_param(param2.get_begin(), param2.get_size());
        cached_item.log_single_param(param3.get_begin(), param3.get_size());
        cached_item.log_single_param(param4.get_begin(), param4.get_size());
        cached_item.log_single_param(param5.get_begin(), param5.get_size());
        cached_item.log_single_param(param6.get_begin(), param6.get_size());
        cached_item.log_single_param(param7.get_begin(), param7.get_size());
        cached_item.log_single_param(param8.get_begin(), param8.get_size());
        cached_item.log_single_param(param9.get_begin(), param9.get_size());
        cached_item.log_single_param(param10.get_begin(), param10.get_size());
        cached_item.log_single_param(param11.get_begin(), param11.get_size());
    }
    
    // for macroses
     public_event()
        WAT_PUBLIC_EVENT_INITIALIZER_LIST     
    {
    }
