#pragma once

#include <memory>

#include "conceptual/aliases/allocator_traits.hpp"
#include "conceptual/aliases/pointer_traits.hpp"
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


template <class T>
concept req_valid_allocator_ptr = 
    nreq_nullable_pointer<T> && nreq_legacy_bidirectional_iterator<T>;



template <class A,
    class T              = alloc_value_t<A>,
    class Ptr            = alloc_ptr_t<A>,
    class ConstPtr       = alloc_const_ptr_t<A>,
    class VoidPtr        = alloc_void_ptr_t<A>,
    class ConstVoidPtr   = alloc_const_void_ptr_t<A>
>
concept req_alloc_impl_validate_ptr_ops =
       req_static_castable_to<VoidPtr, Ptr>
    && req_static_castable_to<ConstVoidPtr, ConstPtr>
    && requires (Ptr p, ConstPtr cp, VoidPtr vp, ConstVoidPtr cvp)//, decltype(*p) r)
    {
        {*p} -> req_same_as<T&>;


    };
    

template <
    class A,
    class T              = alloc_value_t<A>,
    class B              = alloc_rebind_t<A, decltype([]<class = A>(){})>,
    class U              = alloc_value_t<T>,
    class Ptr            = alloc_ptr_t<A>,
    class ConstPtr       = alloc_const_ptr_t<A>,
    class VoidPtr        = alloc_void_ptr_t<A>,
    class ConstVoidPtr   = alloc_const_void_ptr_t<A>,
    class SizeType       = alloc_size_t<A>,
    class DifferenceType = alloc_difference_t<A>
>
concept req_alloc_impl_inner_types =
       req_same_as<T, typename A::value_type>
    && req_valid_allocator_ptr<Ptr>
    && req_convertible_to<Ptr, ConstPtr>
    && req_convertible_to<Ptr, VoidPtr>
    && req_convertible_to<Ptr, ConstVoidPtr>
    
    && req_valid_allocator_ptr<ConstPtr>
    && req_convertible_to<ConstPtr, ConstVoidPtr>

    && req_valid_allocator_ptr<VoidPtr>
    && req_convertible_to<VoidPtr, ConstVoidPtr>

    && req_valid_allocator_ptr<ConstVoidPtr>

    && req_same_as<A, alloc_rebind_t<B, T>>
    && req_same_as<VoidPtr, alloc_void_ptr_t<B>>
    && req_same_as<ConstVoidPtr, alloc_const_void_ptr_t<B>>
    
    && req_signed_integral<DifferenceType>
    && req_unsigned_integral<SizeType>
    ;

}


}