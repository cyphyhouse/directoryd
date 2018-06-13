#include <iostream>
#include <string>
#include "query.hpp"

int main(int argc, char * argv[]) 
{
    if (argc < 2)
    {
        std::cout << "Need a parameter\n";
        return -1;
    }
    auto result = DDClient::find({{"type", argv[1]}}, "new_copter");
    for (auto &r : result)
        std::cout << r.address << ":" << r.port << std::endl;
    return 0;
}
