#include "defines.h"

#include "game_data.h"
#include "item_data.h"
#include "socket.h"
#include "logger.h"
#include "asserts.h"

//#include <mysql/mysql.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv) {

    /*
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char* server = "localhost";
    char* user = "jdavis";
    char* database = "super_mechs_db";

    conn = mysql_init(NULL);

    //TODO: print usage if they don't input password
    if(!mysql_real_connect(conn, server, user, argv[1], database, 0, NULL, 0)) {
        LERROR("%s\n", mysql_error(conn));
        exit(1);
    }

    if(mysql_query(conn, "show tables")) {
        LERROR("%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);

    LINFO("MySQL Tables in mysql database");
    while((row = mysql_fetch_row(res)) != NULL) { LINFO("%s", row[0]); }

    mysql_free_result(res);
    mysql_close(conn);
    */


    clock_t start, end;
    start = clock();

    if(!init_game_data()) {
        LERROR("failed to initialize game data. terminating program...");
        return -1;
    }
    if(!load_item_data()) {
        LERROR("failed to load item data. terminating program...");
        free_game_data();
        return -1;
    }

    LINFO("server startup complete.");

    if(!init_sock_conn()) {
        LERROR("failed to initialize socket connection. terminating program...");
        free_item_data();
        free_game_data();
        return -1;
    }
    if(!load_game_data()) {
        LERROR("failed to load game data. terminating program...");
        free_socket();
        free_item_data();
        free_game_data();
        return -1;
    }

    end = clock();
    LTRACE("cpu time used: %.2lfms", (((double)(end-start))/CLOCKS_PER_SEC) * 1000);

    char command[MAX_MESSAGE_SIZE];
    i8 result;
    u32 turn;
    u32 opp;
    recv_msg(command, P1);
    if(strcmp(command, "start") != 0) { LERROR("P1 failed to start"); return -1; }
    recv_msg(command, P2);
    if(strcmp(command, "start") != 0) { LERROR("P2 failed to start"); return -1; }
    while(true) {
        turn = get_turn();
        LDEBUG("turn: %d", turn);
        if(turn == P1) { opp = P2; }
        if(turn == P2) { opp = P1; }
        send_msg("go", turn);
        recv_msg(command, turn);
        result = process_command(command);
        if(result == 1) { 
            LINFO("player 1 wins."); 
            send_msg("win", P1);
            send_msg("loss", P2);
            break; 
        }
        else if(result == 2) 
        { 
            LINFO("player 2 wins."); 
            send_msg("loss", P1);
            send_msg("win", P2);
            break; 
        }
        else if(result == -1) {
            send_msg("invalid", turn);
            continue;
        }
        send_msg(command, opp);
        send_msg("valid", turn);
        recv_msg(command, turn);
        _ASSERT(strcmp(command, "ACK") == 0);
        recv_msg(command, opp);
        _ASSERT(strcmp(command, "ACK") == 0);
    }

    free_socket();
    free_item_data();
    free_game_data();
    LINFO("server shutdown complete.");
    LINFO("terminating program.");
    return 0;
}
