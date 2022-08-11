server:server.cpp handle.cpp
	g++ -std=c++11 $^ `pkg-config --libs --cflags opencv` -o $@ -levent