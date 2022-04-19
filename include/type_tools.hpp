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

#ifndef TYPE_TOOLS_HPP
#define TYPE_TOOLS_HPP

#include <type_traits>

/**
 * @brief used to check if type is valid. This
 * is mostly similar to the std::experimental::is_detected but
 * works without experimental type traits.
 *
 * If the template-id Op<Args...> denotes a valid type, then value_t is an alias for std::true_type, and type is an alias for Op<Args...>;
 * Otherwise, value_t is an alias for std::false_type and type is an alias for Default.
 *
 */
template <class Default, class AlwaysVoid,
          template <class...> class Op, class... Args>
struct detector
{
    using value_t = std::false_type;
    using type = Default;
};
template <class Default, template <class...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
    using type = Op<Args...>;
};

/**
 * @brief determines if Op<Args...> is valid. Equal to std::true_type on
 * success and std::false_type on failure.
 *
 * @tparam Op template type to validate
 * @tparam Args arguments used by template type
 */
template <template <class...> class Op, class... Args>
using is_detected = typename detector<nullptr_t, void, Op, Args...>::value_t;
/**
 * @brief type of Op<Args...> if valid, otherwise nullptr_t
 *
 * @tparam Op template type to validate
 * @tparam Args arguments used by template type
 */
template <template <class...> class Op, class... Args>
using is_detected_t = typename detector<nullptr_t, void, Op, Args...>::type;
template <template <class...> class Op, class... Args>
inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;
/**
 * @brief type of Op<Args...> if valid, otherwise Default
 *
 * @tparam Default default type if Op<Args...> is not valid
 * @tparam Op template type to validate
 * @tparam Args arguments used by template type
 */
template <class Default, template <class...> class Op, class... Args>
using detected_or = typename detector<Default, void, Op, Args...>::value_t;
template <class Default, template <class...> class Op, class... Args>
using detected_or_t = typename detector<Default, void, Op, Args...>::type;

/**
 * @brief iterator of an iterable type.
 *
 * @tparam Iter, iterable type
 */
template <typename Iter>
using is_iterable_t = typename Iter::iterator;
/**
 * @brief determines if a type is iterable. Equal to std::true_type on
 * success and std::false_type on failure.
 *
 * @tparam Iter, potential iterable type
 */
template <typename Iter>
using is_iterable = is_detected<is_iterable_t, Iter>;
/**
 * @brief equal to true if type is an iterable
 *
 * @tparam Iter, potential iterable type
 */
template <typename Iter>
inline constexpr bool is_iterable_v = is_iterable<Iter>::value;

/**
 * @brief This macro generates member function validators,
 * useful if you want to check if a given type has a member function
 *
 * @arg func the name of the member function to validate
 * @arg name what the generated templates are called
 *
 * Ex: GENERATE_HAS_MEMEBER_FUNCTION(insert, insert_check)
 *  insert_check_t: the return type of member function Class.insert(Args...)
 *  has_insert_check: std::true_type if Class.insert(Args...) is valid
 *  has_insert_check_ret: std::true_type if Class.insert(Args...) is type Return
 *  has_insert_check_v: true if Class.insert(Args...) is valid
 *  has_insert_check_ret_v: true if Class.insert(Args...) is type Return
 */
#define GENERATE_HAS_MEMBER_FUNCTION(func, name)                                                        \
    template <typename Class, typename... Args>                                                         \
    using name##_t = decltype(std::declval<Class>().func(std::declval<Args>()...));                     \
    template <typename Class, typename... Args>                                                         \
    using has_##name = is_detected<name##_t, Class, Args...>;                                           \
    template <typename Class, typename Return, typename... Args>                                        \
    using has_##name##_ret = typename std::is_same<Return, is_detected_t<name##_t, Class, Args...>>::type; \
                                                                                                        \
    template <typename Class, typename... Args>                                                         \
    inline constexpr bool has_##name##_v = has_##name<Class, Args...>::value;                           \
    template <typename Class, typename Return, typename... Args>                                        \
    inline constexpr bool has_##name##_ret_v = has_##name##_ret<Class, Return, Args...>::value;

#endif // MORE_MAPS_TYPE_CHECKS_HPP