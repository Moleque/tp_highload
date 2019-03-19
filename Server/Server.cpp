#include "Server.hpp"

std::string root;





// колбек на запись
void socketWriteCB(uv_write_t *req, int status) {
    if (status) {
        std::cerr << "Write error " << uv_strerror(status) << std::endl;
    }
    free(req);
}

void idlerCB(uv_idle_t *handle) {
	Storage *storage = (Storage*)handle->data;

	Query query;
	uv_mutex_lock(&storage->mutex);
	if (!(storage->queue.empty())) {		
		query = storage->queue.front();
		storage->queue.pop();
	}
	uv_mutex_unlock(&storage->mutex);

	if (query.data != nullptr) {
		// char buffer[1000000];

		Http request(query.data, root);
		char *buffer;
		size_t size = request.getResponse(buffer);

		std::cout << "===============\nREQUEST:\n" << query.data << "RESPONSE:\n" << buffer << "===============\n" << std::endl;

		uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t writeBuf = uv_buf_init(buffer, size);
		uv_write(req, query.client, &writeBuf, 1, socketWriteCB);

		free(buffer);
		free(query.data);
	}
}

void threadWork(void *arg) {	// работа треда
	Storage *storage = (Storage*)arg;
	uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));	// создаем свой луп на каждый тред
	uv_loop_init(loop);
	
	uv_idle_t idler;
	idler.data = storage;
	uv_idle_init(loop, &idler);
	uv_idle_start(&idler, idlerCB);
	
	uv_run(loop, UV_RUN_DEFAULT);
	uv_loop_close(loop);
	free(loop);
}





// колбек на аллокацию
void allocBufferCB(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
	buf->base = (char*)malloc(size);
	buf->len = size;
}

// колбек на чтение данных
void readCB(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0) {
		if (nread != UV_EOF) {
			std::cerr << "read error " << uv_err_name(nread) << std::endl;
		}
		uv_close((uv_handle_t*)client, NULL);
	} else if (nread > 0) {
		Storage *storage = (Storage*)client->data;
		
		Query query;
		query.data = (char*)malloc(nread);
		memcpy(query.data, buf->base, nread);
		query.lenght = nread;
		query.client = client;
		
		uv_mutex_lock(&storage->mutex);
		storage->queue.push(query);
		uv_mutex_unlock(&storage->mutex);
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

	std::cout << "new connection" << std::endl;

	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));	// создание сокета клиента
	client->data = server->data;

	uv_tcp_init(server->loop, client);
	if (!uv_accept(server, (uv_stream_t*)client)) {	// присоединение клиента
		uv_read_start((uv_stream_t*)client, allocBufferCB, readCB);	// ожидание чтения
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

Server::Server(const std::string ip, const unsigned short port, const std::string rootDir, const unsigned short threadsCount) {
	this->threadsCount = threadsCount;
	this->rootDir = rootDir;
	root = rootDir;

	uv_loop_t *loop = uv_default_loop();
	
	struct sockaddr_in address;
	uv_ip4_addr(ip.c_str(), port, &address);

	Storage storage;	// хранилище для синхронизации потоков
	uv_mutex_init(&storage.mutex);

	uv_tcp_t server;
	server.data = &storage;

	uv_tcp_init(loop, &server);
	uv_tcp_bind(&server, (struct sockaddr*)&address, 0);
	uv_listen((uv_stream_t*)&server, CONNECTIONS_COUNT, newConnectionCB);
	
	for (int i = 0; i < threadsCount; i++) {	// создание воркеров в тредах
		uv_thread_t *thread = (uv_thread_t*)malloc(sizeof(uv_thread_t));
		threads.push_back(thread);
		uv_thread_create(thread, threadWork, &storage);
		std::cout << "thread " << thread << " was started" << std::endl;
	}
	uv_run(loop, UV_RUN_DEFAULT);
}

Server::~Server() {
	for (auto thread : threads) {
		free(thread);
	}
}
