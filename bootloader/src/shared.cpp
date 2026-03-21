#include "shared.h"

void log(const char* msg, Print& stream) {
    stream.println(msg);
}

void log(const String& msg, Print& stream) {
    stream.println(msg);
}

void exception(const char* msg, Print& stream) {
    while (true) {
        // flash the message every second
        stream.println(msg);
        delay(1000);
        stream.flush();
        delay(1000);
    }
}

