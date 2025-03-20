#include "game_data.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

game_data* g_data = 0;

//return -1 on error (as in the request is valid but the action implied by the request can't be carried out)
//return 1 if P1 wins as a result of request
//return 2 if P2 wins as a result of request
//return 0 otherwise.
i8 process_request(const char* req); 

int main() {

    /*
    my goal is for main to look like this:

        startup_sever();
        connect_clients();
        load_data();
        while(*game hasn't been won yet*) {
            receive_request();
            i8 result = process_request();
            send_request(); //i.e. ready for command ('rc')
            if(result == 1) {
                *player 1 wins*
            }
            if(result == 2) {
                *player 2 wins*
            }
        }

        send/receive requests are functions in socket.h
        process_request will be in this file
        startup_server() is just allocating memory for game data
        connect_clients() would be setting up sockets and waiting for client connections
        load_data() populates all the game_data (g_data) struct by asking clients for info and then randomly choosing start positions

    */
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    LINFO("starting server...");
    g_data = (game_data*)malloc(sizeof(game_data));
    LTRACE("allocated memory for server data (%lu bytes)", sizeof(game_data));
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    LTRACE("cpu time used: %.2lf", cpu_time_used);
    LINFO("shutting down server...");
    return 0;
}

i8 process_request(const char* req) {
    return 0;
}
