#ifndef _tool_time_h_
#define _tool_time_h_


#include <chrono>
class tool_time
{
public:
    static uint32_t elapse_seconds(std::chrono::system_clock::time_point &start)
    {
        return static_cast<uint32_t>( std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now() - start).count() );
    }

    static uint64_t elapse_ms(std::chrono::system_clock::time_point &start)
    {
        return static_cast<uint64_t>( std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now() - start).count() );
    }

    static uint32_t now_to_seconds()
    {
        return static_cast<uint32_t>( std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now().time_since_epoch()).count() );
    }

    static uint64_t now_to_ms()
    {
        return static_cast<uint64_t>( std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count() );
    }

    static uint64_t now_to_us()
    {
        return static_cast<uint64_t>( std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count() );
    }

    static uint64_t to_ms(const std::chrono::system_clock::time_point &time)
    {
        return static_cast<uint64_t>( std::chrono::duration_cast<std::chrono::milliseconds>
                (time.time_since_epoch()).count() );
    }

    static std::string to_str(const std::chrono::system_clock::time_point &time)
    {
        char _time[25] = {0};
        time_t tt = std::chrono::system_clock::to_time_t(time);
        struct tm local_time={0};
        localtime_r(&tt, &local_time);
        strftime(_time, 22, "%Y-%m-%d %H:%M:%S", &local_time);

        return std::string(_time);
    }
	static uint64_t morning_of_today_ms()
	{
        time_t now = time(0);

        return ((now+8*3600)/86400*86400-8*3600)*1000;

		//time_t now = time(0);
		//struct tm * loc_t = localtime(&now);
		//loc_t->tm_hour=0;loc_t->tm_min=0;loc_t->tm_sec=0;
		//time_t x = mktime(loc_t);
		//return x*1000;
	}
    //"%Y-%m-%d %H:%M:%S"
    static time_t to_time(const std::string&str)
    {
        time_t t_;
        tm tm_={0};
        strptime(str.c_str(), "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
        t_ = mktime(&tm_); //将tm时间转换为秒时间

        return t_;
    }

    ////"%d-%02d-%02d %02d:%02d:%02d.%03d"
    static std::chrono::system_clock::time_point to_time_ex(const std::string&str)
    {
        uint64_t pos = str.length()-3;

        time_t t_;
        tm tm_={0};
        strptime(str.substr(0,pos).c_str(), "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
        t_ = mktime(&tm_); //将tm时间转换为秒时间

        int milli = std::stoi(str.substr(pos));

        return std::chrono::system_clock::time_point(std::chrono::milliseconds(t_*1000 + milli));
    }

    //"%d-%02d-%02d %02d:%02d:%02d.%03d"
    static std::string to_str_ex(uint64_t ms)
    {
        int32_t mill = ms%1000;

        char _time[25] = {0};
        time_t tt = ms/1000;
        struct tm *local_time=localtime(&tt);
        //strftime(_time, 22, "%Y-%m-%d %H:%M:%S", local_time);
        sprintf(_time, "%d-%02d-%02d %02d:%02d:%02d.%03d", local_time->tm_year+1900,
                local_time->tm_mon+1, local_time->tm_mday, local_time->tm_hour,
                local_time->tm_min, local_time->tm_sec, mill);

        return std::string(_time);
    }

    //"%d-%02d-%02d %02d:%02d:%02d.%03d"
    static std::string to_str_ex(std::chrono::system_clock::time_point time)
    {
        return to_str_ex(to_ms(time));
    }

    static int elapse_seconds(time_t &start)
    {
        time_t now;
        time(&now);
        return static_cast<int>(std::difftime(now, start));
    }
    static std::string now_to_str()
    {
        std::time_t t=time(0);
        return to_str(t);
    }
    //"%Y-%m-%d %H:%M:%S"
    static std::string to_str(const std::time_t &time)
    {
        char _time[25] = {0};
        struct tm local_time;
        localtime_r(&time, &local_time);
        strftime(_time, 22, "%Y-%m-%d %H:%M:%S", &local_time);

        return std::string(_time);
    }
    //"%H:%M:%S"
    static std::string to_str_time(const std::time_t &time)
    {
        char _time[25] = {0};
        struct tm local_time;
        localtime_r(&time, &local_time);
        strftime(_time, 22, "%H:%M:%S", &local_time);

        return std::string(_time);
    }

    static int time_to_day_seconds(uint64_t t)
    {
        std::time_t _time = t;
        if (_time == 0)
        {
            _time = tool_time::now_to_seconds();
        }
        struct tm local_time;
        localtime_r(&_time, &local_time);
        return local_time.tm_hour * 3600 + local_time.tm_min * 60 + local_time.tm_sec;
    }
    //t 格式：05:02:02
    static int time_to_day_seconds(const std::string &t)
    {
        int hour = 0,min = 0,sec = 0;
        sscanf(t.c_str(), "%d:%d:%d", &hour,&min,&sec);
        return hour * 3600 + min * 60 + sec;
    }

    static int get_hour()
    {
        time_t tt = time(0);
        struct tm *local_time=localtime(&tt);
        return local_time->tm_hour;
    }
};

#endif

