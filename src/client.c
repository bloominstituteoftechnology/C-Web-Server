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
  int networkSocket;
  networkSocket = socket(AF_INET, SOCK_STREAM, 0); // takes 3 args domain, type and protocol

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(9000);
  server.sin_addr.s_addr = INADDR_ANY;

  int connection_status = connect(networkSocket, (struct sockaddr *)&server, sizeof(server)); // socket , socket address and  socket length
  if (connection_status == -1)
  {
    printf(" problem  with the connection\n");
  }

  char response[256]; ///// buffer
  recv(networkSocket, &response, sizeof(response), 0);
  printf("the response is %s \n", response);
  ///////////////////////////////////////////////////////////////////////////////////

  // bind(networkSocket, (struct sockaddr *)&server, sizeof(server)); // socket , socket address and  socket length
  // listen(networkSocket, 10);
  // char message[] = "thank you";
  // int client;
  // client = accept(networkSocket, NULL, NULL);
  // send(client, message, sizeof(message), 0);
  ///////////////////////////////////////////////////////////////////
  //close(networkSocket);
}
