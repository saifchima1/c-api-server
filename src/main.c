#include "defin.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // initialising the socket data
  int sockfd, thierfd, status;
  struct addrinfo hint, *server_addr;
  memset(&hint, 0, sizeof hint);
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;

  struct sockaddr_storage thier_addr;
  socklen_t thier_addrlen = 0;
  // generating the server address
  if ((status = getaddrinfo(NULL, "8081", &hint, &server_addr)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
  }
  // this generates server socket file descripter
  if ((sockfd = socket(server_addr->ai_family, server_addr->ai_socktype,
                       server_addr->ai_protocol)) < 0) {
    perror("error code: \n");
    exit(1);
  }
  // this bindes the socket to the port 8081
  if ((status = bind(sockfd, server_addr->ai_addr, server_addr->ai_addrlen)) <
      0) {
    perror("error code: \n");
    exit(status);
  }
  // this makes the socket lestens for any connection
  if ((status = listen(sockfd, 15)) < 0) {
    perror("error code: \n");
    exit(status);
  }
  // this is initialising the server loop data
  int64_t bytes = 0;
  char recvbuffer[175];
  char *sendbuffer = 0x0;
  uint64_t sndbufflen = 0;
  char serverip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(server_addr->ai_addr), serverip, INET_ADDRSTRLEN);

  printf(" listening on address: %s\n", serverip);

  while (1) {
    // this accept the connection
    if ((thierfd = accept(sockfd, (struct sockaddr *)&thier_addr,
                          &thier_addrlen)) < 0) {
      perror("error code: \n");
      exit(thierfd);
    }
    // this receves data aka request from the client file descripter
    if ((bytes = recv(thierfd, recvbuffer, 175, 0)) < 0) {
      perror("error code:");
      exit(bytes);
    }
    printf("%s\nbytes receved: %li", recvbuffer, bytes);
    // this generates the response for the request check handler.c to see how it
    // works
    if (!(sendbuffer = handler(recvbuffer, strlen(recvbuffer)))) {
      continue;
    }

    sndbufflen = strlen(sendbuffer);
    if (!sendbuffer) {
      printf("the buffer pointer is null skipped this itterstion\n");
      sleep(1);
      continue;
    }

    // this block sends the generated response to the client
    if ((bytes = send(thierfd, sendbuffer, sndbufflen, 0)) < 0) {
      perror("error code: ");
      exit(bytes);
    }

    // this closes the client file descripter and clears the uneeded data
    close(thierfd);
    memset(recvbuffer, 0, 175);
  }
  // idk why i put this here but it frees the server address data
  freeaddrinfo(server_addr);
  return 0;
}
