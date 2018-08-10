#include <stdio.h>  ///
#include <stdlib.h> ///
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  ///
#include <sys/socket.h> ////
#include <netinet/in.h> ///
#include <netdb.h>

int main(int argc, char *argv[])
{

  char message[256] = "you have reach the server!!!!!!!!!";

  int serverSocket;
  serverSocket = socket(AF_INET, SOCK_STREAM, 0); // takes 3 args domain = family , type and protocol (LESS IMPORT)

  struct sockaddr_in server;
  server.sin_family = AF_INET; // AF_INET is address family in socket
  server.sin_port = htons(9000);
  server.sin_addr.s_addr = INADDR_ANY; /// INNDDR = any IP address on the local machin

  bind(serverSocket, (struct sockaddr *)&server, sizeof(server)); // socket , socket address and  socket length
  listen(serverSocket, 10);

  int client;
  client = accept(serverSocket, NULL, NULL);
  send(client, message, sizeof(message), 0);

  //////////////////////////////////////////////////////////////////////////////
  // int connection_status = connect(serverSocket, (struct sockaddr *)&server, sizeof(server)); // socket , socket address and  socket length
  // if (connection_status == -1)
  // {
  //   printf(" problem  with the connection\n");
  // }
  // char response[256]; ///// buffer
  // recv(serverSocket, &response, sizeof(response), 0);
  // printf("the response is %s \n", response);

  //close(serverSocket);
}
