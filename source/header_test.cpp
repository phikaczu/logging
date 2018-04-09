#include <gtest\gtest.h>

#include <iostream>

#include "Header.h"
#include "Header2.h"
#include "Helpers.h"

TEST(headertest, basic)
{
    Profiler profile("Header");

    for (int i = 0; i < 100000; ++i)
    {
        const auto header = Header::GetHeader("Hello ", 1, " ", 3.14);
        std::cout << header << std::endl;
    }
}

TEST(header2test, basic)
{
    Profiler profile("Header2");

    for (int i = 0; i < 100000; ++i)
    {
        const auto header2 = Header2() << "Hello " << 1 << " " << 3.14;
        std::cout << header2 << std::endl;
    }
}

TEST(headerlambda, basic)
{
    const int value = 1;
    const double dValue = 3.14;

    {
        Profiler profile("HeaderLambda");

        for (int i = 0; i < 100000; ++i)
        {
            auto logReadDataRequest = [&](const std::string& prefix)
            {
                std::cout << prefix << value << " " << dValue << std::endl;
            };

            logReadDataRequest("Hello ");
        }
    }
}