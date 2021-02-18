#pragma once

#include <type_traits>

//  Allow for union/final callables with an overloaded operator() to be 
//  seen as valid callables by req_functor/req_callable

//  HAM_CPT_REGISTER_CALLABLE has no effect on non-final/non-union class types;
//  however, it will still error on invalid signatures, or non class-types


namespace ham::cpt::detail
{

template <class T>
class is_registered_callable;

// callable_registration_helper
template <class T>
class callable_reg_helper
{
    friend class is_registered_callable<T>;

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

    static_assert(std::is_class_v<T> || std::is_union_v<T>, "'T' must be a class type.");
    static_assert(std::is_function_v<Signature>, "'Signature' must be a function type.");
    static_assert(is_valid, "Invalid callable registration.");
};

template <class T, class Signature>
requires 
      (std::is_final_v<T> || std::is_union_v<T>) 
    && std::is_function_v<Signature>
struct register_callable_impl<T, Signature>
{
    static constexpr bool is_valid = has_fn_call_op_with_signature<T, Signature>;

    static_assert(std::is_class_v<T> || std::is_union_v<T>, "'T' must be a class type.");
    static_assert(std::is_function_v<Signature>, "'Signature' must be a function type.");
    static_assert(is_valid, "Invalid callable registration.");

    friend consteval bool callable_reg_adl(callable_reg_helper<T>)
    {
        return is_valid;
    }
};

template <class T, class Signature>
using register_callable_and_check_validity = 
    std::bool_constant<
        register_callable_impl<std::remove_cvref_t<T>, Signature>::is_valid
    >;

//*
template <class T>
class is_registered_callable
{
    template <std::nullptr_t = 0>
    static constexpr bool value_ = false;

    template <std::nullptr_t np>
    requires (callable_reg_adl(callable_reg_helper<std::remove_cvref_t<T>>{}))
    static constexpr bool value_<np> = true;

public:
    static constexpr bool value = value_<>;
};
//*/


}