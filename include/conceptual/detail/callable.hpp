#pragma once

#include <type_traits>

#include "conceptual/traits/categories.hpp"
#include "conceptual/detail/register_callable.hpp"





/**
 *  Allows for union/final class types to be registered as callables by
 *  providing a single valid signature for their operator().
 *  
 *  @param T - class being registered
 *  @param Signature - Any valid signature for T::operator()
 *  
 *  @note Registering a non-final/union class types has no effect; however,
 *  the static_assert will still fire if there is no valid operator() for T
 *  with the given signature. 
 */
#define HAM_CPT_REGISTER_CALLABLE(T, Signature)\
    static_assert(::std::is_class_v<T> || ::std::is_union_v<T>,\
         "'T' must be a class type. (T is '" #T "')" );\
    static_assert(::std::is_function_v<Signature>, \
        "'Signature' must be a fuction type. (Signature is '" #Signature "')");\
    static_assert(\
        ::ham::cpt::detail::register_callable_and_check_validity<T, Signature>{},\
        "'" #T "' has no operator() with signature " #Signature)

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