all: server

CXX = g++
# GDB = -g
LIBS = -luv -lstdc++ #-lpthread
NAME = server

server: main.cpp Server.o Parser.o Config.o
	$(CXX) -o $(NAME) main.cpp Server.o Parser.o Config.o $(LIBS) $(GDB)

Server.o: ./Server/Server.cpp
	$(CXX) -c ./Server/Server.cpp $(LIBS) $(GDB)

Parser.o:
	$(CXX) -c ./Server/Parser.cpp $(LIBS) $(GDB)

Config.o:
	$(CXX) -c ./Settings/Config.cpp $(LIBS) $(GDB)

clean:
	-rm *.o $(NAME)