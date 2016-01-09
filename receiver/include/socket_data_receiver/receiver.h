#ifndef SOCKET_DATA_RECEIVER_H
#define SOCKET_DATA_RECEIVER_H
#include <lms/module.h>
#include <socket_connection/socket_connector.h>
#include <socket_connection/socket_connection_handler.h>
#include <socket_connection/socket_listener.h>
#include <socket_data/message_types.h>
#include "lms/type/module_config.h"

#include <map>
#include <string>
class Receiver:public lms::Module, public socket_connection::SocketListener{
public:


    /**
     * @brief m_connected true if the receiver is connected to a server
     */
    bool m_connected;

    bool initialize();
    bool deinitialize();

    bool cycle();

    //interface methods
    void receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead) override;
    void disconnected(const socket_connection::SocketConnector &disconnected) override;
    void connected(const socket_connection::SocketConnector &connected) override;

    /**
     * @brief registerChannelsAtServer registers channels at server, they are available if((after the next server-cycle - server processed messages) && (in the next cycle of the client))
     * @param channels
     */
    void registerChannelsAtServer();
    /**
     * @brief registerChannelsAtDataManager registers channels at the datamanager (grants serialize-access)
     */
    void registerChannelsAtDataManager();

    /**
     * @brief getDataFromServer asks for all data from the server
     * @param force if set to true, the client will also ask for channels that he asked before and haven't received yet
     */
    void getDataFromServer(bool force);
    /**
     * @brief getDataFromServer asks for the channel given by the channelID
     * @param channelID
     */
    void getDataFromServer(char channelID);
    /**
     * @brief getDataFromServer asks for the channel given by the channelName
     * @param channelName
     */
    void getDataFromServer(std::string channelName);
private:
    socket_connection::SocketConnectionHandler *client;
    std::vector<ChannelMapping> m_channelMapping;
    std::vector<int> busyChannels;

    void startGettingChannel(char channelId);

    void gotChannel(char channelId);
    bool isChannelBusy(char channelId);
    std::string getChannelName(char channelId);

    void channelMapping(char* buff, int bytesRead);
    /**
     * @brief getChannelIdFromName
     * @param name
     * @return the iD of the channel or -1 if the channel wasn't found
     */
    char getChannelIdFromName(std::string name);
};

#endif /*SOCKET_DATA_RECEIVER_H*/
