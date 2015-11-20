#include <socket_data_receiver/receiver.h>
#include <socket_connection/socket_client.h>
#include <lms/datamanager.h>
#include <socket_connection/socket_listener.h>
#include <socket_data/message_types.h>
#include <algorithm>

bool Receiver::cycle(){
    if(!m_connected){
        if(client != nullptr){
            client->reset();
        }else{
            client = new socket_connection::SocketClient(logger);
        }
        client->setSocketListener(this);
        if(!client->connectToServer(config->get<std::string>("ip","127.0.0.1"),getConfig()->get<int>("port",65111))){
            logger.warn("Not connected with server!");
            return true;
        }else{
            m_connected = true;
            registerChannelsAtServer();
        }
    }
    client->cycleClient();
    getDataFromServer(false);
    return true;
}
bool Receiver::initialize(){
    m_connected = false;
    config = getConfig();
    client = nullptr;

    registerChannelsAtDataManager();
    return true;
}

bool Receiver::deinitialize(){
    //TODO
    return false;
}


void Receiver::registerChannelsAtDataManager(){
    const std::vector<std::string> channels =  config->getArray<std::string>("dataChannels");
    if(channels.size()  == 0){
        logger.warn("registerChannelsAtServer") << "no channels to receive given!";
        return;
    }
    for(const std::string &channel:channels){
        datamanager()->getWriteAccess(this,channel);
    }
}

void Receiver::registerChannelsAtServer(){
    const std::vector<std::string> channels =  config->getArray<std::string>("dataChannels");
    //get write access to all channels you want to deserialize
    if(channels.size()  == 0){
        logger.warn("registerChannelsAtServer") << "no channels to receive given!";
        return;
    }
    //send register request!
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
        if(name == cM.name)
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
    (void)from;
    char type = buff[0];
    int channelID;
    logger.debug("Receiver::receivedMessage") << (int)type;
    switch ((MessageType)((int)type)) {
    case MessageType::CHANNEL_DATA:
        channelID = (int) buff[1];
        logger.debug("Receiver::receivedMessage") << "Channel : "<< getChannelName(channelID);
        for(ChannelMapping cM : m_channelMapping){
            if(cM.iD == channelID){
                //serialize channel
                std::istringstream is(std::string(&buff[2],bytesRead-2));
                if(!datamanager()->deserializeChannel(this,cM.name,is)){
                    logger.error("Couldn't deserialize channel")<<cM.name;
                }
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
    logger.debug("startGettingChannel") <<getChannelName(channelId);
    busyChannels.push_back(channelId);
}

std::string Receiver::getChannelName(char channelId){
    for(ChannelMapping cm:m_channelMapping){
        if(cm.iD == channelId){
            return cm.name;
        }
    }
    logger.error("getChannelName") << "No channel with the given Id: " <<channelId;
    return "";
}

void Receiver::gotChannel(char channelId){

    logger.debug("gotChannel") <<getChannelName(channelId);
    busyChannels.erase(std::remove(busyChannels.begin(),busyChannels.end(),channelId),busyChannels.end());
}

bool Receiver::isChannelBusy(char channelId){
    return std::find(busyChannels.begin(), busyChannels.end(), channelId) != busyChannels.end();
}

void Receiver::channelMapping(char* buff, int bytesRead){
    std::vector<std::string> channels = lms::extra::split(buff,bytesRead-1,';');
    for(size_t i = 0; i< channels.size(); i += 2){
        logger.debug("RECEIVER::CHANNELMAPPING") << channels[i] << " " << (int)(channels[i+1][0]);
        m_channelMapping.push_back(ChannelMapping(channels[i],(int)(channels[i+1][0])));
    }
}

void Receiver::disconnected(const socket_connection::SocketConnector &disconnected){
    (void)disconnected;
    m_connected = false;
    busyChannels.clear();
    m_channelMapping.clear();
}

void Receiver::connected(const socket_connection::SocketConnector &connected){
    (void)connected;
    m_connected = true;
}
