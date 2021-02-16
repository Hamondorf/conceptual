#pragma once

#include "conceptual/aliases/numeric_limits.hpp"
#include "conceptual/traits/relationships.hpp"
#include "conceptual/traits/supported_ops.hpp"


namespace ham::cpt
{
namespace detail
{

template <class T, class... ArithmeticTypes>
concept req_arithmetic_interoperability_impl =
    (( 
       req_static_castable_to<ArithmeticTypes       , T>
    && req_static_castable_to<ArithmeticTypes const , T>
    && req_static_castable_to<ArithmeticTypes      &, T>
    && req_static_castable_to<ArithmeticTypes const&, T>
    ) && ...);

template <class T>
concept req_arithmetic_interoperability = 
    req_arithmetic_interoperability_impl<T, 
        bool, 

          signed char,   signed short,   signed int,   signed long,   signed long long,
        unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long,

        float, double, long double
    >;

template <class T>
inline constexpr T zero_v = T(0);

template <class T>
inline constexpr T one_v = T(1);


template <class T>
concept req_common_arithmetic_operators =
    requires (T& lval, T const& x, T const& y)
    {
        {x + y} -> req_convertible_to<T>;
        {x - y} -> req_convertible_to<T>;
        {x * y} -> req_convertible_to<T>;
        {x / y} -> req_convertible_to<T>;

        {lval += x} -> req_same_as<T&>;
        {lval -= x} -> req_same_as<T&>;
        {lval *= x} -> req_same_as<T&>;
        {lval /= x} -> req_same_as<T&>;

        {lval++} -> req_same_as<T>;
        {lval--} -> req_same_as<T>;
        {++lval} -> req_same_as<T&>;
        {--lval} -> req_same_as<T&>;
    };




}

}