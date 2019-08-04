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
    
    _file_name = get_curr_time().substr(0, 8) + _file_name;//20180809logname.log. log update everyday.
    makedir(_dir);
    openfile(_file_name);
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
    //cout<<file_name<<endl;
    _file.open(dir_file.c_str(), ios::app); //追加写入
    return true;
}

string Log::get_curr_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL); //step1
    uint64_t sys_sec = tv.tv_sec;
    //DEBUG<<"sys_sec="<<sys_sec<<endl;
    struct tm curr_time;
    localtime_r((time_t *)&sys_sec, &curr_time); //setp2
    string ret;
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
