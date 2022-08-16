server:server.cpp handle.cpp mydata.cpp
	g++ -std=c++11 $^ `mysql_config --cflags --libs`  -o $@ -levent  -ljsoncpp