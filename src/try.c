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
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{

  // IPv4 demo of inet_ntop() and inet_pton()

  struct sockaddr_in sa;
  char str[INET_ADDRSTRLEN];

  // store this IP address in sa:
  inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));

  // now get it back and print it
  inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);

  printf("%s\n", str); // prints "192.0.2.33"
}
