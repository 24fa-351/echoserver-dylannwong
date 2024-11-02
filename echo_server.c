#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_BACKLOG 5

void handleConnection(int client_socket) {
  char buffer[1024];
  int valread;
  while ((valread = read(client_socket, buffer, sizeof(buffer))) > 0) {
    buffer[valread] = '\0';
    printf("Received: %s\n", buffer);
    write(client_socket, buffer, valread);
  }
  close(client_socket);
}

int main(int argc, char *argv[]) {

  int port = 8080;

  if (argc > 1) {
    if ((strcmp(argv[1], "-p")) == 0) {
      if (argc == 3 && atoi(argv[2]) > 1023) {
        port = atoi(argv[2]);
      }
      printf("Listening to port %d\n", port);
    }
  }

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in socket_address;
  memset(&socket_address, '\0', sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
  socket_address.sin_port = htons(port);

  int returnval;

  returnval = bind(socket_fd, (struct sockaddr *)&socket_address,
                   sizeof(socket_address));

  returnval = listen(socket_fd, LISTEN_BACKLOG);

  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);

  while (1) {
    int client_socket = accept(socket_fd, (struct sockaddr *)&client_address,
                               &client_address_len);

    int pid = fork();

    if (pid == 0) {
      close(socket_fd);
      handleConnection(client_socket);
      exit(0);
    } else {
      close(client_socket);
    }
  }
  return 0;
}