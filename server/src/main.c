#include "defines.h"

#include "game_data.h"
#include "socket.h"
#include "logger.h"

#include <time.h>

int main() {

    /*
    my goal is for main to look like this:

        startup_sever();
            - initialize game_data to 0
        connect_clients();
            - connect 2 sockets, 1 for each client
        load_data();
            - clients sends item IDs of items they'll use
            - server sends item IDs of client 1 to client 2 and vice versa
        while(true) {
            receive 'f' from both clients then send an 'rc' to the client whose turn it is
            receive_request();
            i8 result = process_request();
            send_request(); //i.e. ready for command ('rc')
            if(result == 1) {
                *player 1 wins*
                break;
            }
            if(result == 2) {
                *player 2 wins*
                break;
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

    init_game_data();
    //TODO: load_item_data();  <-- this loads all the items from the item file into arrays for quick access
    init_sock_conn();
    //TODO: load_game_data();  <-- load data from clients into game_data

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    LINFO("server startup complete.");
    LTRACE("cpu time used: %.2lfms", cpu_time_used);

    char command[MAX_MESSAGE_SIZE];
    i8 result;
    u32 turn;
    while(true) {
        turn = get_turn();
        recv_msg(command, turn);
        result = process_command(command);
        if(result == 1) {
            LINFO("player 1 wins.");
            break;
        }
        if(result == 2) {
            LINFO("player 2 wins.");
            break;
        }
    }

    /*
    my goal is for main to look like this:

        startup_sever();
            - initialize game_data to 0
        connect_clients();
            - connect 2 sockets, 1 for each client
        load_data();
            - clients sends item IDs of items they'll use
            - server sends item IDs of client 1 to client 2 and vice versa
        while(true) {
            receive 'f' from both clients then send an 'rc' to the client whose turn it is
            receive_request();
            i8 result = process_request();
            send_request(); //i.e. ready for command ('rc')
            if(result == 1) {
                *player 1 wins*
                break;
            }
            if(result == 2) {
                *player 2 wins*
                break;
            }
        }

        send/receive requests are functions in socket.h
        process_request will be in this file
        startup_server() is just allocating memory for game data
        connect_clients() would be setting up sockets and waiting for client connections
        load_data() populates all the game_data (g_data) struct by asking clients for info and then randomly choosing start positions

    */
    free_game_data();
    LINFO("server shutdown complete.");
    LINFO("terminating program.");
    return 0;
}
