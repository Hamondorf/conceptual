#pragma once

#include <concepts>
#include "conceptual/detail/comparison.hpp"
#include "conceptual/detail/same_as.hpp"

namespace ham::cpt
{

template <class B>
concept boolean_testable = 
       req_same_as_ignore_cv<bool, B>
    || requires (B&& b)
    {
        { !std::forward<B>(b) } -> std::convertible_to<bool>;
    };

//  Comparability

template <class T>
concept req_equality_comparable = std::equality_comparable<T>;

template <class T, class U>
concept req_equality_comparable_with = std::equality_comparable_with<T, U>;

template <class T>
concept req_totally_ordered = std::totally_ordered<T>;

template <class T, class U>
concept req_totally_ordered_with = std::totally_ordered_with<T, U>;


}