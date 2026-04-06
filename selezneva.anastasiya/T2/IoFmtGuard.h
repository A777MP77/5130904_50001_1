#ifndef IOFMTGUARD_H
#define IOFMTGUARD_H

#include <iostream>

class IoFmtGuard
{
public:
    explicit IoFmtGuard(std::basic_ios<char>& s);
    ~IoFmtGuard();

    IoFmtGuard(const IoFmtGuard&) = delete;
    IoFmtGuard& operator=(const IoFmtGuard&) = delete;

private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

#endif
