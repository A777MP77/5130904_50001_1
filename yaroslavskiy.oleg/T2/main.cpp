
// T2 Yaroslavsky Oleg 5130904/50001 Variant 11

#include "DataStruct.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>

bool compare(const nspace::DataStruct& a, const nspace::DataStruct& b);

int main()
{
    std::vector<nspace::DataStruct> data;

    std::istream_iterator<nspace::DataStruct> it(std::cin);
    std::istream_iterator<nspace::DataStruct> end;

    while (it != end)
    {
        data.push_back(*it);
        ++it;
    }

    if (data.empty())
    {
        return EXIT_FAILURE;
    }

    std::sort(data.begin(), data.end(), compare);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}

bool compare(const nspace::DataStruct& a, const nspace::DataStruct& b)
{
    if (a.key1 != b.key1) return a.key1 < b.key1;

    double va = static_cast<double>(a.key2.first) / a.key2.second;
    double vb = static_cast<double>(b.key2.first) / b.key2.second;

    if (std::fabs(va - vb) > 1e-12) return va < vb;

    return a.key3.size() < b.key3.size();
}




