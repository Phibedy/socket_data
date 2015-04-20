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

    std::map<int,std::vector<int>> clientChannels;

    std::vector<ChannelMapping> channelMapping;
private:
    socket_connection::SocketServer *server;
    /**
     * @brief addChannel
     * @param name
     * @return the id of the channel
     */
    int addChannel(std::string name);
};

#endif /*SOCKET_DATA_RECEIVER_H*/
