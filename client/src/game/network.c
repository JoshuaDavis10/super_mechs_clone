#include "network.h"

#include "../tools/logger.h"
#include "../tools/socket.h"
#include "../tools/asserts.h"
#include "../tools/event.h"

#include <string.h>

enum {
    NET_STATE_RECV,
    NET_STATE_WAIT_FOR_INPUT,
    NET_STATE_WAIT_FOR_VALID,
    NET_STATE_WAIT_FOR_UI,
    NET_NUM_STATES
};

//TODO: turn this all into  anetwork state struct
static i32 network_state = NET_STATE_RECV;
static char network_message_buffer[MAX_MESSAGE_SIZE];
static i32 network_action;

u8 on_ui_input(i32 event_type, void* listener, void* sender, event_data data);
u8 on_ui_complete(i32 event_type, void* listener, void* sender, event_data data);

u8 startup_network(char* hostname) {
    if(!conn_to_serv(hostname)) {
        LERROR("failed to connect to server.");
        return false;
    }
    SM_ASSERT(network_state == NET_STATE_RECV);

    register_event(EVENT_UI_GOT_INPUT, on_ui_input, &network_state);
    register_event(EVENT_UI_COMPLETE, on_ui_complete, &network_state);

    LINFO("network startup complete.");
    LNET("NET_STATE_WAIT_FOR_RECV");

    return true;
}

void shutdown_network() {
    LINFO("shutdown netowrk.");
    //TODO: close socket connection?
}

u8 update_network() {
    event_data e_data;
    e_data.data[0] = 0;
    e_data.data[1] = 0;
    switch(network_state) {
        case NET_STATE_RECV:
            if(check_for_msg()) {
                recv_msg(network_message_buffer);
                if(strcmp(network_message_buffer, "go") == 0) {
                    LNET("NET_STATE_WAIT_FOR_INPUT");
                    fire_event(EVENT_NET_READY_FOR_INPUT, NULL, e_data);
                    network_state = NET_STATE_WAIT_FOR_INPUT;
                }
                else if(strcmp(network_message_buffer, "win") == 0) {
                    LINFO("YOU WIN!!!");
                    return 2; //2 for win
                }
                else if(strcmp(network_message_buffer, "loss") == 0) {
                    LINFO("you lose...");
                    return 3; //3 for loss
                }
                else {
                    if(strcmp(network_message_buffer, "w1")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_W1;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "w2")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_W2;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "w3")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_W3;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "w4")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_W4;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "w5")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_W5;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "w6")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_W6;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "s")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_S;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "g")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_G;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "ch")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_CH;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t0")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T0;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t1")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T1;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t2")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T2;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t3")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T3;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t4")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T4;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t5")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T5;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t6")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T6;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t7")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T7;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t8")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T8;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "t9")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_T9;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "da")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_DA;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "dd")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_DD;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "c")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_C;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "r1")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_R1;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "r2")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_R2;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "r3")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_R3;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "l1")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_L1;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "l2")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_L2;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else if(strcmp(network_message_buffer, "l3")==0){e_data.data[0]=P2;e_data.data[1]=ACTION_L3;network_state=NET_STATE_WAIT_FOR_UI;LNET("NET_STATE_WAIT_FOR_UI");fire_event(EVENT_ACTION_TAKEN, NULL, e_data);}
                    else {
                        SM_ASSERT_MSG(0, "got an invalid command from server");
                    }
                }
            }
            break;
        case NET_STATE_WAIT_FOR_VALID:
            if(check_for_msg()) {
                recv_msg(network_message_buffer);
                if(strcmp(network_message_buffer, "valid") == 0) {
                    e_data.data[0] = P1;
                    e_data.data[1] = network_action; 
                    network_state = NET_STATE_WAIT_FOR_UI;
                    fire_event(EVENT_ACTION_TAKEN, NULL, e_data);
                    LNET("NET_STATE_WAIT_FOR_UI");
                }
                else if(strcmp(network_message_buffer, "win") == 0) {
                    LINFO("YOU WIN!!!");
                    return 2; //2 for win
                }
                else if(strcmp(network_message_buffer, "loss") == 0) {
                    LINFO("you lose...");
                    return 3; //3 for loss
                }
                else {
                    SM_ASSERT_MSG(0, "sent an invalid message to server");
                }
            }
            break;
        case NET_STATE_WAIT_FOR_INPUT:
        case NET_STATE_WAIT_FOR_UI:
            break;
    }
    return true;
}

u8 on_ui_input(i32 event_type, void* listener, void* sender, event_data data) {
    SM_ASSERT(event_type == EVENT_UI_GOT_INPUT);
    char msg[MAX_MESSAGE_SIZE];
    switch(data.data[0]) {
        case ACTION_W1: strcpy(msg, "w1");  break; 
        case ACTION_W2: strcpy(msg, "w2");  break;
        case ACTION_W3: strcpy(msg, "w3");  break;
        case ACTION_W4: strcpy(msg, "w4");  break;
        case ACTION_W5: strcpy(msg, "w5");  break;
        case ACTION_W6: strcpy(msg, "w6");  break;
        case ACTION_S: strcpy(msg, "s"); break;
        case ACTION_G: strcpy(msg, "g"); break;
        case ACTION_CH: strcpy(msg, "ch");  break;
        case ACTION_T0: strcpy(msg, "t0");  break;
        case ACTION_T1: strcpy(msg, "t1");  break;
        case ACTION_T2: strcpy(msg, "t2");  break;
        case ACTION_T3: strcpy(msg, "t3");  break;
        case ACTION_T4: strcpy(msg, "t4");  break;
        case ACTION_T5: strcpy(msg, "t5");  break;
        case ACTION_T6: strcpy(msg, "t6");  break;
        case ACTION_T7: strcpy(msg, "t7");  break;
        case ACTION_T8: strcpy(msg, "t8");  break;
        case ACTION_T9: strcpy(msg, "t9");  break;
        case ACTION_DD: strcpy(msg, "dd");  break;
        case ACTION_DA: strcpy(msg, "da");  break;
        case ACTION_C: strcpy(msg, "c"); break;
        case ACTION_R1: strcpy(msg, "r1");  break;
        case ACTION_R2: strcpy(msg, "r2");  break;
        case ACTION_R3: strcpy(msg, "r3");  break;
        case ACTION_L1: strcpy(msg, "l1");  break;
        case ACTION_L2: strcpy(msg, "l2");  break;
        case ACTION_L3: strcpy(msg, "l3");  break;
    }
    network_action = data.data[0];
    send_msg(msg);
    i32* net_state = (i32*)listener;
    LNET("NET_STATE_WAIT_FOR_VALID");
    *net_state = NET_STATE_WAIT_FOR_VALID;
    return true;
}

u8 on_ui_complete(i32 event_type, void* listener, void* sender, event_data data) {
    i32* net_state = (i32*)listener;
    *net_state = NET_STATE_RECV;
    send_msg("ACK");
    LNET("NET_STATE_WAIT_FOR_RECV");
    return true;
}
