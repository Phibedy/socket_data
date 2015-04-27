#ifndef SOCKET_DATA_MESSAGE_TYPES
#define SOCKET_DATA_MESSAGE_TYPES
#include <string>

#include <vector>
enum class MessageType{
    MESSAGE = 0, ERROR = 100, REGISTER_CHANNEL = 10, CHANNEL_MAPPING = 11,CHANNEL_DATA = 12,GET_CHANNEL_DATA = 13
};

struct ChannelMapping{
    ChannelMapping(){}
    ChannelMapping(std::string name, char iD):name(name),iD(iD){}
    std::string name;
    char iD;
};
typedef std::vector<ChannelMapping> ChannelMappingArray;

#endif
