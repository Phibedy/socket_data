#include <socket_data_sender/sender.h>

bool Sender::cycle(){
    server->cycle();
    return true;
}

bool Sender::initialize(){
    server = new socket_connection::SocketServer(&logger);
    server->start(65111);
    return true;
}

bool Sender::deinitialize(){
    //TODO
    return false;
}
