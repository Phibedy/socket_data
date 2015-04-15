#ifndef SOCKET_DATA_MESSAGE_TYPES
#define SOCKET_DATA_MESSAGE_TYPES
#include <string>

enum class MessageType{
    MESSAGE = 0, ERROR = 100, REGISTER_CHANNEL = 10, CHANNEL_MAPPING = 11,CHANNEL_DATA = 12
};

struct channelMapping{
    std::string name;
    char iD;
};

#endif
