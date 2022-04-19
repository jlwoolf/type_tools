// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 - 2022 Jonathan Woolf <jlwoolf@protonmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <map>
#include <vector>

#include <type_tools.hpp>

template <class T>
using copy_assign_t = decltype(std::declval<T &>() = std::declval<const T &>());

struct CanCopy
{
};
struct CannotCopy
{
    void operator=(const CannotCopy &) = delete;
};

TEST_CASE("is_detected") {
    SECTION("is_detected") {
        REQUIRE(std::is_same_v<is_detected<copy_assign_t, CanCopy>, std::true_type>);
        REQUIRE(std::is_same_v<is_detected<copy_assign_t, CannotCopy>, std::false_type>);
    }
    SECTION("is_detected_v") {
        REQUIRE(is_detected_v<copy_assign_t, CanCopy>);
        REQUIRE(!is_detected_v<copy_assign_t, CannotCopy>);
    }

    SECTION("detected_t")
    {
        REQUIRE(std::is_same_v<detected_t<copy_assign_t, CanCopy>, copy_assign_t<CanCopy>>);
        REQUIRE(std::is_same_v<detected_t<copy_assign_t, CannotCopy>, nullptr_t>);
    }

    SECTION("detected_or")
    {
        REQUIRE(std::is_same_v<detected_or<int, copy_assign_t, CanCopy>, std::true_type>);
        REQUIRE(std::is_same_v<detected_or<int, copy_assign_t, CannotCopy>, std::false_type>);
        
    }

    SECTION("detected_or_t") {
        REQUIRE(std::is_same_v<detected_or_t<int, copy_assign_t, CanCopy>, copy_assign_t<CanCopy>>);
        REQUIRE(std::is_same_v<detected_or_t<int, copy_assign_t, CannotCopy>, int>);
    }
}

TEST_CASE("is_iterable") {
    SECTION("is_iterable_t") {
        REQUIRE(std::is_same_v<is_iterable_t<std::vector<int>>, std::vector<int>::iterator>);
    }

    SECTION("is_iterable") {
        REQUIRE(std::is_same_v<is_iterable<std::vector<int>>, std::true_type>);
        REQUIRE(std::is_same_v<is_iterable<std::map<int, float>>, std::true_type>);
        REQUIRE(std::is_same_v<is_iterable<float>, std::false_type>);
    }

    SECTION("is_iterable_v") {
        REQUIRE(is_iterable_v<std::vector<int>>);
        REQUIRE(is_iterable_v<std::map<int, float>>);
        REQUIRE(!is_iterable_v<float>);
    }
}

