#ifndef SOCKET_DATA_RECEIVER_H
#define SOCKET_DATA_RECEIVER_H
#include <lms/module.h>
#include <socket_connection/socket_server.h>
class Sender:public lms::Module{
public:

    bool initialize();
    bool deinitialize();

    bool cycle();
private:
    socket_connection::SocketServer *server;
};

#endif /*SOCKET_DATA_RECEIVER_H*/
