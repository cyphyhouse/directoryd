#include <iostream>
#include <sstream>
#include <string>
#include <zmq.hpp>
#include "register.hpp"

int main()
{
    zmq::context_t context (1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");

    const char * filter = "stop ";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));

    int flag = 1;

    do {
        zmq::message_t update;
        std::string signal;

        DDClient::register_service("quadcopter1", 1234, {{"type", "quadcopter"}});

        subscriber.recv(&update);

        std::istringstream iss(static_cast<char *>(update.data()));
        iss >> signal;

        flag = signal.compare("stop 1");

        sleep(1);
    } while (flag != 0);
    return 0;
}

