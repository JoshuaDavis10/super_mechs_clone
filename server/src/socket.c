#include "socket.h"

#include "logger.h"
#include "asserts.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_BACKLOG 5 
#define PORT "5001"

typedef struct {
    i32 client_fd[2];
} connection_info;

static connection_info* ci = 0;

void* get_in_addr(struct sockaddr* sa);
i32 setup_socket();

//TODO: close socket

u8 init_sock_conn() {
    _ASSERT(ci == 0);

    ci = (connection_info*)malloc(sizeof(connection_info));
    memset(ci, 0, sizeof(connection_info));
    LTRACE("memory used for socket: %lu", sizeof(connection_info));

    int sockfd = setup_socket();
    LTRACE("socket created.");
    LINFO("waiting for clients to connect...");

    // wait for connections and fill in connection_info accordingly
    struct sockaddr_storage client_one_addr, client_two_addr; //used to store address info for client
    socklen_t client_socket_size = sizeof client_one_addr;
    int client_one_fd, client_two_fd; //ID for client socket

    //accept blocks until a connection comes in
    client_one_fd = accept(sockfd, (struct sockaddr*)&client_one_addr, &client_socket_size);
    _ASSERT(client_one_fd != -1);
    LINFO("player 1 successfully connected.");

    client_two_fd = accept(sockfd, (struct sockaddr*)&client_two_addr, &client_socket_size);
    _ASSERT(client_two_fd != -1);
    LINFO("player 2 successfully connected.");

    ci->client_fd[P1] = client_one_fd;
    ci->client_fd[P2] = client_two_fd;
    return true;
}

void free_socket() {
    _ASSERT(ci != 0);
    free(ci);
    ci = 0;
    LINFO("uninitialized socket info");
}

u8 send_msg(const char* msg, i32 client) {
    LTRACE("sending message: %s.", msg);
    if(send(ci->client_fd[client], msg, strlen(msg), 0) == -1) { _ASSERT(0); }
    return true;
}

u8 recv_msg(char* msg, i32 client) {
    _ASSERT(client == P1 || client == P2);
    char client_msg[MAX_MESSAGE_SIZE];
    int numbytes = recv(ci->client_fd[client], client_msg, MAX_MESSAGE_SIZE-1, 0);
    _ASSERT(numbytes != -1);
    client_msg[numbytes] = '\0';
    LTRACE("received message from client %d: %s", client, client_msg);
    strcpy(msg, client_msg);
    return true;
}

void* get_in_addr(struct sockaddr *sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

i32 setup_socket() {

    //declarations for socket
    struct addrinfo hints;      //will be used as 3rd param in getaddrinfo() (give getaddrinfo 'hints' about how u want to use it?)
    struct addrinfo* servinfo;  //pass as 4th param of getaddrinfo to get the linked list of addrinfo structs

    memset(&hints, 0, sizeof(struct addrinfo));// zero out memory
    hints.ai_family = AF_UNSPEC;        // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream
    hints.ai_flags = AI_PASSIVE;        // fill in IP for me (don't have to hardcode it)

    //getaddrinfo()
    //ig getaddrinfo should return 0 if its successful
    if((getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        printf("\e[1;31mERROR: getaddrinfo() did not return 0 as expected\e[0;37m\n");
        return -1;
    }

    //loop through addrinfo linked list until we can create and bind a socket with it
    struct addrinfo* ai_ptr;
    int sockfd; //socket ID?
    int yes = 1; //for setsockopt()... 4th param
    for(ai_ptr = servinfo; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
        //socket()
        if((sockfd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) == -1) {
            printf("\e[1;33mWARNING: socket() returned -1\e[0;37m\n");
            continue; //go to next addrinfo struct in linked list
        }
        //setsockopt() <- allows to rerun server without having to wait a minute for the socket to clear after last run
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            printf("\e[1;31mERROR: setsockopt() returned -1\e[0;37m\n");
            return -1;
        }
        //bind() <- associate socket with an actual port
        if(bind(sockfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen) == -1) {
            close(sockfd); //close the socket we created since we can't bind it 
            printf("\e[1;33mWARNING: bind() returned -1\e[0;37m\n");
            continue; //go to next addrinfo struct in linked list
        }
        
        break; //end loop once we've successfully created/bound a socket 
    }

    freeaddrinfo(servinfo); //done with addrinfo struct

    //check that socket was actually bound
    if(ai_ptr == NULL) {
        printf("\e[1;31mERROR: failed to bind\e[0;37m\n");
        return -1;
    }

    //listen() <- wait for incoming connections with 
    if(listen(sockfd, MAX_BACKLOG) == -1) {
        printf("\e[1;31mERROR: listen() returned -1\e[0;37m\n");
        return -1;
    }

    return sockfd;

}
