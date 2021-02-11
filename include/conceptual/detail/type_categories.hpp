#pragma once

#include <concepts>
#include <type_traits>

#include "same_as.hpp"

#ifdef HAM_CPT_EXTENDED_SUBSUMPTION
#   define HAM_CPT_SUBSUMPTION_ON(...) __VA_ARGS__
#   define HAM_CPT_SUBSUMPTION_OFF(...)
#else
#   define HAM_CPT_SUBSUMPTION_ON(...)
#   define HAM_CPT_SUBSUMPTION_OFF(...) __VA_ARGS__
#endif


namespace ham::cpt
{

namespace detail
{

template <class T, class... Types>
using remove_cv_same_as_any_of [[maybe_unused]] 
    = std::bool_constant<(req_same_as<std::remove_cv_t<T>, Types> || ...)>;

template <class T>
concept req_void_impl = 
    HAM_CPT_SUBSUMPTION_ON(req_same_as_ignore_cv<T, void>)
    HAM_CPT_SUBSUMPTION_OFF(std::is_void_v<T>);

template <class T>
concept req_null_pointer_impl = 
    HAM_CPT_SUBSUMPTION_ON(req_same_as_ignore_cv<T, std::nullptr_t>)
    HAM_CPT_SUBSUMPTION_OFF(std::is_null_pointer_v<T>);


template <class T>
concept req_narrow_char_impl = std::integral<T> &&
    HAM_CPT_SUBSUMPTION_ON(  
       req_same_as_ignore_cv<T, char>
    || req_same_as_ignore_cv<T, signed   char>
    || req_same_as_ignore_cv<T, unsigned char>
    || req_same_as_ignore_cv<T, char8_t>
    )
    HAM_CPT_SUBSUMPTION_OFF(
        remove_cv_same_as_any_of<T, char, signed char, unsigned char, char8_t>::value
    );

template <class T>
concept req_wide_char_impl = std::integral<T> &&
    HAM_CPT_SUBSUMPTION_ON((
       req_same_as_ignore_cv<T, char16_t>
    || req_same_as_ignore_cv<T, char32_t>
    || req_same_as_ignore_cv<T, wchar_t>
    ))
    HAM_CPT_SUBSUMPTION_OFF(
        remove_cv_same_as_any_of<T, char16_t, char32_t, wchar_t>::value
    );

template <class T>
concept req_boolean_impl = std::integral<T> && 
    HAM_CPT_SUBSUMPTION_ON(req_same_as_ignore_cv<T, bool>)
    HAM_CPT_SUBSUMPTION_OFF(remove_cv_same_as_any_of<T, bool>::value);

//  signed char is considered a signed integer type 
//  lest req_signed_integer<std::int8_t> return false as int8_t
//  is often an alias for it
template <class T>
concept req_signed_integer_impl = std::integral<T> &&
    HAM_CPT_SUBSUMPTION_ON((
       req_same_as_ignore_cv<T, signed char>
    || req_same_as_ignore_cv<T, signed short>          
    || req_same_as_ignore_cv<T, signed int>
    || req_same_as_ignore_cv<T, signed long>
    || req_same_as_ignore_cv<T, signed long long>
    ))
    HAM_CPT_SUBSUMPTION_OFF(
        remove_cv_same_as_any_of<T, 
            signed char, 
            signed short, 
            signed int, 
            signed long, 
            signed long long>::value
    );

//  unsigned char is considered a signed integer type 
//  lest req_signed_integer<std::uint8_t> return false as uint8_t
//  is often an alias for it
template <class T>
concept req_unsigned_integer_impl = std::integral<T> &&
    HAM_CPT_SUBSUMPTION_ON((
       req_same_as_ignore_cv<T, unsigned char>
    || req_same_as_ignore_cv<T, unsigned short>          
    || req_same_as_ignore_cv<T, unsigned int>
    || req_same_as_ignore_cv<T, unsigned long>
    || req_same_as_ignore_cv<T, unsigned long long>
    ))
    HAM_CPT_SUBSUMPTION_OFF(
        remove_cv_same_as_any_of<T, 
            unsigned char, 
            unsigned short, 
            unsigned int, 
            unsigned long, 
            unsigned long long>::value
    );

template <class T>
concept req_integral_impl =
    HAM_CPT_SUBSUMPTION_ON(
       req_char_type<T>
    || req_boolean<T>
    || req_signed_integer<T>
    || req_unsigned_integer<T>
    )
    HAM_CPT_SUBSUMPTION_OFF(std::integral<T>);


template <class T>
concept req_floating_impl = 
    HAM_CPT_SUBSUMPTION_ON((
       req_same_as_ignore_cv<T, float>
    || req_same_as_ignore_cv<T, double>
    || req_same_as_ignore_cv<T, long double> ) &&
    )
    std::floating_point<T>;

}

}