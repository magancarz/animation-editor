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

#include <gtest/gtest.h>
#include <glm/gtx/quaternion.hpp>

#include "Common/ConstChainIterator.h"

glm::mat4 TestUtils::fromVector(const glm::vec3& vector)
{
    glm::mat4 rotation = glm::toMat4(glm::quat{glm::vec3{0, 1, 0}, glm::normalize(vector)});
    glm::mat4 translation = glm::translate(glm::mat4{1.0f}, vector);
    return translation * rotation;
}

void TestUtils::expectEqual(const chs::common::Chain& first, const chs::common::Chain& second, float precision)
{
    chs::common::ConstChainIterator first_chain_iterator{first};
    chs::common::ConstChainIterator second_chain_iterator{second};

    while (first_chain_iterator.hasNext() && second_chain_iterator.hasNext())
    {
        expectEqual(first_chain_iterator.next(), second_chain_iterator.next(), precision);
    }

    EXPECT_TRUE(!first_chain_iterator.hasNext() && !second_chain_iterator.hasNext());
}

void TestUtils::expectEqual(const chs::common::Segment& first, const chs::common::Segment& second, float precision)
{
    expectEqual(first.worldOrigin(), second.worldOrigin(), precision);
    expectEqual(first.worldEnd(), second.worldEnd(), precision);
    expectEqual(first.length(), second.length(), precision);
}

void TestUtils::expectEqual(const glm::vec3& first, const glm::vec3& second, float precision)
{
    expectEqual(first.x, second.x, precision);
    expectEqual(first.y, second.y, precision);
    expectEqual(first.z, second.z, precision);
}

void TestUtils::expectEqual(float first, float second, float precision)
{
    EXPECT_TRUE(first - precision <= second && second <= first + precision);
}
