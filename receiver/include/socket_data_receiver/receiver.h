#ifndef SOCKET_DATA_RECEIVER_H
#define SOCKET_DATA_RECEIVER_H
#include <lms/module.h>
class Receiver:public lms::Module{
public:

    bool initialize();
    bool deinitialize();

    bool cycle();
};

#endif /*SOCKET_DATA_RECEIVER_H*/
