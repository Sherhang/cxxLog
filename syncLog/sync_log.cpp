/*同步日志系统
 * */
#include "sync_log.h"
#include <sys/time.h> //struct timeval
#include <time.h>     //struct tm

std::mutex _mtx;//这个变量是为了在h中使用
Log *Log::_inst = new Log();//饿汉模式,NULL懒汉模式

void Log::init_path(const string &dir, const string &file_name)
{
    _dir = dir;
    if (file_name.size() < 4 || file_name.substr(file_name.size() - 4) != ".log")
    {
        _file_name = file_name + ".log";
    }
    else
    {
        _file_name = file_name;
    }
    
    string file_name_tmp = get_curr_time().substr(0, 8) + _file_name;//20180809logname.log. log update everyday.
    makedir(_dir);
    openfile(file_name_tmp);
}

bool Log::makedir(const std::string &dir)
{
    stringstream ss;
    ss << dir;
    string dir_c;
    string tmp;
    while (getline(ss, tmp, '/')) //getline第一个参数是流。
    {
        dir_c = dir_c + tmp + "/";
        if (access(dir_c.c_str(), F_OK | W_OK) == 0) //如果目录已经存在，可读可写，则跳过
        {
            //DEBUG<<"dir "<<dir_c<<" exists!"<<endl;
            continue;
        }
        if (mkdir(dir_c.c_str(), 0777) != 0) //返回0表示成功,失败则退出程序
        {
            //DEBUG<<"mkdir fail "<<dir_c<<endl;
            return false;
        }
    }
    return true;
}

bool Log::openfile(const string &file_name)
{
    string dir_file;
    if ((!_dir.empty()) && _dir[_dir.size() - 1] != '/')
    {
        dir_file = _dir + "/" + file_name;
    }
    else
        dir_file = _dir + file_name;
    //cout<<"file_name "<<dir_file<<endl;
    _file.open(dir_file.c_str(), ios::app); //追加写入
    return true;
}

string Log::get_curr_time()
{
    //struct timeval tv;
    //gettimeofday(&tv, NULL); //step1
    //uint64_t sys_sec = tv.tv_sec;
    //DEBUG<<"sys_sec="<<sys_sec<<endl;
    time_t sys_sec= time(NULL);
    struct tm curr_time;
    localtime_r((time_t *)&sys_sec, &curr_time); //setp2
    string ret;
    //用snprintf()来格式化效率低
    string year, mon, mday, hour, min, sec;
    year = num2str<uint64_t>(curr_time.tm_year + 1900);
    mon = num2str<uint64_t>(curr_time.tm_mon + 1);
    mday = num2str<uint64_t>(curr_time.tm_mday);
    hour = num2str<uint64_t>(curr_time.tm_hour);
    min = num2str<uint64_t>(curr_time.tm_min);
    sec = num2str<uint64_t>(curr_time.tm_sec);
    if (mon.size() < 2)
        mon = "0" + mon;
    if (mday.size() < 2)
        mday = "0" + mday;
    if (hour.size() < 2)
        hour = "0" + hour;
    if (min.size() < 2)
        min = "0" + min;
    if (sec.size() < 2)
        sec = "0" + sec;
    ret = year + mon + mday + "|" + hour + ":" + min + ":" + sec;
    return ret;
}

//友元函数用于每天刷新
void flush_filename(int signal)
{
    _mtx.lock();//一定要保证当前没有写操作
    Log::inst()->_file.clear();//清除状态标识符
    Log::inst()->_file.close();
    string file_name = Log::inst()->get_curr_time().substr(0,8) + Log::inst()->_file_name;//20180809|12:12:13 14到分钟
    //cout<<"get a signal "<<signal<<endl;
    Log::inst()->openfile(file_name);
    _mtx.unlock();
}

bool set_time()
{
    struct itimerval value, ovalue;
    string time_now = Log::inst()->get_curr_time();
    time_now = time_now.substr(9);
    //cout<<"time_now "<<time_now<<endl;
    uint64_t hour_now = str2num<uint64_t>(time_now.substr(0,2));
    uint64_t min_now = str2num<uint64_t>(time_now.substr(3,2));
    uint64_t sec_now = str2num<uint64_t>(time_now.substr(6,2));
    value.it_value.tv_sec = 24*3600-(hour_now*3600+min_now*60+sec_now);//当天剩余时间
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 24*3600;//间隔一天
    value.it_interval.tv_usec = 0;
    //cout<<"invoke signal, setitimer"<<endl;
    signal(SIGALRM, flush_filename);
    setitimer(ITIMER_REAL, &value, &ovalue);//set 
    for(;;);
    return true;
}
