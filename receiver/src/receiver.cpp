#include <socket_data_receiver/receiver.h>
#include <socket_connection/socket_client.h>
#include <lms/datamanager.h>
#include <socket_connection/socket_listener.h>
#include <socket_data/message_types.h>
#include <algorithm>

bool Receiver::cycle(){
    client->cycleClient();
    getDataFromServer(false);
    return true;
}

bool Receiver::initialize(){
    client = new socket_connection::SocketClient(&logger);
    client->connectToServer(getConfig()->get<std::string>("ip","127.0.0.1"),getConfig()->get<int>("port",65111));
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

void Receiver::getDataFromServer(char channelID){
    //add type
    std::string toSend(1,(char)MessageType::GET_CHANNEL_DATA);
    //add channelID
    toSend += channelID;
    client->sendMessageToAllServers(toSend.c_str(),toSend.size(),true);
    //set channel busy
    startGettingChannel(channelID);
}
void Receiver::getDataFromServer(std::string channelName){
    char id = getChannelIdFromName(channelName);
    if(id == -1){
        //TODO error handling
        return;
    }
    getDataFromServer(id);
}

char Receiver::getChannelIdFromName(std::string name){
    for(ChannelMapping cM : m_channelMapping){
        return cM.iD;
    }
    return -1;
}

void Receiver::getDataFromServer(bool force){
    if(m_channelMapping.size() == 0){
        logger.info("getDataFromServer") << "No channels registered yet on the server";
        return;
    }
    if(force){
        std::string toSend(1,(char)MessageType::GET_CHANNEL_DATA_ALL);
        client->sendMessageToAllServers(toSend.c_str(),toSend.size(),true);
    }else{
        for(ChannelMapping cM:m_channelMapping){
            if(!isChannelBusy(cM.iD)){
                getDataFromServer(cM.iD);
            }else{
                logger.debug("CHANNEL BUSY") << cM.name;
            }
        }
    }
}

void Receiver::receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead){
    char type = buff[0];
    int channelID;
    logger.debug("Receiver::receivedMessage") << (int)type;
    switch ((MessageType)((int)type)) {
    case MessageType::CHANNEL_DATA:
        channelID = (int) buff[1];
        logger.debug("Receiver::receivedMessage") << "CHANNEL_MAPPING iD: "<< channelID;
        for(ChannelMapping cM : m_channelMapping){
            if(cM.iD == channelID){
                //serialize channel
                std::istringstream is(std::string(&buff[2],bytesRead-2));
                datamanager()->deserializeChannel(this,cM.name,is);
                logger.debug("Receiver::receivedMessage") << "CHANNEL_MAPPING: name" << cM.name;
                gotChannel(cM.iD);
            }
        }
        break;
    case MessageType::MESSAGE:

        break;
    case MessageType::CHANNEL_MAPPING:
        //channelMapping
        logger.debug("Receiver::receivedMessage") << "CHANNEL_MAPPING";
        channelMapping(&buff[1],bytesRead-1);
        break;
    case MessageType::ERROR:

        break;
    case MessageType::GET_CHANNEL_DATA_ALL:
        //shouldn't be called on the receiver!
        break;
    case MessageType::REGISTER_CHANNEL:
        //shouldn't be called on the receiver!
        break;
    default:
        break;
    }
}


void Receiver::startGettingChannel(char channelId){
    busyChannels.push_back(channelId);
}

void Receiver::gotChannel(char channelId){
    busyChannels.erase(std::remove(busyChannels.begin(),busyChannels.end(),channelId),busyChannels.end());
}

bool Receiver::isChannelBusy(char channelId){
    return std::find(busyChannels.begin(), busyChannels.end(), channelId) != busyChannels.end();
}

void Receiver::channelMapping(char* buff, int bytesRead){
    std::vector<std::string> channels = lms::extra::split(buff,bytesRead-1,';');
    for(int i = 0; i< channels.size(); i += 2){
        logger.debug("RECEIVER::CHANNELMAPPING") << channels[i] << " " << (int)(channels[i+1][0]);
        m_channelMapping.push_back(ChannelMapping(channels[i],(int)(channels[i+1][0])));
    }
}

void Receiver::disconnected(const socket_connection::SocketConnector &disconnected){

}

void Receiver::connected(const socket_connection::SocketConnector &connected){

}
