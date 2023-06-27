#include "socket.h"

#include <stdio.h>

#include "get_message.h"
#include "strike.h"
#include "ws.h"

#define MSG_BUFF_SIZE 10000  // TODO: need to get real size
#define UNUSED(x) (void)x;

static char msg_buf[MSG_BUFF_SIZE];

static int num_clients = 0;
static ws_cli_conn_t *clients[MAX_CLIENTS];  // Defined in library

// Gets and sends an api message from the name of the json file we want to send
// and a client to send it to
void send_message(ws_cli_conn_t *client, char *message_name) {
    get_message(msg_buf, message_name);
    ws_sendframe_txt(client, msg_buf);
}

// Sends the message to all active clients
void broadcast(char *msg) {
    for (int i = 0; i < num_clients; ++i) {
        ws_sendframe_txt(clients[i], msg);
    }
}

// sends the named json message to all active clients
void broadcast_by_name(char *msg_name) {
    get_message(msg_buf, msg_name);
    puts("Broadcasting message: ");
    puts(msg_name);
    puts(msg_buf);
    for (int i = 0; i < num_clients; ++i) {
        ws_sendframe_txt(clients[i], msg_buf);
    }
}

// Handlers for use with wsServer

void onopen(ws_cli_conn_t *client) {
    clients[num_clients] = client;
    num_clients++;

    send_message(client, "nodes.json");
    send_message(client, "prev_strikes.json");
    ws_sendframe_txt(client, msg_buf);
}

void onclose(ws_cli_conn_t *client) {
    bool found = false;
    for (int i = 0; i < num_clients; ++i) {
        if (found) {
            clients[i - 1] = clients[i];
        } else if (clients[i] == client) {
            found = true;
        }
    }
    num_clients--;
    puts("Closed");
}

void onmessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size,
               int type) {
    // (Note: Since we only have one type of message at the moment, we don't
    // perform message type checking) In particular, assume that the user wanted
    // to check off the strike with the passed ID.
    UNUSED(client)
    UNUSED(size)
    UNUSED(type)

    const char *strike_id = (const char *)msg;
    puts(strike_id);  // Debug

    // Update our records
    checkoff_strike(strike_id);

    // Grab the template and fill it in
    get_message(msg_buf, "cancel.json");
    sprintf(msg_buf, msg_buf, strike_id);

    // Send to everyone
    broadcast(msg_buf);
}
