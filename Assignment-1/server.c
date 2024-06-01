#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(){
    char buf[200];

    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3939);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    // listen for connections
    listen(server_socket, 5);

    // accept the connection
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    // communication
    recv(client_socket, &buf, sizeof(buf), 0);
    for(int i=0; i<sizeof(buf); i++){
        buf[i] = buf[i] - 1;
    }
    printf("\n%s\n", buf);

    // close the socket
    close(server_socket);

    return 0;
}