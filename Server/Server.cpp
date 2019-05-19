#include "Server.hpp"

std::string root;

// колбек на аллокацию
void allocBufferCB(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
	buf->base = (char*)malloc(size);
	buf->len = size;
}

void closeCB(uv_handle_t* client) {
	delete client;
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
		request.sendResponse(client->io_watcher.fd);
		uv_close((uv_handle_t*)client, closeCB);
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

	uv_tcp_t *client = new uv_tcp_t;	// создание сокета клиента

	uv_tcp_init(server->loop, client);
	if (!uv_accept(server, (uv_stream_t*)client)) {	// присоединение клиента
		uv_read_start((uv_stream_t*)client, allocBufferCB, readCB);	// ожидание чтения
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

void test(void *arg) {
	uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	uv_loop_init(loop);

	uv_tcp_t server;
	int fd, option = 1;
	
	uv_tcp_init_ex(loop, &server, AF_INET);
	uv_fileno((uv_handle_t*)&server, &fd);
	setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option));

	uv_tcp_bind(&server, (struct sockaddr*)arg, 0);
	uv_listen((uv_stream_t*)&server, CONNECTIONS_COUNT, newConnectionCB);
	uv_run(loop, UV_RUN_DEFAULT);
}

Server::Server(const std::string ip, const unsigned short port, const std::string rootDir, const unsigned short threadsCount) {
	this->threadsCount = threadsCount;
	this->rootDir = rootDir;
	root = rootDir;
	
	struct sockaddr_in address;
	uv_ip4_addr(ip.c_str(), port, &address);

	for (int i = 0; i < threadsCount; i++) {
		uv_thread_t *worker = (uv_thread_t*)malloc(sizeof(uv_thread_t));
    	uv_thread_create(worker, test, &address);
		workers.push_back(worker);
    }

	for (auto worker : workers) {
		uv_thread_join(worker);
	}
	std::cout << "server stoped" << std::endl;
}
