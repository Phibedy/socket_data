#ifndef SOCKET_DATA_RECEIVER_H
#define SOCKET_DATA_RECEIVER_H
#include <lms/module.h>
#include <socket_connection/socket_connector.h>
#include <socket_connection/socket_client.h>
#include <socket_connection/socket_listener.h>

#include <map>
#include <string>
class Receiver:public lms::Module, public socket_connection::SocketListener{
public:

    bool initialize();
    bool deinitialize();

    bool cycle();

    void receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead);
    void disconnected(const socket_connection::SocketConnector &disconnected);
    void connected(const socket_connection::SocketConnector &connected);

    std::map<std::string, char> channelMapping;
    void registerChannelsAtServer(const std::vector<std::string> &channels);

    void getDataFromServer();
private:
    socket_connection::SocketClient *client;
};

#endif /*SOCKET_DATA_RECEIVER_H*/
