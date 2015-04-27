#ifndef SOCKET_DATA_RECEIVER_H
#define SOCKET_DATA_RECEIVER_H
#include <lms/module.h>
#include <socket_connection/socket_server.h>
#include <vector>
#include <map>
#include <socket_data/message_types.h>
#include <string>
#include <socket_connection/socket_listener.h>
class Sender:public lms::Module, public socket_connection::SocketListener{
public:

    bool initialize();
    bool deinitialize();

    bool cycle();

    void receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead);
    void disconnected(const socket_connection::SocketConnector &disconnected);
    void connected(const socket_connection::SocketConnector &connected);
    /**
     * @brief clientChannels first is the iD of the client, second values are the iDs for the dataChannels
     */
    std::map<int,std::vector<char>> clientChannels;

    /**
     * @brief channelMapping mapping channelID and channelName (not that fancy)
     */
    std::vector<ChannelMapping> channelMapping;

    void sendChannelsToClient(socket_connection::SocketConnector &from);
private:
    socket_connection::SocketServer *server;
    /**
     * @brief addChannel
     * @param name
     * @return the id of the channel
     */
    char addChannel(std::string name);
};

#endif /*SOCKET_DATA_RECEIVER_H*/
