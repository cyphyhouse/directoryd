#include <iostream>
#include <string>
#include "query.hpp"

int main() {
    try {
	auto result = DDClient::find({{"type", "vision"}, {"format", "protobuf"}}, "ruby-vision");
	for (auto &r : result) {
	    std::cout << r.hostname << ":" << r.port << std::endl;
	}
    }
    catch (std::string &s) {
	std::cout << s << std::endl;
    }
    return 0;
}
