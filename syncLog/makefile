CXX = g++ -std=c++11

src = $(wildcard *.cpp)
target = $(patsubst %.cpp, %, $(wildcard demo*.cpp))  #目标文件是所有demo*的 
rely = sync_log.cpp 


INCS = 
LIBS = -lpthread 

CFLAGS += -g 

all:$(target) $(relay)

$(target):%:%.cpp $(rely)  	#用模式规则对每个独立的目标进行编译
	$(CXX) $(INCS) $(CFLAGS) $^ -o $@ $(LIBS)


.PHONY:all clean
clean:
	rm -rf  $(obj) $(target) ./app_log
