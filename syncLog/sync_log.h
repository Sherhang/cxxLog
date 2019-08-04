/*
 * 同步日志系统
 * 多线程使用c++11的std::thread
 * */
#include<iostream>
#include<fstream>
#include<sstream>
#include<unistd.h>
#include<sys/time.h>
#include<sys/stat.h>

#define LOGINIT(dir,file_name) \
do\
{\
    Log::inst()->init_path(dir,file_name);\
}while(0)

#define DEBUG Log::inst()->_file<<"[DEBUG]"<<__DATE__<<"|"<<__TIME__<<"|"<<__FILE__<<"|"<<__func__<<"|"<<__LINE__<<": " 
#define ERROR Log::inst()->_file<<"[ERROR]"<<__DATE__<<"|"<<__TIME__<<"|"<<__FILE__<<"|"<<__func__<<"|"<<__LINE__<<": " 
#define INFO Log::inst()->_file<<"[INFO ]"<<__DATE__<<"|"<<__TIME__<<"|"<<__FILE__<<"|"<<__func__<<"|"<<__LINE__<<": " 

using namespace std;
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
        return _inst;
    }
   
    void init_path(const string& dir, const string& file_name);
    bool makedir(const std::string& dir);
    bool openfile(const string& file_name);
    string get_curr_time();//当前系统时间，形式：20190804|09:28:56
};

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

