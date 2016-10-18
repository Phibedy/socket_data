#ifndef SOCKET_DATA_MESSAGE_TYPES
#define SOCKET_DATA_MESSAGE_TYPES
#include <string>

#include <vector>
enum class MessageType{
    MESSAGE = 0, ERROR = 100, REGISTER_CHANNEL = 10, CHANNEL_MAPPING = 11,CHANNEL_DATA = 12,GET_CHANNEL_DATA = 13,GET_CHANNEL_DATA_ALL = 14
};

struct ChannelMapping{
    ChannelMapping(){}
    ChannelMapping(std::string name, char iD):name(name),iD(iD){}
    std::string name;
    char iD;
};
typedef std::vector<ChannelMapping> ChannelMappingArray;

//TODO
inline std::vector<std::string> split(const char *string, int strLength,char splitter) {
    std::vector<std::string> result;
    const char *resultBuff = string;
    int oldFound = 0;
    for (int i = 0; i < strLength; i++) {
    if (resultBuff[i] == splitter) {
    // found new part
    std::string name(&resultBuff[oldFound], i - oldFound);
    oldFound = i + 1;
    result.push_back(name);
    }
    }
    // add last element if the string doesn't end with the splitter
    if (oldFound < strLength) {
    std::string name(&resultBuff[oldFound], strLength - oldFound);
    result.push_back(name);
    }
    return result;
}

#endif
