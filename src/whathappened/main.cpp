#include "wat_lib.h"
#include "wat_analyzer.h"
#include "iostream"
#include "wat_reporter.h"

struct usage_error:std::runtime_error
{
    std::string m_option;
    int m_required_arguments_count;

    usage_error(const std::string & option = std::string(), int required_arguments_count = 0)
        : 
            std::runtime_error("Incorrect usage"),
            m_option(option),
            m_required_arguments_count(required_arguments_count)
    {
    }
};
void do_selftest(const std::string & file_name_hint)
{
    std::string report_file_name = file_name_hint + ".report";
    wat::ptime_t timeval;
    {
        // test code instrumentation
        wat::lib_auto lib(file_name_hint);

        // declare single scoped event
        wat::public_event evt(__FUNCTION__, "number=",1);

        // declare second scoped event
        WAT_DEF_SCOPED_EVENT(evt2)
                WAT_PARAM("number=") 
                WAT_PARAM((unsigned long long)2)
            WAT_END;

        wat::sleep_for_milliseconds(100);

        // fire single message to the log
        WAT_LOG_EVENT("just test message")
                WAT_PARAM((int)42)
            WAT_LOG_END;

        // link the tag to the thread for identification purposes
        WAT_LOG_SET_THRED_TAG("pool thread")
                WAT_PARAM("very important")
            WAT_LOG_END;

        timeval = wat::get_local_time();
    }
    std::string log_file_name = wat::generate_new_name(file_name_hint, 0);
    wat::sleep_for_milliseconds(100);
    wat::ptime_t start_time = timeval;
    wat::analyzer analyzer(report_file_name, log_file_name, start_time, 1);
    analyzer.do_analysis();
}

void do_report(const std::string & reportfilename, 
               const std::string & logfilename, 
               const std::string & timestr)
{
    wat::ptime_t start_time;
    if (!wat::time_from_string(timestr, &start_time))
    {
        throw std::runtime_error("Invalid time format: " + timestr);
    }
    int seconds = 1;
    wat::analyzer analyzer(reportfilename, logfilename, start_time, seconds);
    analyzer.do_analysis();
}
int main(int argc, char * argv[])
{
    try
    {
        if (argc <= 2)
        {
            throw usage_error();
        }
        std::string operation = argv[1];
        if (operation == "/selftest")
        {
            do_selftest(argv[2]);
            return 0;
        }
        if (operation == "/report")
        {
            if (argc != 5)
            {
                throw usage_error(operation, 5);
            }
            do_report(argv[2], argv[3], argv[4]);
            return 0;
        }
        throw std::runtime_error("Unknown option");
    }
    catch(const usage_error & ex)
    {
        if (!ex.m_option.empty() && ex.m_required_arguments_count)
        {
            std::cout<<"Error: \""<<ex.m_option<<"\" requires "<<ex.m_required_arguments_count<<"arguments, see usage:\n";
        }
        std::cout<<"Usage: \n";
        std::cout<<"1) /report <report file> <log file> <yyyy/mm/dd hh:mm:ss:SSS>\n";
        std::cout<<"2) /selftest <log file>\n";
        return 1;
    }
    catch(const std::exception & ex)
    {
        std::cout<<ex.what()<<"\n";
        return 1;
    }
    return 0;
}