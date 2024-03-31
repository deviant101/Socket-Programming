#include <stdio.h> // basic C header
#include <stdlib.h>
#include <sys/types.h> // provides required data types
#include <sys/socket.h> // holds address family and socket functions
#include <unistd.h>
#include <netinet/in.h> // has the sockaddr_in structure

int calculator(int operand1, int operand2, char op);

int main(){

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
    int operand1=0, operand2=0;
    char op;

    recv(client_socket, &operand1, sizeof(operand1), 0);
    printf("Received 1st Number : %d\n",operand1);

    recv(client_socket, &op, sizeof(op), 0);
    printf("Received Operator : %c\n",op);

    recv(client_socket, &operand2, sizeof(operand2), 0);
    printf("Received 2nd Number : %d\n",operand2);

    // close the socket
    printf("%d %c %d = %d \n",operand1,op,operand2,calculator(operand1,operand2,op));
    close(server_socket);

    return 0;
}

int calculator(int operand1, int operand2, char op){
    switch(op){
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            return operand1 / operand2;
        default:
            return 0;
    }
}