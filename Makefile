all: server

CXX = g++
GDB = -g
LIBS = -lrt -luv -lstdc++ #-lpthread
NAME = server

server: clean main.cpp Server.o Parser.o Config.o
	$(CXX) -o $(NAME) main.cpp Server.o Parser.o Config.o $(LIBS) $(GDB)
	# kill $(lsof -t -i:80)

Server.o: ./Server/Server.cpp
	$(CXX) -c ./Server/Server.cpp $(LIBS) $(GDB)

Parser.o:
	$(CXX) -c ./Server/Parser.cpp $(LIBS) $(GDB)

Config.o:
	$(CXX) -c ./Settings/Config.cpp $(LIBS) $(GDB)


test:
	docker stop $$(docker ps -a -q)
	docker build -t salman .
	docker rm salman-server
	docker run --network host -p 80:80 -v /etc/httpd.conf:/etc/httpd.conf:ro -v /var/www/html:/var/www/html:ro --name salman-server -t salman

clean:
	# docker image prune
	# docker system prune
	-rm *.o $(NAME)