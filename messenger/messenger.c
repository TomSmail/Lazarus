#include "messenger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sends message to phone number via whatsapp specified in local environment
int send_text_message(char* message) {
    // If this gives SEGFAULT make sure you have specified your
    // environment variables
    const char* TWILIO_NUMBER = getenv("TWILIO_NUMBER");
    const char* MY_NUMBER = getenv("MY_NUMBER");
    const char* ACCOUNT_SID = getenv("ACCOUNT_SID");
    const char* AUTH_TOKEN = getenv("AUTH_TOKEN");
    const int MAX_BUFFER_SIZE = 1024;

    // Constructs command
    char instruction[MAX_BUFFER_SIZE];
    strcpy(instruction, "./send_whatsapp.sh \"");
    strcat(instruction, MY_NUMBER);
    strcat(instruction, "\" \"");
    strcat(instruction, TWILIO_NUMBER);
    strcat(instruction, "\" \"");
    strcat(instruction, message);
    strcat(instruction, "\" \"");
    strcat(instruction, ACCOUNT_SID);
    strcat(instruction, "\" \"");
    strcat(instruction, AUTH_TOKEN);
    strcat(instruction, "\" #");  // comment out any garbage remaining in the buffer

    return system(instruction);
}
