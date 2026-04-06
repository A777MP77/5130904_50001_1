#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.h"
#include "IoTypes.h"
#include "DataSort.h"

int main() {
    std::vector<DataStruct> data;

    std::cout << "Input data:" << "\n";
    std::cout << "Example: (:key1 5.45e-2:key2 123ll:key3 \"hello\":)" << "\n\n";

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::cout << "\nSorted data:" << "\n";

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}