GENERATE_HAS_MEMBER_FUNCTION(insert, insert);
TEST_CASE("member_insert") {
    SECTION("insert_t") {
        REQUIRE(std::is_same_v<insert_t<std::vector<int>, std::vector<int>::iterator, int>, std::vector<int>::iterator>);
        REQUIRE(std::is_same_v<insert_t<std::vector<int>, std::vector<int>::iterator, const int&>, std::vector<int>::iterator>);
        REQUIRE(std::is_same_v<insert_t<std::vector<int>, std::vector<int>::const_iterator, int>, std::vector<int>::iterator>);
        REQUIRE(std::is_same_v<insert_t<std::vector<int>, std::vector<int>::const_iterator, const int&>, std::vector<int>::iterator>);

        REQUIRE(std::is_same_v<insert_t<std::map<int, float>, std::pair<int, float>>, std::pair<std::map<int, float>::iterator, bool>>);
        REQUIRE(std::is_same_v<insert_t<std::map<int, float>, const std::pair<int, float>&>, std::pair<std::map<int, float>::iterator, bool>>);
    }

    SECTION("has_insert") {
        REQUIRE(std::is_same_v<has_insert<std::vector<int>, std::vector<int>::iterator, int>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert<std::vector<int>, std::vector<int>::iterator, const int &>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert<std::vector<int>, std::vector<int>::const_iterator, int>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert<std::vector<int>, std::vector<int>::const_iterator, const int &>, std::true_type>);

        REQUIRE(std::is_same_v<has_insert<std::vector<int>, std::vector<int>::iterator>, std::false_type>);
        REQUIRE(std::is_same_v<has_insert<std::vector<int>, std::vector<int>::iterator, const float*>, std::false_type>);

        REQUIRE(std::is_same_v<has_insert<std::map<int, float>, std::pair<int, float>>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert<std::map<int, float>, const std::pair<int, float> &>, std::true_type>);

        REQUIRE(std::is_same_v<has_insert<std::map<int, float>, std::pair<char*, float>>, std::false_type>);
        REQUIRE(std::is_same_v<has_insert<std::map<int, float>, const std::pair<int, float> &, int>, std::false_type>);
    }

    SECTION("has_insert_ret") {
        REQUIRE(std::is_same_v<has_insert_ret<std::vector<int>, std::vector<int>::iterator, std::vector<int>::iterator, int>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert_ret<std::vector<int>, std::vector<int>::iterator, std::vector<int>::iterator, const int &>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert_ret<std::vector<int>, std::vector<int>::iterator, std::vector<int>::const_iterator, int>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert_ret<std::vector<int>, std::vector<int>::iterator, std::vector<int>::const_iterator, const int &>, std::true_type>);

        REQUIRE(std::is_same_v<has_insert_ret<std::vector<int>, bool, std::vector<int>::iterator, int>, std::false_type>);
        REQUIRE(std::is_same_v<has_insert_ret<std::vector<int>, std::vector<int>, std::vector<int>::iterator, const int&>, std::false_type>);

        REQUIRE(std::is_same_v<has_insert_ret<std::map<int, float>, std::pair<std::map<int, float>::iterator, bool>, std::pair<int, float>>, std::true_type>);
        REQUIRE(std::is_same_v<has_insert_ret<std::map<int, float>, std::pair<std::map<int, float>::iterator, bool>, const std::pair<int, float> &>, std::true_type>);

        REQUIRE(std::is_same_v<has_insert_ret<std::map<int, float>, bool, std::pair<int, float>>, std::false_type>);
        REQUIRE(std::is_same_v<has_insert_ret<std::map<int, float>, std::pair<std::map<int, float>::iterator, char *>, const std::pair<int, float> &>, std::false_type>);
    }

    SECTION("has_insert_v")
    {
        REQUIRE(has_insert_v<std::vector<int>, std::vector<int>::iterator, int>);
        REQUIRE(has_insert_v<std::vector<int>, std::vector<int>::iterator, const int &>);
        REQUIRE(has_insert_v<std::vector<int>, std::vector<int>::const_iterator, int>);
        REQUIRE(has_insert_v<std::vector<int>, std::vector<int>::const_iterator, const int &>);

        REQUIRE(!has_insert_v<std::vector<int>, std::vector<int>::iterator>);
        REQUIRE(!has_insert_v<std::vector<int>, std::vector<int>::iterator, const float*>);

        REQUIRE(has_insert_v<std::map<int, float>, std::pair<int, float>>);
        REQUIRE(has_insert_v<std::map<int, float>, const std::pair<int, float> &>);

        REQUIRE(!has_insert_v<std::map<int, float>, std::pair<char *, float>>);
        REQUIRE(!has_insert_v<std::map<int, float>, const std::pair<int, float> &, int>);
    }

    SECTION("has_insert_ret_v") {
        REQUIRE(has_insert_ret_v<std::vector<int>, std::vector<int>::iterator, std::vector<int>::iterator, int>);
        REQUIRE(has_insert_ret_v<std::vector<int>, std::vector<int>::iterator, std::vector<int>::iterator, const int &>);
        REQUIRE(has_insert_ret_v<std::vector<int>, std::vector<int>::iterator, std::vector<int>::const_iterator, int>);
        REQUIRE(has_insert_ret_v<std::vector<int>, std::vector<int>::iterator, std::vector<int>::const_iterator, const int &>);

        REQUIRE(!has_insert_ret_v<std::vector<int>, bool, std::vector<int>::iterator, int>);
        REQUIRE(!has_insert_ret_v<std::vector<int>, std::vector<int>, std::vector<int>::iterator, const int &>);

        REQUIRE(has_insert_ret_v<std::map<int, float>, std::pair<std::map<int, float>::iterator, bool>, std::pair<int, float>>);
        REQUIRE(has_insert_ret_v<std::map<int, float>, std::pair<std::map<int, float>::iterator, bool>, const std::pair<int, float> &>);

        REQUIRE(!has_insert_ret_v<std::map<int, float>, bool, std::pair<int, float>>);
        REQUIRE(!has_insert_ret_v<std::map<int, float>, std::pair<std::map<int, float>::iterator, char *>, const std::pair<int, float> &>);
    }
}