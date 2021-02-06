#pragma once

#include <type_traits>

//  Allow for union/final callables with an overloaded operator() to be 
//  seen as valid callables by req_functor/req_callable

//  HAM_CPT_REGISTER_CALLABLE has no effect on non-final/non-union class types;
//  however, it will still error on invalid signatures, or non class-types


namespace ham::cpt::detail
{

template <class T>
struct is_registered_callable;

// callable_registration_helper
template <class T>
class callable_reg_helper
{
    friend struct is_registered_callable<T>;

    friend consteval bool callable_reg_adl(callable_reg_helper);

    callable_reg_helper() = default;
    callable_reg_helper(callable_reg_helper     &&) = default;
    callable_reg_helper(callable_reg_helper const&) = default;

    ~callable_reg_helper() = default;

    callable_reg_helper& operator=(callable_reg_helper     &&) = default;
    callable_reg_helper& operator=(callable_reg_helper const&) = default;
};

template <class C, class F>
concept has_fn_call_op_with_signature =
    requires
    {
        static_cast<F C::*>(&C::operator());
    };

template <class T, class Signature>
struct register_callable_impl
{
    static constexpr bool is_valid = has_fn_call_op_with_signature<T, Signature>;
};

template <class T, class Signature>
requires 
      (std::is_final_v<T> || std::is_union_v<T>) 
    && std::is_function_v<Signature>
struct register_callable_impl<T, Signature>
{
    static constexpr bool is_valid = has_fn_call_op_with_signature<T, Signature>;

    friend consteval bool callable_reg_adl(callable_reg_helper<T>)
    {
        return is_valid;
    }
};

template <class T, class Signature>
inline constexpr bool register_callable_and_check_validity = 
    register_callable_impl<std::remove_cvref_t<T>, Signature>::is_valid;

//*
template <class T>
class is_registered_callable
{
    template <int... Is>
    requires (sizeof...(Is) == 0)
    static constexpr bool value_ = false;

    template <int... Is>
    requires (callable_reg_adl(callable_reg_helper<std::remove_cvref_t<T>>{}))
    static constexpr bool value_<Is...> = true;

public:
    static constexpr bool value = value_<>;
};
//*/


}