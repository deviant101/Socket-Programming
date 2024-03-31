#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(){

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

    int operand=0;
    char op;
    printf("Enter 1st operand : ");
    scanf("%d", &operand);
    send(sock, &operand, sizeof(operand), 0);


    printf("Enter a character : ");
    scanf(" %c", &op);
    send(sock, &op, sizeof(op), 0);

    printf("Enter 2nd operand : ");
    scanf("%d", &operand);
    send(sock, &operand, sizeof(operand), 0);

    close(sock);

    return 0;
}
