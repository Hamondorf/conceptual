#pragma once

#include <concepts>
#include <type_traits>

#include "conceptual/detail/same_as.hpp"


namespace ham::cpt
{


template <class T>
concept req_void = req_same_as_ignore_cv<T, void>;

template <class T>
concept req_null_pointer = req_same_as_ignore_cv<T, std::nullptr_t>;


template <class T>
concept req_narrow_char =
       req_same_as_ignore_cv<T, char>
    || req_same_as_ignore_cv<T, signed   char>
    || req_same_as_ignore_cv<T, unsigned char>
    || req_same_as_ignore_cv<T, char8_t>;

template <class T>
concept req_wide_char =
       req_same_as_ignore_cv<T, char16_t>
    || req_same_as_ignore_cv<T, char32_t>
    || req_same_as_ignore_cv<T, wchar_t>;

//  Requires Character Type
//  constrains T to be a standard character type
template <class T>
concept req_char_type = req_narrow_char<T> || req_wide_char<T>;

template <class T>
concept req_boolean = req_same_as_ignore_cv<T, bool>;

//  signed char is considered a signed integer type 
//  lest req_signed_integer<std::int8_t> return false as int8_t
//  is often an alias for it
template <class T>
concept req_signed_integer =
       req_same_as_ignore_cv<T, signed char>
    || req_same_as_ignore_cv<T, signed short>          
    || req_same_as_ignore_cv<T, signed int>
    || req_same_as_ignore_cv<T, signed long>
    || req_same_as_ignore_cv<T, signed long long>;

//  unsigned char is considered a signed integer type 
//  lest req_signed_integer<std::uint8_t> return false as uint8_t
//  is often an alias for it
template <class T>
concept req_unsigned_integer =
       req_same_as_ignore_cv<T, unsigned char>
    || req_same_as_ignore_cv<T, unsigned short>          
    || req_same_as_ignore_cv<T, unsigned int>
    || req_same_as_ignore_cv<T, unsigned long>
    || req_same_as_ignore_cv<T, unsigned long long>;

//  Requires Integral
// 
//  subsumes: req_same_as<T, [cv-qualified integral type]> and std::integral<T>
template <class T>
concept req_integral =
      (req_char_type<T>
    || req_boolean<T>
    || req_signed_integer<T>
    || req_unsigned_integer<T>)
    && std::integral<T>;

template <class T>
concept req_floating = 
      (req_same_as_ignore_cv<T, float>
    || req_same_as_ignore_cv<T, double>
    || req_same_as_ignore_cv<T, long double>)
    && std::floating_point<T>;


template <class T>
concept req_array = std::is_array_v<T>;

template <class T>
concept req_enum = std::is_enum_v<T>;

template <class T>
concept req_class = std::is_class_v<T>;

template <class T>
concept req_struct = req_class<T>;

template <class T>
concept req_union = std::is_union_v<T>;

template <class T>
concept req_function = std::is_function_v<T>;

template <class T>
concept req_pointer = std::is_pointer_v<T>;

template <class T>
concept req_lval_ref = std::is_lvalue_reference_v<T>;

template <class T>
concept req_rval_ref = std::is_rvalue_reference_v<T>;

template <class T>
concept req_member_object_ptr = std::is_member_object_pointer_v<T>;

template <class T>
concept req_member_function_ptr = std::is_member_function_pointer_v<T>;


// Composite Type Categories

template <class T>
concept req_member_pointer = req_member_function_ptr<T> || req_member_object_ptr<T>;

template <class T>
concept req_arithmetic = req_floating<T> || req_integral<T>;

template <class T>
concept req_fundamental = req_void<T> || req_null_pointer<T> || req_arithmetic<T>;

template <class T>
concept req_scalar = 
       req_arithmetic<T> 
    || req_enum<T> 
    || req_pointer<T> 
    || req_member_pointer<T> 
    || req_null_pointer<T>;

template <class T>
concept req_reference = req_lval_ref<T> || req_rval_ref<T>;

template <class T>
concept req_class_or_union = req_class<T> || req_union<T>;

template <class T>
concept req_object = req_class_or_union<T> || req_scalar<T> || req_array<T>;

template <class T>
concept req_compound = 
       req_reference<T>
    || req_pointer<T> 
    || req_member_pointer<T> 
    || req_array<T>
    || req_function<T>
    || req_enum<T>
    || req_class_or_union<T>;

}