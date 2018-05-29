#include <iostream>
#include <string>
#include "query.hpp"

int main() {
    try {
	auto result = DDClient::find({}, "new_copter");
	for (auto &r : result) {
	    std::cout << r.address << ":" << r.port << std::endl;
	}
    }
    catch (std::string &s) {
	std::cout << s << std::endl;
    }
    return 0;
}
