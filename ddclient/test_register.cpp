#include <iostream>
#include <string>
#include "register.hpp"

int main()
{
    for (int i = 0; i < 15; ++i)
    {
        DDClient::register_service("quadcopter1", 1234, {{"type", "quadcopter"}});
        sleep(1);
    }
    return 0;
}

