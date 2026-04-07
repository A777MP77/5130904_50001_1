

#include "Parsers.hpp"
#include <sstream>
#include <cctype>

namespace nspace
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c;
        in >> c;
        if (in && c != dest.exp)
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, SllLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::string token;
        in >> token;
        if (!in)
        {
            return in;
        }

        if (token.size() < 2)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string suffix = token.substr(token.size() - 2);
        if (suffix != "LL" && suffix != "ll")
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string numStr = token.substr(0, token.size() - 2);
        if (numStr.empty())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        for (size_t i = 0; i < numStr.size(); ++i)
        {
            if (i == 0 && numStr[i] == '-') continue;
            if (!std::isdigit(static_cast<unsigned char>(numStr[i])))
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        try
        {
            dest.ref = std::stoll(numStr);
            return in;
        }
        catch (...)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::string label;
        if ((in >> StringIO{ label }) && label != dest.exp)
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        long long numerator = 0;
        unsigned long long denominator = 0;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> LabelIO{ "N" } >> DelimiterIO{ ' ' } >> numerator
            >> DelimiterIO{ ':' } >> LabelIO{ "D" } >> DelimiterIO{ ' ' } >> denominator
            >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

        if (in && denominator != 0)
        {
            dest.ref = { numerator, denominator };
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
}



