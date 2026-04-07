
#include "DataStruct.hpp"
#include "parsers.hpp"
#include "format.hpp"
#include <istream>
#include <ostream>
#include <string>
#include <utility>

namespace
{
    class StreamFormatGuard
    {
    public:
        explicit StreamFormatGuard(std::basic_ios< char >& stream) :
            stream_(stream),
            precision_(stream.precision()),
            fill_(stream.fill()),
            flags_(stream.flags())
        {
        }

        ~StreamFormatGuard()
        {
            stream_.precision(precision_);
            stream_.fill(fill_);
            stream_.flags(flags_);
        }

    private:
        std::basic_ios< char >& stream_;
        std::streamsize precision_;
        char fill_;
        std::basic_ios< char >::fmtflags flags_;
    };
}

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
{
    if (lhs.key1 != rhs.key1)
    {
        return lhs.key1 < rhs.key1;
    }

    double ra = static_cast<double>(lhs.key2.first) / lhs.key2.second;
    double rb = static_cast<double>(rhs.key2.first) / rhs.key2.second;

    if (ra != rb)
    {
        return ra < rb;
    }

    return lhs.key3.size() < rhs.key3.size();
}

std::istream& operator>>(std::istream& in, DataStruct& value)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    std::string record;
    DataStruct parsedValue{};

    while (readRecord(in, record))
    {
        if (parseRecord(record, parsedValue))
        {
            value = std::move(parsedValue);
            return in;
        }
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& value)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    StreamFormatGuard guard(out);

    out << "(:key1 " << formatLongLongLit(value.key1)
        << ":key2 " << formatRational(value.key2)
        << ":key3 \"" << value.key3 << "\":)";

    return out;

}


