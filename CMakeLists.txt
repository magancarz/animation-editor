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

cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
project(animation-editor LANGUAGES C CXX)

add_subdirectory(ThirdParty)

file(GLOB_RECURSE CHAOS_ANIMATIONS_COMMON_SOURCES Source/Common/*.cpp)
add_library(chaos-animations-common STATIC ${CHAOS_ANIMATIONS_COMMON_SOURCES})
target_include_directories(chaos-animations-common PUBLIC Include)
target_link_libraries(chaos-animations-common glm::glm)

file(GLOB_RECURSE CHAOS_ANIMATIONS_ASSETS_SOURCES Source/Assets/*.cpp)
add_library(chaos-animations-assets STATIC ${CHAOS_ANIMATIONS_ASSETS_SOURCES})
target_include_directories(chaos-animations-assets PUBLIC Include)
target_include_directories(chaos-animations-assets PUBLIC ThirdParty/assimp/include)
target_link_libraries(chaos-animations-assets
    chaos-animations-common
    assimp)

file(GLOB_RECURSE CHAOS_IK_SOURCES Source/InverseKinematics/*.cpp)
add_library(chaos-inverse-kinematics STATIC ${CHAOS_IK_SOURCES})
target_include_directories(chaos-inverse-kinematics PUBLIC Include)
target_link_libraries(chaos-inverse-kinematics chaos-animations-common)

file(GLOB_RECURSE CHAOS_ANIMATIONS_SOURCES Source/Animations/*.cpp)
add_library(chaos-animations STATIC ${CHAOS_ANIMATIONS_SOURCES})
target_include_directories(chaos-animations PUBLIC Include)
target_link_libraries(chaos-animations chaos-animations-common)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

add_subdirectory(Tests)
add_subdirectory(Application)