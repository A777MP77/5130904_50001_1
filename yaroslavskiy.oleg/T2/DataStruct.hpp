
#pragma once

#include <iostream>
#include <string>
#include <utility>

namespace nspace
{
    struct DataStruct
    {
        long long key1;
        std::pair<long long, unsigned long long> key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
}


