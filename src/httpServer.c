
#include <stdio.h>  ///
#include <stdlib.h> ///
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>  ///
#include <sys/socket.h> ////
#include <netinet/in.h> ///
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[])
{

  FILE *html_data;
  html_data = fopen("index.txt", "r");

  char response_data[1024];
  fgets(response_data, 1024, html_data);

  //printf("the response data is ====>  %s \n", response_data);
  //fclose(html_data);

  char http_header[2048] = "HTTP/1.1 200 Ok\r\n\n";
  strcat(http_header, response_data);

  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(8000);
  server.sin_addr.s_addr = INADDR_ANY;

  bind(server_socket, (struct sockaddr *)&server, sizeof(server)); // socket , socket address and  socket length
  listen(server_socket, 10);

  int client_socket; // place holder for connection after happened
  while (1)
  {
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1)
    {
      perror("accept failed");
    }

    send(client_socket, http_header, sizeof(http_header), 0); // send data to client
    close(client_socket);
  }

  return 0;
}
