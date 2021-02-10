#pragma once

#include <memory>

#include "conceptual/aliases/allocator_traits.hpp"
#include "conceptual/named_requirements/nullable_pointer.hpp"
#include "conceptual/named_requirements/iterator.hpp"
#include "conceptual/traits/relationships.hpp"



namespace ham::cpt
{

namespace detail
{

template <class T, bool b>
concept req_derived_from_bool_constant_impl =
        req_publicly_derived_from<T, std::bool_constant<b>> 
    && !req_derived_from<T, std::bool_constant<!b>>;

template <class T>
concept req_derived_from_bool_constant =
       req_derived_from_bool_constant_impl<T,  true>
    || req_derived_from_bool_constant_impl<T, false>;


template <class...>
concept req_allocator_ptr_ops = false;

template <
    class A,
    class T   = alloc_value_t<A>,
    class B = alloc_rebind_t<A, decltype([]<class = A>(){})>,
    class U = alloc_value_t<T>,
    class Ptr = alloc_ptr_t<A>,
    class ConstPtr = alloc_const_ptr_t<A>,
    class VPtr = alloc_void_ptr_t<A>,
    class ConstVPtr = alloc_const_void_ptr_t<A>

>
concept req_allocator_impl = false;

}

constexpr void f(int x, decltype(x) y);

}