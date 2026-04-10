#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include "data_struct.h"

int main() {
    std::vector<DataStruct> vec;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        DataStruct temp;
        if (iss >> temp) {
            vec.push_back(temp);
        } else {
            std::cerr << "Failed to parse: " << line << std::endl;
        }
    }

    if (vec.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    }

    std::sort(vec.begin(), vec.end(),
        [](const DataStruct& a, const DataStruct& b) {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            double mod_a = std::abs(a.key2);
            double mod_b = std::abs(b.key2);
            if (mod_a != mod_b) return mod_a < mod_b;
            return a.key3.size() < b.key3.size();
        });

    std::copy(vec.begin(), vec.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    std::cout << "Atleast one supported record type\n";

    return 0;
}
