
#include "DataStruct.hpp"
#include <string>
#include <sstream>

namespace nspace
{
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::string line;
        if (!std::getline(in, line))
        {
            return in;
        }

        size_t pos1 = line.find(":key1 ");
        if (pos1 == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        pos1 += 6;

        size_t pos2 = line.find("LL", pos1);
        if (pos2 == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string key1_str = line.substr(pos1, pos2 - pos1);
        dest.key1 = std::stoll(key1_str);

        size_t posN = line.find(":N ");
        if (posN == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        posN += 3;

        size_t posD = line.find(":D ", posN);
        if (posD == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string num_str = line.substr(posN, posD - posN);
        long long num = std::stoll(num_str);

        size_t posClose = line.find(":)", posD + 3);
        if (posClose == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string den_str = line.substr(posD + 3, posClose - (posD + 3));
        unsigned long long den = std::stoull(den_str);

        dest.key2 = { num, den };

        size_t posKey3 = line.find(":key3 \"");
        if (posKey3 == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        posKey3 += 7;

        size_t posQuote = line.find("\"", posKey3);
        if (posQuote == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.key3 = line.substr(posKey3, posQuote - posKey3);

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        out << "(:key1 " << src.key1 << "LL"
            << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }
}


