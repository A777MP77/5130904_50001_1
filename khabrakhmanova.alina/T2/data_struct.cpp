#include "data_struct.hpp"
#include "parser.hpp"
#include <iomanip>
#include <cmath>

bool compareData(const DataStruct& left, const DataStruct& right)
{
    if (left.key1 != right.key1)
    {
        return left.key1 < right.key1;
    }

    double leftMod = std::abs(left.key2);
    double rightMod = std::abs(right.key2);
    if (leftMod != rightMod)
    {
        return leftMod < rightMod;
    }

    return left.key3.length() < right.key3.length();
}

std::istream& operator>>(std::istream& stream, DataStruct& data)
{
    std::string rawLine;
    while (std::getline(stream, rawLine))
    {
        std::size_t startTrim = rawLine.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == startTrim)
        {
            continue;
        }
        std::size_t endTrim = rawLine.find_last_not_of(" \t\n\r\f\v");
        std::string cleanLine = rawLine.substr(startTrim, endTrim - startTrim + 1);

        if (cleanLine.empty())
        {
            continue;
        }

        bool parseSuccess = parseRecordString(cleanLine, data);
        if (parseSuccess)
        {
            return stream;
        }
    }
    stream.setstate(std::ios::failbit);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const DataStruct& data)
{
    stream << "(:key1 ";
    stream << data.key1 << "ull";

    stream << ":key2 ";
    stream << "#c(" << std::fixed << std::setprecision(1)
           << data.key2.real() << " " << data.key2.imag() << ")";

    stream << ":key3 \"" << data.key3 << "\":)";
    return stream;
}
