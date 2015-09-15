#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include "client.h"

#define BUFFSIZE  1024

int sock;
struct sockaddr_in server;
char buffer[BUFFSIZE];

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int tcp_conn(char *ip, int port) {
    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        error("ERROR opening socket");
        return 1;
    }
    server.sin_addr.s_addr = inet_addr( ip );
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        error("connect error");
        return 1;
    }
    return 0;
}

int tcp_send(char *data) {
    //Send some data
    if(send(sock, data, strlen(data), 0) < 0)
    {
        error("Send failed");
        return 1;
    }
    return 0;
}


int tcp_recv(char **reply) {
    int bytes_read;
    *reply = (char *) malloc(sizeof(char));
    do
    {
        //Receive a reply from the server
        bytes_read = recv(sock, buffer, sizeof(buffer), 0);
        if(bytes_read == -1)
        {
            error("recv failed");
            return 1;
        }
        strcat(*reply, buffer);
        memset(buffer, 0, sizeof(buffer));
    } while (bytes_read > 0);
    return 0;
}

void tcp_close() {
    // Close connection
    close(sock);
}

// -------------------------------------

int main(int argc, char const *argv[])
{
    char *response;
    tcp_conn("127.0.0.1", 8000);
    tcp_send("GET / HTTP/1.1\r\n\r\n");
    tcp_recv(&response);
    printf("%s\n", response);
    return 0;
}