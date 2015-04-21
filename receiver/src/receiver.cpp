#include <socket_data_receiver/receiver.h>
#include <socket_connection/socket_client.h>
#include <lms/datamanager.h>
#include <socket_connection/socket_listener.h>
#include <socket_data/message_types.h>

bool Receiver::cycle(){
    client->cycleClient();
    return true;
}

bool Receiver::initialize(){
    client = new socket_connection::SocketClient(&logger);
    client->connectToServer("127.0.0.1",65111);
    client->setSocketListener(this);

    //get write access to all channels you want to deserialize
    std::vector<std::string> channels =  getConfig()->getArray<std::string>("dataChannels");
    for(std::string &channel:channels){
        datamanager()->getWriteAccess(this,channel);
    }
    if(channels.size() > 0){
        registerChannelsAtServer(channels);
    }else{
        logger.warn("init") << "no channels to receive given!";
    }
    return true;
}

bool Receiver::deinitialize(){
    //TODO
    return false;
}

void Receiver::registerChannelsAtServer(const std::vector<std::string> &channels){
    std::string toSend(1,(char)MessageType::REGISTER_CHANNEL);
    for(const std::string &channel:channels){
        toSend += channel;
        toSend += ";";
    }
    client->sendMessageToAllServers(toSend.c_str(),toSend.size(),true);
}

void Receiver::getDataFromServer(){
    std::string toSend =std::to_string((int)MessageType::GET_CHANNEL_DATA);
    client->sendMessageToAllServers(toSend.c_str(),toSend.size(),true);
}

void Receiver::receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead){
    char type = buff[0];
    logger.debug("Receiver::receivedMessage") << (int)type;
    switch ((MessageType)((int)type)) {
    case MessageType::CHANNEL_DATA:
        break;
    case MessageType::MESSAGE:

        break;
    case MessageType::CHANNEL_MAPPING:
        //channelMapping

        break;
    case MessageType::ERROR:

        break;
    case MessageType::GET_CHANNEL_DATA:
        //shouldn't be called on the receiver!
        break;
    case MessageType::REGISTER_CHANNEL:
        //shouldn't be called on the receiver!
        break;
    default:
        break;
    }
}

void Receiver::disconnected(const socket_connection::SocketConnector &disconnected){

}

void Receiver::connected(const socket_connection::SocketConnector &connected){

}
