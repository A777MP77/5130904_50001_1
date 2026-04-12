#ifndef IOTYPES_H
#define IOTYPES_H

#include <iostream>
#include <string>

struct DelimiterIO {
    char exp;
};

struct DoubleSciIO {
    double& ref;
};

struct SllLitIO {
    long long& ref;
};

struct StringIO {
    std::string& ref;
};

struct LabelIO {
    std::string exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleSciIO&& dest);
std::istream& operator>>(std::istream& in, SllLitIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);

#endif
