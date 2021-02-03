#pragma once

#include "conceptual/traits/categories.hpp"

//  req_trivial, and req_trivially_copyable can be found in concepts/objects.hpp

namespace ham::cpt
{

template <class T>
concept req_const = std::is_const_v<T>;

template <class T>
concept req_volatile = std::is_volatile_v<T>;


template <class T>
concept req_empty = req_class<T> && std::is_empty_v<T>;

template <class T>
concept req_aggregate = req_array<T> || req_class_or_union<T> && std::is_aggregate_v<T>;

template <class T>
concept req_polymorphic = req_class<T> && requires (T * ptr) { dynamic_cast<void const volatile*>(ptr); };

template <class T>
concept req_monomorphic = req_object<T> && !req_polymorphic<T>;

template <class T>
concept req_monomorphic_class = req_class<T> && !req_polymorphic<T>;

template <class T>
concept req_abstract = req_polymorphic<T> && std::is_abstract_v<T>;

template <class T>
concept req_final = req_class<T> && std::is_final_v<T>;

template <class T>
concept req_standard_layout = 
       req_monomorphic<T>
    && std::is_standard_layout_v<T>;



template <class T>
concept req_signed = 
       req_floating<T> 
    || req_signed_integer<T>
    || (std::is_signed_v<char> && req_same_as_ignore_cv<T, char>);

template <class T>
concept req_unsigned = 
        req_signed_integer<T>
    || (std::is_unsigned_v<char> && req_same_as_ignore_cv<T, char>);

template <class T>
concept req_signed_integral = 
       req_integral<T> 
    && req_signed<T> 
    && std::signed_integral<T>;

template <class T>
concept req_unsigned_integral = 
       req_integral<T> 
    && req_unsigned<T>
    && std::unsigned_integral<T>;




template <class T>
concept req_scoped_enum = 
    req_enum<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

template <class T>
concept req_unscoped_enum = req_enum<T> && !req_scoped_enum<T>;

template <class T>
concept req_unqiue_object_reps = 
    req_object<T> && std::has_unique_object_representations_v<T>;


}