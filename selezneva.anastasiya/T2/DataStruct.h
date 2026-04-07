#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <vector>
#include "IoTypes.h"
#include "IoFmtGuard.h"

struct DataStruct {
    double key1 = 0.0;
    long long key2 = 0;
    std::string key3 = "";
};

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& src);

bool parseFromString(const std::string& line, DataStruct& dest);

std::string formatDoubleSci(double value);
std::string formatLongLongLit(long long value);

#endif
