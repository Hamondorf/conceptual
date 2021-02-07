#pragma once

#include "conceptual/detail/type_categories.hpp"



namespace ham::cpt::detail
{

template <class T>
using make_cref_t = std::remove_reference_t<T> const&;

template <class L, class R>
concept req_no_overloaded_logical_and_or_or_impl =
    requires (L lhs, R rhs)
    {
        requires (!requires { 
            operator&&(std::forward<L>(lhs), std::forward<R>(rhs)); 
        });
        requires (!requires { 
            std::forward<L>(lhs).operator&&(std::forward<R>(rhs)); 
        });
        requires (!requires { 
            operator&&(std::forward<L>(lhs), std::forward<R>(rhs)); 
        });
        requires (!requires { 
            std::forward<L>(lhs).operator&&(std::forward<R>(rhs)); 
        });
    };

template <class L, class R>
concept req_no_overloaded_logical_and_or_or =
       req_no_overloaded_logical_and_or_or_impl<L, R>
    && req_no_overloaded_logical_and_or_or_impl<R, L>;

template <class T, class U = std::remove_const_t<std::remove_reference_t<T>>>
concept req_boolean_testable_impl =
       req_arithmetic<T>
    || std::convertible_to<T, bool>
    && req_no_overloaded_logical_and_or_or<T, bool>
    && req_no_overloaded_logical_and_or_or<T, bool &>
    && req_no_overloaded_logical_and_or_or<T, bool const>
    && req_no_overloaded_logical_and_or_or<T, bool const&>
    && req_no_overloaded_logical_and_or_or<T, U>
    && req_no_overloaded_logical_and_or_or<T, U &>
    && req_no_overloaded_logical_and_or_or<T, U const>
    && req_no_overloaded_logical_and_or_or<T, U const&>
    ;


}