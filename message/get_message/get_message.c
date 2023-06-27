#include "get_message.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

// Retrieves the specified api message from the api_messages directory
// ready for sending to the client via websockets.
void get_message(char *msg, char *file_name) {
    char rel_path[100] = "extension/api_messages/";
    strcat(rel_path, file_name);

    FILE *init_file = fopen(rel_path, "r");
    assert(init_file != NULL);

    int length = file_size(init_file);
    fread(msg, 1, length, init_file);
    msg[length] = '\0';
}
