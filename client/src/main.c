#include "defines.h"

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>

#define PORT "5001"

#define MAX_MESSAGE_SIZE 1000

SOCKET sock = INVALID_SOCKET;
        
u32 conn_to_serv(const char* hostname) {

    WSADATA wsaData;
    struct addrinfo hints, *servinfo, *ai_ptr;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    //getaddrinfo()
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(hostname, PORT, &hints, &servinfo) != 0) {
        printf("\e[1;31mERROR: getaddrinfo() did not return 0 as expected\e[0;37m\n");
        WSACleanup();
        return false;
    }

    //loop to addrinfo that will work
    for(ai_ptr = servinfo; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {

        //socket()
        if((sock = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) == INVALID_SOCKET) {
            printf("\e[1;31mERROR: socket() returned -1\e[0;37m\n");
            WSACleanup();
            return false;
        }

        //connect()
        if(connect(sock, ai_ptr->ai_addr, ai_ptr->ai_addrlen) == SOCKET_ERROR) {
            printf("\e[1;31mERROR: connect() returned -1\e[0;37m\n");
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }

        break; //found good addrinfo and connected successfully
    }
    
    if(ai_ptr == NULL) {
            printf("\e[1;31mERROR: connection failed\e[0;37m\n");
            return false;
    }

    printf("DEBUG: connected to server\n");

    freeaddrinfo(servinfo); //done with addressinfo

    if(sock == INVALID_SOCKET) {
        printf("ERROR: unable to connect to server!\n");
        WSACleanup();
        return false;
    }

    return true;
}

u32 recv_msg(char* msg) {
    i32 numbytes = recv(sock, msg, MAX_MESSAGE_SIZE-1, 0);
    if(numbytes == -1) {
        printf("[ERROR]: send() returned -1\n");
        return false;
    }
    msg[numbytes] = '\0';
    return true;
}

int main() {
    char buffer[MAX_MESSAGE_SIZE];
    conn_to_serv("josh-Inspiron-15-5510");
    recv_msg(buffer);
    printf("received: %s", buffer); 
    return 0;
}
