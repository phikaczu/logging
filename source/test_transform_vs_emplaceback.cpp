#include <gtest\gtest.h>

#include <algorithm>
#include "Helpers.h"
#include <memory>

namespace
{
    const std::vector<int> inputValues = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int*> getInputPtrs()
    {
        std::vector<int*> inputPtrs;

        for(int idx = 0; idx < 10; ++idx)
        {
            inputPtrs.push_back(new int(idx));
        }

        return inputPtrs;
    }
}

TEST(performance, emplacebackValue)
{
    std::vector<int> output;
    output.reserve(inputValues.size());

    {
        Profiler profile("emplacebackValue");

        for (auto value : inputValues)
        {
            output.emplace_back(value + 10);
        }
    }
}

TEST(performance, transformValue)
{
    std::vector<int> output;
    output.reserve(inputValues.size());

    {
        Profiler profile("transformValue");
        std::transform(inputValues.cbegin(), inputValues.cend(), std::back_inserter(output), [](const int v) { return v + 10; });
    }
}

TEST(performance, emplacebackPtr)
{
    const std::vector<int*> inputPtrs = getInputPtrs();
    std::vector<std::unique_ptr<int>> output;
    output.reserve(inputPtrs.size());

    {
        Profiler profile("emplacebackPtr");

        for (auto value : inputPtrs)
        {
            output.emplace_back(value);
        }
    }
}

TEST(performance, transformPtr)
{
    const std::vector<int*> inputPtrs = getInputPtrs();
    std::vector<std::unique_ptr<int>> output;
    output.reserve(inputPtrs.size());

    {
        Profiler profile("transformPtr");
        std::transform(inputPtrs.cbegin(), inputPtrs.cend(), std::back_inserter(output), [](int* v) { return std::unique_ptr<int>(v); });
    }
}

namespace std
{
    bool operator==(const std::vector<int*>& lhs, const std::vector<std::unique_ptr<int>>& rhs)
    {
        return (lhs.size() == rhs.size())
            ? std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), [](const int* lv, const std::unique_ptr<int>& rv) { return lv == rv.get(); })
            : false;
    }
}

TEST(algorithm, equals)
{
    const std::vector<int*> inputPtrs = getInputPtrs();
    std::vector<std::unique_ptr<int>> output;
    output.reserve(inputPtrs.size());
    std::transform(inputPtrs.cbegin(), inputPtrs.cend(), std::back_inserter(output), [](int* v) { return std::unique_ptr<int>(v); });

    // The ASSERT_TRUE will start in current namespace and therefore it does not need the operator== overload to be present in the
    // std namespace
    //ASSERT_TRUE(inputPtrs == output);
    ASSERT_EQ(inputPtrs, output);
}