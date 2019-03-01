#include "Server.hpp"

Server::Server(const unsigned short port) {
    int sockfd;
	if (!(sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
		std::cout << "Cannot create listen socket" << std::endl;
		// pthread_exit(0);
	}

    int opt = true;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))<0) {
		std::cout << "Cannot install listen socket" << std::endl;
		close(sockfd);
		// pthread_exit(0);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
        std::cout << "Cannot configure listen socket" << std::endl;
        close(sockfd);
        // pthread_exit(0);
    }

	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))) {
		std::cout << "Ошибка настройки слушающего сокета." << std::endl;
		close(sockfd);
		// pthread_exit(0);
	}

	// if (listen(sockfd, (CL_MAX+1))!=0) {
	// 	std::cout << "Ошибка создания очереди." << std::endl;
	// 	close(sockfd);
	// 	// pthread_exit(0);
	// }
}

Server::~Server(){
}

void Server::start(){
	// if (WSAStartup(MAKEWORD(2, 2), &wData) == 0)
	// {
	// 	printf("WSA Startup succes\n");
	// }
	// SOCKADDR_IN addr;
	// int addrl = sizeof(addr);
	// addr.sin_addr.S_un.S_addr = INADDR_ANY;
	// addr.sin_port = htons(port);
	// addr.sin_family = AF_INET;
	// this_s = socket(AF_INET, SOCK_STREAM, NULL);
	// if (this_s == SOCKET_ERROR) {
	// 	printf("Socket not created\n");
	// }

	// if (bind(this_s, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
	// 	printf("Socket succed binded\n");
	// }

	// if (listen(this_s, SOMAXCONN) != SOCKET_ERROR){
	// 	printf("Start listenin at port%u\n", ntohs(addr.sin_port));
	// }
	// handle();
}

void Server::closeS() {
	// closesocket(this_s);
	// WSACleanup();
	// cout << "Server was stoped. You can close app" << endl;
}


void Server::handle() {
// 	while (true)
// 	{
// 		SOCKET acceptS;
// 		SOCKADDR_IN addr_c;
// 		int addrlen = sizeof(addr_c);
// 		if ((acceptS = accept(this_s, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
// 			printf("send\n");
// 			printf("sended Client connected from 0  %u.%u.%u.%u:%u\n",
// 				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
// 				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
// 				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
// 				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
// 				ntohs(addr_c.sin_port));
// 			SClient* client = new SClient(acceptS, addr_c);

// 		}
// 		Sleep(50);
// 	}
}