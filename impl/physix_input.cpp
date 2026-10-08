#include "physix_input.h"
#include <iostream>
#include <limits>
#include <stdexcept>

namespace Physix::Input {
    double getDouble(const std::string& prompt) 
    {
        double value;

        while (true)
        {
            std::cout << prompt << ": ";

            if (std::cin >> value)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            else
            {
                if (std::cin.eof() || std::cin.bad())
                {
                    throw std::runtime_error("Input ended before a valid number was entered");
                }
                std::cout << "Invalid input, please try again\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        }
    }

}
