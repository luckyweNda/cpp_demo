CC = 
CXX = g++
CFLAGS = 
CXXFLAGS = \
-Wall \
-std=c++11 \
-lssl \
-lcrypto \
-lpthread \
-ldl \
-lz
CPPFLAGS = 
LDFLAGS = /usr/lib/x86_64-linux-gnu/libmysqlclient.a

mysql: mysql.cpp
	@echo compiling mysql...
	@$(CXX) $< $(LDFLAGS) $(CXXFLAGS)  -o $@

.PHONY: clean

clean:
	@rm -rf mysql