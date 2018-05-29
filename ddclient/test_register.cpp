#include <iostream>
#include <string>
#include "register.hpp"

int main()
{
    for (int i = 0; i < 15; ++i)
    {
	char name[] = "new_copter";
	DDClient::register_service(name, 1234, {});
        sleep(1);
    }
    return 0;
}

