#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(){

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

    printf("Enter the message to be sent to the server: ");
    fgets(request, sizeof(request), stdin);
    // modify the message to be sent to the server that it is not easy to sniff
    for(int i = 0; i < sizeof(request); i++){
        request[i] = request[i] + 1;
    }
    send(sock, request, sizeof(request), 0);
    
    close(sock);

    return 0;
}