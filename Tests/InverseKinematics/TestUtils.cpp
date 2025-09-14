// MIT License

// Copyright (c) 2025 Mateusz Gancarz

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "TestUtils.h"

bool TestUtils::equal(const chs::ik::Chain& first, const chs::ik::Chain& second, float precision)
{
    if (first.size() != second.size())
    {
        return false;
    }

    for (int index = 0; index < first.size(); ++index)
    {
        if (!equal(first.at(index), second.at(index), precision))
        {
            return false;
        }
    }

    return true;
}

bool TestUtils::equal(const chs::ik::Segment& first, const chs::ik::Segment& second, float precision)
{
    return equal(first.worldOrigin(), second.worldOrigin(), precision) &&
        equal(first.worldEnd(), second.worldEnd(), precision) &&
        equal(first.length(), second.length(), precision);
}

bool TestUtils::equal(const glm::vec3& first, const glm::vec3& second, float precision)
{
    return equal(first.x, second.x, precision) &&
        equal(first.y, second.y, precision) &&
        equal(first.z, second.z, precision);
}

bool TestUtils::equal(float first, float second, float precision)
{
    return second <= first + precision && second >= first - precision;
}
