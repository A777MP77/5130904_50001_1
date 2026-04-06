#ifndef IOTYPES_H
#define IOTYPES_H

#include <iostream>
#include "DataStruct.h"

struct DelimiterIO {
    char exp = 0;
};

struct DoubleSciIO {
    double& ref;
};

struct LongLongLitIO {
    long long& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleSciIO&& dest);
std::istream& operator>>(std::istream& in, LongLongLitIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);

std::ostream& operator<<(std::ostream& out, const DataStruct& src);

#endif
