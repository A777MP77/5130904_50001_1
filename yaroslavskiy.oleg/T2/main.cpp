
// T2 Yaroslavsky Oleg 5130904/50001 Variant 11

#include "DataStruct.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
    std::vector<nspace::DataStruct> data;

    std::copy(
        std::istream_iterator<nspace::DataStruct>(std::cin),
        std::istream_iterator<nspace::DataStruct>(),
        std::back_inserter(data)
    );

    if (data.empty())
    {
        std::cout << "Atleast one supported record type" << std::endl;
        return EXIT_SUCCESS;
    }

    std::sort(data.begin(), data.end(), nspace::compare);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}



