#ifndef SOCKET_H
#define SOCKET_H

#include "ws.h"
extern void onmessage(ws_cli_conn_t* client, const unsigned char* msg,
                      uint64_t size, int type);
extern void onclose(ws_cli_conn_t* client);
extern void onopen(ws_cli_conn_t* client);
extern void broadcast(char* msg);
extern void send_message(ws_cli_conn_t* client, char* message_name);
extern void broadcast_by_name(char* msg_name);
extern void broadcast(char* msg);

#endif
