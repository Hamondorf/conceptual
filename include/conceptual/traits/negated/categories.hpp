#pragma once

#include "conceptual/traits/categories.hpp"


//  Together all of the type categories form a bounded set that is 
//  the set of all types. That is to say every type can be categorized
//  into a single type category (fundamental or composite), and as such
//  the negation of a category can be defined as the union of every
//  other type category that does not including itself.

namespace ham::cpt
{

template <class T>
concept non_compound = req_fundamental<T>;

template <class T>
concept non_fundamental = req_compound<T>;

template <class T>
concept non_object = 
       req_void<T>
    || req_function<T>
    || req_reference<T>;

template <class T> 
concept non_scalar =
       non_object<T>
    || req_array<T>
    || req_class_or_union<T>;



template <class T>
concept non_reference = 
    non_compound<T> || req_function<T> || req_object<T>;

template <class T>
concept non_pointer =
    non_compound<T> || req_member_pointer<T> || non_scalar<T>;

template <class T> 
concept non_member_pointer =
    non_compound<T> || req_pointer<T> || non_scalar<T>;

template <class T>
concept non_array =
       non_object<T>
    || req_scalar<T>
    || req_class_or_union<T>;

template <class T>
concept non_function = 
    req_void<T> || req_object<T> || req_reference<T>;

template <class T>
concept non_enum =
       non_compound<T>
    || non_scalar<T>
    || req_pointer<T>
    || req_member_pointer<T>;



template <class T>
concept non_class_type =
    non_object<T> || req_scalar<T> || req_array<T>;

template <class T>
concept non_class = non_class_type<T> || req_union<T>;

template <class T>
concept non_union = non_class_type<T> || req_class<T>;



template <class T>
concept non_void = 
    non_fundamental<T> || req_null_pointer<T> || req_arithmetic<T>;

template <class T>
concept non_null_pointer =
    non_fundamental<T> || req_void<T> || req_arithmetic<T>;

template <class T>
concept non_arithmetic = 
    req_void<T> || req_null_pointer<T> || non_fundamental<T>;

template <class T>
concept non_integral = non_arithmetic<T> || req_floating<T>;

template <class T>
concept non_floating = non_arithmetic<T> || req_integral<T>;



}
