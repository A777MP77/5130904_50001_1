
#ifndef PARSERS_HPP
#define PARSERS_HPP

#include <iostream>
#include <string>
#include <utility>

namespace nspace
{
    struct DelimiterIO
    {
        char exp;
    };

    struct SllLitIO
    {
        long long& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };

    struct RationalIO
    {
        std::pair<long long, unsigned long long>& ref;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, SllLitIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, RationalIO&& dest);
}

#endif


