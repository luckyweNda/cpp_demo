CC = 
CXX = g++
CFLAGS = 
CXXFLAGS = -Wall -lmysqlcppconn
CPPFLAGS = 
LDFLAGS = 

file: file.cpp
	@echo compiling file...
	@$(CXX) $? $(CXXFLAGS) -o $@
	@echo compile success

.PHONY: clean

clean:
	@rm -rf file