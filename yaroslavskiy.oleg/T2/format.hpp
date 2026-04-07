
#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <string>
#include <utility>

std::string formatLongLongLit(long long value);
std::string formatRational(const std::pair<long long, unsigned long long>& value);

#endif
