#pragma once

#include <type_traits>

#include "conceptual/traits/categories.hpp"
#include "conceptual/detail/register_callable.hpp"

#define HAM_CPT_REGISTER_CALLABLE(T, Signature)\
    static_assert(::ham::cpt::detail::register_callable_and_check_validity<T, Signature>,\
    "'" #T "' has no operator() with signature " #Signature);

namespace ham::cpt
{

namespace detail
{

struct fn_call_operator_fallback
{
    void operator()();
};

template <class T>
struct detect_fn_call_operator
{
private:
    using Fallback = fn_call_operator_fallback;
    struct Derived : T, Fallback {};

    
    static std::false_type check()
        requires requires { &Derived::operator(); };

    static std::true_type check();

public:
    static constexpr bool value = decltype(check())::value;
};

template <class T>
requires (!std::is_class_v<T> || std::is_final_v<T>)
struct detect_fn_call_operator<T> : std::bool_constant<is_registered_callable<T>::value>
{

};

template <class T>
concept req_fn_call_operator_impl = 
       detect_fn_call_operator<T>::value
    || requires { std::remove_cvref_t<T>::operator(); };




}

}