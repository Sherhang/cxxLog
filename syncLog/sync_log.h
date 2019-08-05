/*
 * 同步日志系统
 * 多线程使用c++11的std::thread和mutex
 * */
#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<unistd.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<thread>
#include<mutex>
#include<signal.h>

using namespace std;
extern std::mutex _mtx;//为了防止多次包含，cpp中定义，h中用extern

class Log
{
private:
    string _dir;
    string _file_name;  
    static Log* _inst;//单例模式
    Log() {}
    ~Log() {}
public:
     ofstream _file;
     static Log* inst()
    {
         if(_inst == NULL)
        {
            //cout<<"inst is NULL , create "<<endl;
            _inst = new Log();
        }
        //cout<<"get inst"<<endl;
        return _inst;
    }
   
    void init_path(const string& dir, const string& file_name);
    bool makedir(const std::string& dir);
    bool openfile(const string& file_name);
    string get_curr_time();//当前系统时间，形式：20190804|09:28:56
    friend void flush_filename(int signal);
    //friend bool set_time();
};

bool set_time();
//数字转字符串
//demo: int a = str2num<int>(string("-01.23"));
template <class T>
T str2num(const string &str)
{
    stringstream ss;
    T num;
    ss << str;
    ss >> num;
    return num;
}

//demo: double num=-23.1; string str = num2str(num);
//也可以指定T的类型，str = num2str<int>(num);
template <class T>
string num2str(const T &num)
{
    stringstream ss;
    ss << num;
    string str;
    ss >> str;
    return str;
}

#define LOGINIT(dir,file_name) \
do\
{\
    _mtx.lock();\
    Log::inst()->init_path(dir,file_name);\
    std::thread th;\
    th=std::thread(set_time);\
    th.detach();\
    _mtx.unlock();\
}while(0)

#define DEBUG(x)\
do\
{\
    _mtx.lock();\
    Log::inst()->_file<<"[DEBUG]"<<__DATE__<<"|"<<__TIME__<<"|"<<__FILE__<<"|"<<__func__<<"|"<<__LINE__<<": " x;\
    _mtx.unlock();\
}while(0) 

#define ERROR(x)\
do\
{\
    _mtx.lock();\
    Log::inst()->_file<<"[ERROR]"<<__DATE__<<"|"<<__TIME__<<"|"<<__FILE__<<"|"<<__func__<<"|"<<__LINE__<<": " x;\
    _mtx.unlock();\
}while(0)

#define INFO(x)\
do\
{\
    _mtx.lock();\
    Log::inst()->_file<<"[INFO ]"<<__DATE__<<"|"<<__TIME__<<"|"<<__FILE__<<"|"<<__func__<<"|"<<__LINE__<<": " x;\
    _mtx.unlock();\
}while(0)

