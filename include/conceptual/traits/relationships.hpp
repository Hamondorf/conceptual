#pragma once

#include "conceptual/traits/categories.hpp"
#include "conceptual/detail/type_relationships.hpp"

// TODO: Figure out pointer interconvertibility traits and add concepts for them

namespace ham::cpt
{

template <class From, class To>
concept req_static_castable_to =
    std::convertible_to<From, To>
    || requires (std::add_rvalue_reference_t<From>(&f)())
    {
        static_cast<To>(f());
    };

template <class From, class To>
concept req_nothrow_static_castable_to =
    req_static_castable_to<From, To>
    && requires (std::add_rvalue_reference_t<From>(&f)() noexcept)
    {
        {static_cast<To>(f())} noexcept;
    };



template <class From, class To>
concept req_convertible_to = std::convertible_to<From, To>;

template <class From, class To>
concept req_nothrow_convertible_to = 
       req_convertible_to<From, To> 
    && req_nothrow_static_castable_to<From, To>
    && std::is_nothrow_convertible_v<From, To>;



template <class Derived, class Base>
concept req_derived_from = 
       req_class<Derived> 
    && req_class<Base> 
    && std::is_base_of_v<Base, Derived>;


template <class Derived, class Base>
concept req_publicly_derived_from = 
       req_derived_from<Derived, Base> 
    && req_convertible_to<Derived const volatile*, Base const volatile*>;


#ifdef __cpp_lib_is_layout_compatible

template <class T, class U>
concept req_layout_compatible_with = 
    req_same_as_ignore_cv<T, U> ||
    (      detail::req_partial_layout_compatible_with<T, U> 
        && detail::req_partial_layout_compatible_with<U, T>);

#endif

}