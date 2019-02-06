#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void){
    // send buffer
    char sendbuffer[100];

    // receive buffer
    char recvbuffer[100];

    int sockfd;

    // struct that holds info such as IP address and port
    struct sockaddr_in servaddr;

    // init the socket to use IP addressing
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // clear the servaddr struct
    memset(&servaddr, 0, sizeof(servaddr));

    // Set the values in the servaddr struct
    // set addressing scheme to IP addressing
    servaddr.sin_family = AF_INET;
    //listen on port 3490
    servaddr.sin_port = htons(3490);

    // set the IP address of the server we want to contact
    // in this case, it will just be localhost
    //inet_pton converts 127.0.0.1 to an integer and sets it in the servaddr struct
    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

    //connect to the server specified in the servaddr struct
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    while(1){
        //clear both the buffers
        memset(sendbuffer, 0, 100);
        memset(recvbuffer, 0, 100);

        //read at most 100 bytes from stdin into the sendbuffer
        fgets(sendbuffer, 100, stdin);

        //write the contents of the sendbuffer to the socket
        write(sockfd, sendbuffer, strlen(sendbuffer));

        //server receives the data in the sendbuffer
        //server echoes it back

        //read at most 100 bytes from the socket into recvbuffer
        read(sockfd, recvbuffer, 100);

        //print what the server sent back
        printf("%s", recvbuffer);
    }
    return 0;

}