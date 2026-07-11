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
  int sockfd, thierfd, status;
  struct addrinfo hint, *server_addr;
  memset(&hint, 0, sizeof hint);
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;

  struct sockaddr_storage thier_addr;
  socklen_t thier_addrlen = 0;

  if ((status = getaddrinfo(NULL, "3490", &hint, &server_addr)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
  }

  if ((sockfd = socket(server_addr->ai_family, server_addr->ai_socktype,
                       server_addr->ai_protocol)) < 0) {
    perror("error code: \n");
    exit(1);
  }

  if ((status = bind(sockfd, server_addr->ai_addr, server_addr->ai_addrlen)) <
      0) {
    perror("error code: \n");
    exit(status);
  }

  if ((status = listen(sockfd, 15)) < 0) {
    perror("error code: \n");
    exit(status);
  }

  int64_t bytes = 0;
  char recvbuffer[175];
  char *sendbuffer = 0x0;
  uint64_t sndbufflen = 0;
  char serverip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(server_addr->ai_addr), serverip, INET_ADDRSTRLEN);

  printf(" listening on address: %s\n", serverip);

  while (1) {
    if ((thierfd = accept(sockfd, (struct sockaddr *)&thier_addr,
                          &thier_addrlen)) < 0) {
      perror("error code: \n");
      exit(thierfd);
    }

    if ((bytes = recv(thierfd, recvbuffer, 175, 0)) < 0) {
      perror("error code:");
      exit(bytes);
    }
    printf("%s\nbytes receved: %li", recvbuffer, bytes);

    char *sendbuffer = handler(recvbuffer, strlen(recvbuffer));

    sndbufflen = strlen(sendbuffer);

    if (!sendbuffer) {
      printf("the buffer pointer is null skipped this itterstion\n");
      sleep(1);
      continue;
    }

    if ((bytes = send(thierfd, sendbuffer, sndbufflen, 0)) < 0) {
      perror("error code: ");
      exit(bytes);
    }

    close(thierfd);
    memset(recvbuffer, 0, 175);
  }

  freeaddrinfo(server_addr);
  return 0;
}
