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

#include <iostream>
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

void is_detected_examples()
{
    if (is_detected<copy_assign_t, CanCopy>::value)
        std::cout << "CanCopy is copy assignable" << std::endl;
    else
        std::cout << "CanCopy is not copy assignable" << std::endl;

    if (is_detected<copy_assign_t, CannotCopy>::value)
        std::cout << "CannotCopy is copy assignable" << std::endl;
    else
        std::cout << "CannotCopy is not copy assignable" << std::endl;
    std::cout << std::endl;
}

void is_iterable_examples()
{
    if (is_iterable_v<std::vector<int>>)
        std::cout << "std::vector<int> is iterable" << std::endl;
    else
        std::cout << "std::vector<int> is not iterable" << std::endl;

    if (is_iterable_v<float>)
        std::cout << "float is iterable" << std::endl;
    else
        std::cout << "float is not iterable" << std::endl;
    std::cout << std::endl;
}

GENERATE_HAS_MEMBER_FUNCTION(insert, insert)
GENERATE_HAS_MEMBER_FUNCTION(operator[], operator_index)

void has_member_function_examples()
{
    typedef std::map<int, float> if_map;
    if (std::is_same_v<std::pair<if_map::iterator, bool>, insert_t<if_map, if_map::value_type>>)
    {
        std::cout << "std::map<int, float>::insert(int) has return type std::pair<std::map<int, float>::iterator, bool>" << std::endl;
    }
    std::cout << std::endl;

    if (has_operator_index_v<std::map<int, float>, int>)
    {
        std::cout << "std::map<int,float> has member function operator[](int)" << std::endl;
    }
    if (!has_operator_index_v<std::map<int, float>, char*>)
    {
        std::cout << "std::map<int,float> does not have member function operator[](char*)" << std::endl;
    }
    std::cout << std::endl;

    if (has_operator_index_ret_v<std::map<int, float>, float&, int>)
    {
        std::cout << "std::map<int,float> member function operator[](int) with return type float&" << std::endl;
    }
    if (!has_operator_index_ret_v<std::map<int, float>, float, int>)
    {
        std::cout << "std::map<int,float> does not have member function operator[](int) with return type float" << std::endl;
    }
}

int main()
{
    is_detected_examples();
    is_iterable_examples();
    has_member_function_examples();
}