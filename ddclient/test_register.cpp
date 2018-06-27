#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <zmq.hpp>
#include "register.hpp"

void subscription(zmq::message_t * update)
{
    zmq::context_t context (1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    const char * filter = "stop ";
    subscriber.connect("epgm://intwifi0;239.192.1.1:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));
    subscriber.recv(update);
}

int main()
{
    int flag = 1;
    zmq::message_t update;
    std::thread (subscription, &update).detach();

    while (flag != 0)
    {
        std::string signal;

        DDClient::register_service("quadcopter2", 1234, {{"type", "quadcopter"}});

	signal = (char *) update.data();

        flag = signal.compare("stop 2");

        sleep(1);
    }
    return 0;
}

