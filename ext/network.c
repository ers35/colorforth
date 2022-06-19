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

#define PERROR(msg) cf_printf(s, "ERROR: %s: %s\n", strerror(errno)); push(s->stack, 0)

void
server_start(struct state *s) {
  cell port = pop(s->stack);

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

  push(s->stack, server_fd);
}

void
server_stop(struct state *s) {
  cell fd = pop(s->stack);

	if (close(fd)) {
		PERROR("socket close");
		return;
	}

	cf_printf(s, "FD closed. Server stopped\n");

  push(s->stack, -1);
}

void
server_nonblocking(struct state *s) {
  cell fd = pop(s->stack);

	int status = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

	if (status == -1){
		PERROR("calling fcntl");
		return;
	}

	cf_printf(s, "FD made nonblocking\n");

	push(s->stack, fd);
}


void
server_accept(struct state *s) {
  cell server_fd = pop(s->stack);

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	cell new_socket;

	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
		PERROR("accept");
		return;
	}

	cf_printf(s, "Connection accepted");

	push(s->stack, new_socket);
}

void
socket_send (struct state *s) {
  cell socket = pop(s->stack);
  char * msg = CFSTRING2C(pop(s->stack));

	if (send(socket, msg, strlen(msg), 0) < 0) {
		PERROR("send failed");
		return;
	}

	push(s->stack, -1);
}

void
socket_send_char (struct state *s) {
  cell socket = pop(s->stack);
  char c = pop(s->stack);

	if (send(socket, &c, 1, 0) < 0) {
		PERROR("send failed");
		return;
	}

	push(s->stack, -1);
}

void
require_network_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, SERVER_START_HASH,          ENTRY_NAME("server-start"), server_start);
  define_primitive_extension(state, SERVER_STOP_HASH,           ENTRY_NAME("server-stop"), server_stop);
  define_primitive_extension(state, SERVER_NONBLOCKING_HASH,    ENTRY_NAME("server-nonblocking"), server_nonblocking);
  define_primitive_extension(state, SERVER_ACCEPT_HASH,         ENTRY_NAME("server-accept"), server_accept);

  define_primitive_extension(state, SOCKET_SEND_HASH,           ENTRY_NAME("socket-send"), socket_send);
  define_primitive_extension(state, SOCKET_SEND_CHAR_HASH,      ENTRY_NAME("socket-send-char"), socket_send_char);

  initialized = 1;
}
