// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

static char initialized = 0;

#define PERROR(msg) cf_printf(s, "ERROR: %s: %s\n", msg, strerror(errno)); PUSH1(0)

void
server_start(struct state *s) {
  POP1();
  cell port = p1;

	cell server_fd;
	struct sockaddr_in address;
	int opt = 1;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		PERROR("socket failed");
		return;
	}

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		PERROR("socket failed");
		return;
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
				   &opt, sizeof(opt))) {
		PERROR("setsockopt");
		return;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		PERROR("bind failed");
		return;
	}

	if (listen(server_fd, 3) < 0) {
		PERROR("listen");
		return;
	}

	cf_printf(s, "Server started on port: %d\n", port);

  PUSH1(server_fd);
}

void
client_start(struct state * s) {
  POP2();
  cell port = p1;
  char * host = CFSTRING2C(p2);

  cf_printf(s, "client: %s  %d\n", host, port);

	int sock = 0;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		PERROR("Socket creation error");
		return;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
		PERROR("Invalid address/ Address not supported");
		return;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		PERROR("Connection Failed");
		return;
	}

  cf_printf(s, "Client connected on %s on port %d\n", host, port);

  PUSH1(sock);
}

void
server_stop(struct state *s) {
  POP1();
  cell fd = p1;

	if (close(fd)) {
		PERROR("socket close");
		return;
	}

	cf_printf(s, "FD closed. Server stopped\n");

  PUSH1(-1);
}

void
server_nonblocking(struct state *s) {
  POP1();
  cell fd = p1;

	int status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1){
		PERROR("calling fcntl");
		return;
	}

	cf_printf(s, "FD made nonblocking\n");

	PUSH1(fd);
}


void
server_accept(struct state *s) {
  POP1();
  cell server_fd = p1;

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	cell new_socket;

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
		PERROR("accept");
		return;
	}

	cf_printf(s, "Connection accepted\n");

	PUSH1(new_socket);
}

void
socket_send (struct state *s) {
  POP2();
  cell socket = p1;
  char * msg = CFSTRING2C(p2);

	if (send(socket, msg, strlen(msg), 0) < 0) {
		PERROR("send failed");
		return;
	}

	PUSH1(-1);
}

void
socket_send_char (struct state *s) {
  POP2();
  cell socket = p1;
  char c = p2;

	if (send(socket, &c, 1, 0) < 0) {
		PERROR("send failed");
		return;
	}

	PUSH1(-1);
}

void
socket_recv(struct state *s) {
  POP3();
  cell socket = p1;
  cell len = p2;
  char *buffer = (char *)p3;

  buffer[0] = recv(socket, CFSTRING2C(buffer), len, 0);

	if (buffer[0] < 0) {
		if (errno == EAGAIN) {
      PUSH1(0);
			return;
		}
		PERROR("recv failed");
		return;
	}

  PUSH1(-1);
}

void
require_network_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, SERVER_START_HASH,          ENTRY_NAME("server-start"), server_start);
  define_primitive_extension(state, CLIENT_START_HASH,          ENTRY_NAME("client-start"), client_start);
  define_primitive_extension(state, SERVER_STOP_HASH,           ENTRY_NAME("server-stop"), server_stop);
  define_primitive_extension(state, SERVER_NONBLOCKING_HASH,    ENTRY_NAME("server-nonblocking"), server_nonblocking);
  define_primitive_extension(state, SERVER_ACCEPT_HASH,         ENTRY_NAME("server-accept"), server_accept);

  define_primitive_extension(state, SOCKET_SEND_HASH,           ENTRY_NAME("socket-send"), socket_send);
  define_primitive_extension(state, SOCKET_SEND_CHAR_HASH,      ENTRY_NAME("socket-send-char"), socket_send_char);
  define_primitive_extension(state, SOCKET_RECV_HASH,           ENTRY_NAME("socket-recv"), socket_recv);

  initialized = 1;
}
