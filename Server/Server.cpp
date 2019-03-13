#include "Server.hpp"


uv_loop_t *loop;
std::string root;

// колбек на аллокацию
void allocBufferCB(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
	buf->base = (char*)malloc(size);
	buf->len = size;
}

void socketWriteCB(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}

// колбек на чтение данных
void readCB(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0) {
		if (nread != UV_EOF) {
			std::cerr << "read error " << uv_err_name(nread) << std::endl;
		}
		uv_close((uv_handle_t*)client, NULL);
	} else if (nread > 0) {
		Http request(buf->base, root);
		std::cout << "===============\n";
		std::cout << "=== REQUEST ===\n" << buf->base;
		std::string response = request.getResponse();
		std::cout << "=== RESPONSE ===\n" << response;
		std::cout << "================\n";


		uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t writeBuf = uv_buf_init(const_cast<char*>(response.c_str()), response.length());
		uv_write(req, client, &writeBuf, 1, socketWriteCB);
	}

	if (buf->base) {
		free(buf->base);
	}
}

// колбек на подключение нового клиента
void newConnectionCB(uv_stream_t *server, int status) {
	if (status < 0) {	// проверка на наличие ошибок
		std::cerr << "new connection error " << uv_strerror(status) << std::endl;
		return;
	}

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));	// создание сокета клиента
	uv_tcp_init(loop, client);
	if (!uv_accept(server, (uv_stream_t*)client)) {	// присоединение клиента
		uv_read_start((uv_stream_t*)client, allocBufferCB, readCB);	// ожидание чтения
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

Server::Server(const std::string ip, const unsigned short port, const std::string dir) {
	root = dir;
	loop = uv_default_loop();
	
	struct sockaddr_in address;
	uv_ip4_addr(ip.c_str(), port, &address);

	uv_tcp_init(loop, &server);
	uv_tcp_bind(&server, (struct sockaddr*)&address, 0);
	uv_listen((uv_stream_t*)&server, CONNECTIONS_COUNT, newConnectionCB);
	uv_run(loop, UV_RUN_DEFAULT);
}

Server::~Server(){
	std::cout << "test" << std::endl;
	uv_loop_close(loop);
	free(loop);
}
