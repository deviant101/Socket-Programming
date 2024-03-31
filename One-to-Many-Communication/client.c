#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(){
    char request[256];
    char buf[200];

    // create the socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // setup an addressNational University of Computer and Emerging Sciences (NUCES) Islamabad
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3001);
    connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    
    // Send the first message (number) to the server
    printf("Enter a number: ");
    fgets(request, sizeof(request), stdin);
    send(sock, request, sizeof(request), 0);
    
    // Send the third message (number) to the server
    printf("Enter another number: ");
    fgets(request, sizeof(request), stdin);
    send(sock, request, sizeof(request), 0);
    
    // Receive the result from the server
    recv(sock, &buf, sizeof(buf), 0);
    printf("\nServer result: %s\n", buf);
    
    // close the socket
    close(sock);
    return 0;
}