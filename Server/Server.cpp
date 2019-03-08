#include "Server.hpp"


uv_loop_t *loop;

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
	printf("read\n");
	if (nread < 0) {
		if (nread != UV_EOF) {
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		}
		uv_close((uv_handle_t*)client, NULL);
	} else if (nread > 0) {
		printf(buf->base);
		// httpParse(buf->base);

		uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
		uv_buf_t writeBuf = uv_buf_init(buf->base, nread);
		uv_write(req, client, &writeBuf, 1, socketWriteCB);
	}

	if (buf->base) {
		free(buf->base);
	}



	// response_id = http_parse(&http_request, bufferevent_get_input(bev), root_dir);
    // switch (response_id) {
    //     case NOT_ALLOWED_HTTP_METHOD :
    //         createResponse("405", "Not Implemented", response);
    //         printf("405 ERROR\n");
    //         break;
    //     case FILE_NOT_EXIST :
    //         createResponse("404", "Not found", response);
    //         printf("404 ERROR\n");
    //         break;
    //     case FILE_IS_EXECUTABLE :
    //         create_response("500", "Internal server error", response);
    //         printf("500 ERROR\n");
    //         break;
    //     case ALL_OK :
    //         create_response("200", "OK", response);
    //         printf("200 OK\n");
    //         break;
    //     case ESCAPING_ROOT :
    //         printf("403 ERROR\n");
    //         create_response("403", "Forbidden", response);
    //         break;
    //     case PARSE_ERROR : 
    //         printf("PARSE ERROR\n");
    //     	create_response("400", "Bad request", response);
    //         break;
    //     case INDEX_FILE_NOT_EXIST:
    //         printf("INDEX ERROR\n");
    //         create_response("403", "Forbidden", response);
    //         break;
    //     default:
    //         printf("DEFAULT 500 ERROR\n");
    //         create_response("500", "Internal server error", response);
    //         break;
    // }

    // printf("content-length\n");

    // if (response_id == ALL_OK) {
    //     sprintf(response + strlen(response), "Content-Length: %lu\r\n", http_request.filesize);
    //     sprintf(response + strlen(response), "Content-Type: %s\r\n\r\n", http_request.filetype);
    // }

    // evbuffer_add(client->output_buffer, response, strlen(response));
    // printf("client buffer filled\n");
    // if (strcmp(http_request.method, "GET") == 0 && response_id == ALL_OK) {
    //     int fd = open(http_request.filename, O_RDONLY, 0);
    //     evbuffer_add_file(client->output_buffer, fd, 0, http_request.filesize);
    // }
    // if(strcmp(http_request.method, "HEAD") == 0){
    //     printf("HEAD request\n%s", response);
    // }
    // bufferevent_disable(bev, EV_READ);
    // bufferevent_enable(bev, EV_WRITE);
	// /* Send the results to the client.  This actually only queues the results for sending.
	//  * Sending will occur asynchronously, handled by libevent. */
	// if (bufferevent_write_buffer(bev, client->output_buffer) != 0) {
    //     printf("bufferevent error\n");
    //     closeClient(client);
    // }

    //struct timeval delay = { 1e };
}

// колбек на подключение нового клиента
void newConnectionCB(uv_stream_t *server, int status) {
	if (status < 0) {	// проверка на наличие ошибок
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
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
	printf("test\n");
	uv_loop_close(loop);
	free(loop);
}

void Server::start() {
	
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
