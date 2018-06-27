#include <stdio.h>  ///
#include <stdlib.h> ///
#include <unistd.h>
#include <errno.h>
#include <string.h>    ///
#include <sys/types.h> ///
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> ////
#include <netinet/in.h>
#include <sys/types.h> ///
#include <netdb.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("you need IP address \n");
    exit(1);
  }
  char *address;
  address = argv[1];

  int client_socket;
  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in remote_address;
  remote_address.sin_family = AF_INET;
  remote_address.sin_port = htons(80);
  inet_aton(address, &remote_address.sin_addr.s_addr);

  int connection_status = connect(client_socket, (struct sockaddr *)&remote_address, sizeof(remote_address));
  if (connection_status == -1)
  {
    printf(" %d , connection error\n ", connection_status);
  }

  char request[] = "GET / HTTP/1.1\r\n\r\n";
  char response[5000];

  // printf("the request ===>: %s \n", request);
  // printf(" hilal \n");
  send(client_socket, request, sizeof(request), 0);
  recv(client_socket, &response, sizeof(response), 0);

  printf("%s \n", response);
  close(client_socket);
  return 0;
}