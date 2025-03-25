#include "defines.h"

#include "game_data.h"
#include "item_data.h"
#include "socket.h"
#include "logger.h"

#include <time.h>

int main() {
    clock_t start, end;
    start = clock();

    init_game_data();
    load_item_data();

    end = clock();
    LINFO("server startup complete.");
    LTRACE("cpu time used: %.2lfms", (((double)(end-start))/CLOCKS_PER_SEC) * 1000);

    init_sock_conn();
    send_msg("*message from server*", P1);
    send_msg("*message from server*", P2);
    //TODO: load_game_data();  <-- load data from clients into game_data


    /*
    char command[MAX_MESSAGE_SIZE];
    i8 result;
    u32 turn;
    while(true) {
        turn = get_turn();
        recv_msg(command, turn);
        result = process_command(command);
        if(result == 1) { LINFO("player 1 wins."); break; }
        if(result == 2) { LINFO("player 2 wins."); break; }
    }
    */

    //TODO: close socket
    free_socket();
    free_item_data();
    free_game_data();
    LINFO("server shutdown complete.");
    LINFO("terminating program.");
    return 0;
}
