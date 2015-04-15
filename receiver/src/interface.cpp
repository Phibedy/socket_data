#include <socket_data_receiver/receiver.h>

extern "C" {
void* getInstance () {
    return new Receiver();
}
}
