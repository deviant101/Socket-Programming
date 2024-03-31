#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h> // holds address family and socket functions
#include <unistd.h>
#include <netinet/in.h> // has the sockaddr_in structure
#include <cstring>

void reverse_message(char *message, char *reversed_message);

int main(){
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
    
    // send a welcome message to the client
    if(client_socket<0){
        printf("Error accepting client connection.\n");
        return 1;
    }
    char welcome_message[] = "Hi, you have connected to the server!";
    send(client_socket, welcome_message, sizeof(welcome_message), 0);

    for(int i=0; i<10; ++i){
        
        char buf[200];
        recv(client_socket, &buf, sizeof(buf), 0);

        if(i%2==1)
            continue;
        printf("Received message from client: %s \n", buf);

        // sending reversed string to client
        char server_message[200];
        reverse_message(buf, server_message);
        send(client_socket, server_message, sizeof(server_message), 0);
    }

    printf("Client connected.\n");

    // close the client socket
    close(client_socket);

    return 0;
}
void reverse_message(char *message, char *reversed_message){

    int len = strlen(message);
    int index;
    for(index=0; index<len; index++){
        reversed_message[index] = message[len-index-1];
    }
    reversed_message[index] = '\0';
}
