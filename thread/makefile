CC = 
CXX = g++
CFLAGS = 
CXXFLAGS = -Wall -lpthread
CPPFLAGS = 
LDFLAGS = 

OBJ = thread
PREREQUISITES = thread.cpp

$(OBJ): $(PREREQUISITES)
	@echo compiling $@...
	@$(CXX) $? $(CXXFLAGS) -o $@
	@echo compile success

.PHONY: clean

clean:
	@rm -rf $(OBJ)