#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define SERVER_IP "127.0.0.1" // Change this to the IP address of the server
// using namespace std;

int main() {
    // create the socket
    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // setup an address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(3001);

    while(1){
        // Send the first message (numbers) to the server
        char message[256];
        printf("MESSAGE : ");
        fgets(message, sizeof(message), stdin);
        sendto(sock, message, strlen(message), 0, (struct sockaddr *) &server_address, sizeof(server_address));

        // Receive the result from the server
        recvfrom(sock, &message, sizeof(message), 0, NULL, NULL);
        if(message[0]=='e' && message[1]=='x' && message[2]=='i' && message[3]=='t'){
            printf("Server has exited\n");
            break;
        }
        // if(message=="exit")
        //     break;
        printf("\nReceived from Server: %s\n", message);
    }

    // close the socket
    close(sock);
    return 0;
}