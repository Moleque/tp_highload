all: server

CXX = g++
# GDB = -g
# LIBS = -lpthread
NAME = server

server: main.cpp Server.o
	$(CXX) -o $(NAME) main.cpp Server.o $(LIBS) $(GDB)

Server.o: ./Server/Server.cpp
	$(CXX) -c ./Server/Server.cpp $(LIBS) $(GDB)

clean:
	-rm *.o