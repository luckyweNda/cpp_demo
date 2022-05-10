target: client server

client: client.cpp
	@g++ $< -o $@

server: server.cpp
	@g++ $< -o $@


.PHONY: clean

clean:
	@rm -rf client server