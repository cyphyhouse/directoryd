#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "query.hpp"

int main()
{
    std::ifstream vehicles("vehicles.txt");
    std::ofstream addresses("addresses.txt");
    if (vehicles.fail())
    {
        std::cout << "vehicles.txt not found\n";
        return -1;
    }
    while (!vehicles.eof())
    {
        std::string vehicle_type;
        int vehicle_number;
        vehicles >> vehicle_type >> vehicle_number;
        while (vehicle_number)
        {
            auto result = DDClient::find({}, vehicle_type + std::to_string(vehicle_number));
            if (!result.empty())
            {
                addresses << result.back().address << std::endl;
                --vehicle_number;
            }
            sleep(1);
        }
    }
    vehicles.close();
    addresses.close();
    return 0;
}
