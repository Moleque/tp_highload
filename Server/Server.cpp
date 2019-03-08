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


uv_loop_t *loop;


void alloc_buffer(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
	buf->base = (char*)malloc(size);
	buf->len = size;
}

void socket_write_cb(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }

    free(req);
}

void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {	// колбек на чтение данных
	if (nread < 0) {
		if (nread != UV_EOF) {
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		}
		uv_close((uv_handle_t*)client, NULL);
	} else if (nread > 0) {
		uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t write_buf = uv_buf_init(buf->base, nread);

		uv_write(req, client, &write_buf, 1, socket_write_cb);
	}

	printf("read\n");
	if (buf->base) {
		free(buf->base);
	}
}

void newConnectionCB(uv_stream_t *server, int status) {	// колбек на подключение нового клиента
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		return;
	}
	printf("connection\n");

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (!uv_accept(server, (uv_stream_t*)client)) {	// клиент подсоединился
		uv_read_start((uv_stream_t*)client, alloc_buffer, (uv_read_cb)read_cb);
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

Server::Server(const char *ip, const unsigned short port/*, const std::string dir*/) {
	loop = uv_default_loop();
	
	struct sockaddr_in address;
	uv_ip4_addr(ip, port, &address);

	uv_tcp_init(loop, &server);
	uv_tcp_bind(&server, (struct sockaddr*)&address, 0);
	uv_listen((uv_stream_t*)&server, CONNECTIONS_COUNT, newConnectionCB);
	printf("start\n");
	uv_run(loop, UV_RUN_DEFAULT);
}

Server::~Server(){
}

void Server::start() {
	printf("test\n");
	uv_loop_close(loop);
	free(loop);
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
