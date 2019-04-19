#include "Server.hpp"

std::string root;
int counter = 0;
int thCnt;


// void onCloseCB(uv_handle_t *client) {
//     printf("Closed, bye!\n");
//     free(client);
// }

// колбек на запись
void socketWriteCB(uv_write_t *req, int status) {
    if (status) {
        std::cerr << "Write error " << uv_strerror(status) << std::endl;
		if (!uv_is_closing((uv_handle_t*)req->handle)) {
			uv_close((uv_handle_t*)req->handle, NULL);//onCloseCB);
		}
    }
	free(req->bufs);
    free(req);
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
		std::vector<ThreadStorage*> *storages = (std::vector<ThreadStorage*>*)client->data;
		
		Query *query = new Query;
		query->data = (char*)malloc(nread);
		memcpy(query->data, buf->base, nread);
		query->lenght = nread;
		query->client = client;
		
		uv_mutex_lock(&(*storages)[counter]->mutex);
		(*storages)[counter]->queries.push(query);
		uv_mutex_unlock(&(*storages)[counter]->mutex);

		// std::cout << "!!!" << counter << std::endl;
		counter = (counter+1)%thCnt;
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
	client->data = server->data;

	uv_tcp_init(server->loop, client);
	if (!uv_accept(server, (uv_stream_t*)client)) {	// присоединение клиента
		uv_read_start((uv_stream_t*)client, allocBufferCB, readCB);	// ожидание чтения
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

void work(uv_work_t *req) {
	ThreadStorage *storage = (ThreadStorage*)req->data;

	while (true) {
		// Query *query = ;
		if (!storage->queries.empty()) {
			// std::cout << "lock:" << storage << std::endl;
			uv_mutex_lock(&storage->mutex);
			// std::cout << "unlock:" << storage << std::endl;
		// }
		// if (!(queue->empty())) {		
			Query *query = storage->queries.front();
			storage->queries.pop();
			uv_mutex_unlock(&storage->mutex);

		// }

		// if (query->data != nullptr) {
			Http request(query->data, root);
			
			// std::cout << "===============\nREQUEST:\n" << query.data << std::endl;
			request.sendResponse(query->client->io_watcher.fd);
			
			close(query->client->io_watcher.fd);
			free(query->data);
			delete query;
		}
	}
}

void after_work(uv_work_t *req, int status) {
    int i = *(int *) req->data;
    if (status == UV_ECANCELED) {
        printf("Cancelled\n");
    } else if (!status){
        printf("Done calculating %dth work\n", i);
    }
    free(req->data);
    free(req);
}

Server::Server(const std::string ip, const unsigned short port, const std::string rootDir, const unsigned short threadsCount) {
	this->threadsCount = threadsCount;
	thCnt = threadsCount;
	this->rootDir = rootDir;
	root = rootDir;

	uv_loop_t *loop = uv_default_loop();
	
	struct sockaddr_in address;
	uv_ip4_addr(ip.c_str(), port, &address);

	for (int i = 0; i < threadsCount; i++) {
		ThreadStorage *storage = new ThreadStorage;
		storages.push_back(storage);
		uv_mutex_init(&storage->mutex);
		// uv_mutex_lock(&storage->mutex);

        uv_work_t *worker = (uv_work_t*)malloc(sizeof(uv_work_t));
		workers.push_back(worker);
        
		worker->data = (void*)storage;
        uv_queue_work(loop, workers[i], work, after_work);
    }

	uv_tcp_t server;
	server.data = &storages;

	uv_tcp_init(loop, &server);
	uv_tcp_bind(&server, (struct sockaddr*)&address, 0);
	uv_listen((uv_stream_t*)&server, CONNECTIONS_COUNT, newConnectionCB);
	uv_run(loop, UV_RUN_DEFAULT);
}

Server::~Server() {
	for (auto worker : workers) {
		free(worker);
	}
	for (auto storage : storages) {
		uv_mutex_destroy(&storage->mutex);
		delete storage;
	}
}
