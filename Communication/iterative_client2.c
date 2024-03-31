#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

int main(){

    char buf[200];
    char request[256];

    // create the socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    //setup an address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(3939);

    // connect to the server
    connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    
    recv(sock, &buf, sizeof(buf), 0);
    printf("%s\n",buf);

    for(int i=0; i<5; ++i){
        printf("\nEnter the message to be sent to the server: ");
        fgets(request, sizeof(request), stdin);
        send(sock, request, sizeof(request), 0);

        char server_message[200];
        recv(sock, &server_message, sizeof(server_message), 0);
        printf("Received : ");
        printf("%s", server_message);
        printf("\n");
    }
    close(sock);

    return 0;
}