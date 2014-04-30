#include <iostream>
#include <string>
#include "register.hpp"

int main() {
    try {
	char name[] = "hello 0";
	DDClient::register_service("lol", 1234, {});
	for (int i = 0; i < 10; ++i) {
	    sleep(1);
	    std::cout << "tick" << std::endl;
	    DDClient::register_service(name, 5678+i, {});
	    name[6]++;
	}
    }
    catch (std::string &s) {
	std::cout << s << std::endl;
    }
    sleep(1);
    return 0;
}

