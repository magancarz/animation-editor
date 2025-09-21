# MIT License

# Copyright (c) 2025 Mateusz Gancarz

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

include(CTest)
include(GoogleTest)

enable_testing()
if(BUILD_TESTING)
    file(GLOB_RECURSE CHAOS_IK_TESTS_SOURCES InverseKinematics/*.cpp TestUtils.cpp)
	add_executable(chaos-inverse-kinematics-tests InverseKinematics/main.cpp ${CHAOS_IK_TESTS_SOURCES})
	target_link_libraries(chaos-inverse-kinematics-tests
		chaos-inverse-kinematics
		GTest::gtest_main
		GTest::gmock)
	target_include_directories(chaos-inverse-kinematics-tests PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/InverseKinematics)
	add_test(NAME chaos-inverse-kinematics-tests-all COMMAND chaos-inverse-kinematics-tests)

    file(GLOB_RECURSE CHAOS_ANIMATIONS_COMMON_TESTS_SOURCES Common/*.cpp TestUtils.cpp)
	add_executable(chaos-animations-common-tests Common/main.cpp ${CHAOS_ANIMATIONS_COMMON_TESTS_SOURCES})
	target_link_libraries(chaos-animations-common-tests
		chaos-animations-common
		GTest::gtest_main
		GTest::gmock)
	target_include_directories(chaos-animations-common-tests PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/Include/)
	add_test(NAME chaos-animations-common-tests-all COMMAND chaos-animations-common-tests)

    file(GLOB_RECURSE CHAOS_ANIMATION_TESTS_SOURCES Animations/*.cpp TestUtils.cpp)
	add_executable(chaos-animations-tests Animations/main.cpp ${CHAOS_ANIMATION_TESTS_SOURCES})
	target_link_libraries(chaos-animations-tests
		chaos-animations
		GTest::gtest_main
		GTest::gmock)
	target_include_directories(chaos-animations-tests PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/Animations/
		${CMAKE_SOURCE_DIR}/Include/)
	add_test(NAME chaos-animations-tests-all COMMAND chaos-animations-tests)
endif()