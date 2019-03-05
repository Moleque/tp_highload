#include "Server.hpp"

int setNonblock(int sockfd) {	// кроссплатформенная установка неблокирующего сокета
	int flags;
	#if defined (O_NONBLOCK)
		if ((flags = fcntl(sockfd, F_GETFL, 0)) == -1) {
			flags = 0;
		}
		return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
	#else
		flags = 1;
		return ioctl(fd, FIOBIO, &flags);
	#endif
}


uv_tcp_t server;	// сокет
uv_loop_t *loop = uv_default_loop();

void read_cb(uv_stream_t *stream, ssize_t nread, uv_buf_t buf) {
	uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_write(req, stream, &buf, 1, NULL);
	free(buf.base);
}

uv_buf_t alloc_buffer(uv_handle_t *handle, size_t size) {
	return uv_buf_init((char*)malloc(size), size);
}

void connection_cb(uv_stream_t *server, int status) {
	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	uv_accept(server, (uv_stream_t*)client);
	uv_read_start((uv_stream_t*)client, (uv_alloc_cb)alloc_buffer, (uv_read_cb)read_cb);
}

Server::Server(const unsigned short port/*, const std::string dir*/) {
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	uv_ip4_addr("0.0.0.0", port, &address);

	uv_tcp_init(loop, &server);
	uv_tcp_bind(&server, (struct sockaddr*)&address, 0);
	uv_listen((uv_stream_t*)&server, 128, connection_cb);

	uv_run(loop, UV_RUN_DEFAULT);

	// std::set<int> clients;
    // int sockfd = socket(AF_INET, SOCK_STREAM, 0);	// создание сокета
	// if (!sockfd) {
	// 	std::cout << "Cannot create listening socket" << std::endl;
	// }

    // int opt = 1;
	// if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {	// 
	// 	std::cout << "Cannot install listen socket" << std::endl;
	// 	close(sockfd);
	// }
	// // TODO: добавить ограничение на время ожидания чтения или конекта
	// // memset(&address, 0, sizeof(address));
	
	// struct sockaddr_in address;
	// address.sin_family = AF_INET;
	// address.sin_addr.s_addr = htonl(INADDR_ANY);
	// address.sin_port = htons(port);
	// if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))) {	// соединение сокета с ip адресом
	// 	std::cout << "Ошибка настройки слушающего сокета." << std::endl;
	// 	close(sockfd);
	// }

	// if (setNonblock(sockfd) == -1) {	// установка сокета неблокирующим
    //     std::cout << "Cannot set listening socket to nonblock" << std::endl;
    //     close(sockfd);
    // }
	// if (listen(sockfd, SOMAXCONN) < 0) {	// прослушивание сокета
	// 	std::cout << "Ошибка создания очереди." << std::endl;
	// 	close(sockfd);
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
