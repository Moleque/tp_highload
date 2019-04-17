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
		std::vector<std::queue<Query>*> *queues = (std::vector<std::queue<Query>*>*)client->data;
		
		Query query;
		query.data = (char*)malloc(nread);
		memcpy(query.data, buf->base, nread);
		query.lenght = nread;
		query.client = client;
		
		(*queues)[counter]->push(query);
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
	std::queue<Query> *queue = (std::queue<Query>*)req->data;

	while (true) {
		Query query;
		if (!(queue->empty())) {		
			query = queue->front();
			queue->pop();
		}

		if (query.data != nullptr) {
			Http request(query.data, root);
			
			// std::cout << "===============\nREQUEST:\n" << query.data << std::endl;
			request.sendResponse(query.client->io_watcher.fd);
			
			close(query.client->io_watcher.fd);
			free(query.data);
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

	std::vector<std::queue<Query>*> queues;

	for (int i = 0; i < threadsCount; i++) {
		std::queue<Query> *queue = new std::queue<Query>;
		queues.push_back(queue);

        uv_work_t *worker = (uv_work_t*)malloc(sizeof(uv_work_t));
        worker->data = (void*)queue;
		workers.push_back(worker);
        uv_queue_work(loop, workers[i], work, after_work);
    }

	uv_tcp_t server;
	server.data = &queues;

	uv_tcp_init(loop, &server);
	uv_tcp_bind(&server, (struct sockaddr*)&address, 0);
	uv_listen((uv_stream_t*)&server, CONNECTIONS_COUNT, newConnectionCB);
	uv_run(loop, UV_RUN_DEFAULT);
}

Server::~Server() {
	for (auto thread : threads) {
		free(thread);
	}
}
