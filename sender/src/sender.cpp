#include <socket_data_sender/sender.h>
#include <string>
#include <socket_data/message_types.h>
#include <lms/extra/string.h>
bool Sender::cycle(){
    server->cycle();
    return true;
}

bool Sender::initialize(){
    server = new socket_connection::SocketServer(&logger);
    server->setSocketListener(this);
    server->start(65111);
    return true;
}

bool Sender::deinitialize(){
    //TODO
    return false;
}

void Sender::receivedMessage(socket_connection::SocketConnector &from, char* buff, int bytesRead){
    char type = buff[0];
    logger.debug("Sender::receivedMessage") << "size: " <<bytesRead << "type: " << (int)type;
    switch ((MessageType)((int)type)) {
    case MessageType::CHANNEL_DATA:
        break;
    case MessageType::MESSAGE:
        break;
    case MessageType::CHANNEL_MAPPING:
        break;
    case MessageType::ERROR:
        break;
    case MessageType::GET_CHANNEL_DATA:
        //TODO send channel data
        break;
    case MessageType::REGISTER_CHANNEL:
        //TODO does that really add an element if it doesn't exists? :D
        std::vector<int> &clientMapping = clientChannels[from.getID()];

        //split the string
        std::vector<std::string> channels = lms::extra::split(&buff[1],bytesRead-1,';');
        if(clientMapping.size() == 0){
            //no channels received!
            break;
        }

        std::string ans(1,(char)MessageType::CHANNEL_MAPPING);
        for(std::string &channel:channels){
            int channelID = addChannel(channel);
            clientMapping.push_back(channelID);
            logger.debug("REGISTER_CHANNEL") <<"NAME-"<< channel<<"---";
            ans += channel+";"+std::to_string(channelID)+";";
        }
        from.sendMessage(ans.c_str(),ans.length(),true);
        /*
        char *resultBuff = &buff[1];
        int oldFound = 0;
        //add message id
        std::string ans(1,(char)MessageType::CHANNEL_MAPPING);
        for(int i = 0; i < bytesRead-1; i++){
            if(resultBuff[i] == ';'){
                //found new part
                std::string name(&resultBuff[oldFound],i-oldFound);
                oldFound = i+1;
                logger.debug("REGISTER_CHANNEL") <<"NAME-"<< name<<"---";
                int channelID = addChannel(name);
                clientMapping.push_back(channelID);
                ans += channelID+";";
            }
        }
        from.sendMessage(ans.c_str(),ans.length(),true);
        */
        break;
    }
}

int Sender::addChannel(std::string name){
    //check if channel is already registered
    for(int i = 0; i < channelMapping.size(); i++){
        if(channelMapping[i].name == name){
            return i;
        }
    }
    ChannelMapping cm;
    cm.name = name;
    cm.iD = channelMapping.size();
    channelMapping.push_back(cm);
    return channelMapping.size() -1;
}

void Sender::disconnected(const socket_connection::SocketConnector &disconnected){
    //TODO
}

void Sender::connected(const socket_connection::SocketConnector &connected){
    //TODO
}
