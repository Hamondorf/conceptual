#pragma once

#include <concepts>

#include "conceptual/detail/callable.hpp"


namespace ham::cpt
{

template <class T>
concept req_functor = 
    req_class_type<T> && detail::req_fn_call_operator_impl<T>;

template <class T>
concept req_function_object = 
    req_object<T> && (req_functor<T> || req_function_pointer<T>);

template <class T>
concept req_callable =
       req_function_object<T> || req_function_reference<T> || req_member_pointer<T>;


template <class F, class... Args> 
concept req_invocable = std::invocable<F, Args...>;

template <class F, class... Args> 
concept req_regular_invocable = std::regular_invocable<F, Args...>;


}