#include "socket.h"

#include "logger.h"
#include "asserts.h"

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h> //TODO: use logger

#define PORT "5001"

static SOCKET sock = INVALID_SOCKET;

u32 conn_to_serv(const char* hostname) {

    WSADATA wsaData;
    struct addrinfo hints, *servinfo, *ai_ptr;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    //getaddrinfo()
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(hostname, PORT, &hints, &servinfo) != 0) {
        LERROR("getaddrinfo() did not return 0 as expected");
        WSACleanup();
        return false;
    }

    //loop to addrinfo that will work
    for(ai_ptr = servinfo; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {

        //socket()
        if((sock = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) == INVALID_SOCKET) {
            LERROR("socket() returned -1");
            WSACleanup();
            return false;
        }

        //connect()
        if(connect(sock, ai_ptr->ai_addr, ai_ptr->ai_addrlen) == SOCKET_ERROR) {
            LERROR("connect() returned -1");
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }

        break; //found good addrinfo and connected successfully
    }
    
    if(ai_ptr == NULL) {
        LERROR("failed to connect to server");
        return false;
    }

    LINFO("connected to server");

    freeaddrinfo(servinfo); //done with addressinfo

    if(sock == INVALID_SOCKET) {
        LERROR("unable to connect to server");
        WSACleanup();
        return false;
    }

    return true;
}

u32 recv_msg(char* msg) {
    i32 numbytes = recv(sock, msg, MAX_MESSAGE_SIZE-1, 0);
    if(numbytes == -1) {
        LERROR("send() returned -1");
        return false;
    }
    SM_ASSERT(numbytes != 0); //crash if server isn't sending messages
    msg[numbytes] = '\0';
    LDEBUG("received message: %s", msg);
    return true;
}

u32 send_msg(const char* msg) {
    LDEBUG("sending message: %s", msg);
    if(send(sock, msg, strlen(msg), 0) == -1) {
        return false;
    }
    return true;
}

i8 check_for_msg() {
    TIMEVAL timeout;
    timeout.tv_usec = 1000; //1000 microseconds == 1 millisecond
    fd_set read;
    FD_ZERO(&read);
    FD_SET(sock, &read);
    i32 result = select(0, &read, NULL, NULL, &timeout);
    if(result == SOCKET_ERROR) {
        return -1;
    }
    if(result == 0) {
        return false;
    }
    return true;
}
