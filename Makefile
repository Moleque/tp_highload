all: server

CXX = g++
# GDB = -g
LIBS = -luv -lstdc++ #-lpthread
NAME = server

server: main.cpp Server.o Parser.o
	$(CXX) -o $(NAME) main.cpp Server.o Parser.o $(LIBS) $(GDB)

Server.o: ./Server/Server.cpp
	$(CXX) -c ./Server/Server.cpp $(LIBS) $(GDB)

Parser.o:
	$(CXX) -c ./Server/Parser.cpp $(LIBS) $(GDB)

clean:
	-rm *.o $(NAME)