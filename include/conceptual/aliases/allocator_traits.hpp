#pragma once

#include <memory>



namespace ham::cpt
{
inline namespace alloc_traits
{

#define HAM_CPT_ALLOC_TRAITS_ALIAS(alias_suffix, aliased_type) \
template <class Alloc> \
using alloc_ ## alias_suffix = typename std::allocator_traits<Alloc>:: aliased_type

HAM_CPT_ALLOC_TRAITS_ALIAS(alloc_t, allocator_type);
HAM_CPT_ALLOC_TRAITS_ALIAS(value_t, value_type);
HAM_CPT_ALLOC_TRAITS_ALIAS(ptr_t, pointer);
HAM_CPT_ALLOC_TRAITS_ALIAS(const_ptr_t, const_pointer);
HAM_CPT_ALLOC_TRAITS_ALIAS(void_ptr_t, void_pointer);
HAM_CPT_ALLOC_TRAITS_ALIAS(const_void_ptr_t, const_void_pointer);
HAM_CPT_ALLOC_TRAITS_ALIAS(size_t, size_type);
HAM_CPT_ALLOC_TRAITS_ALIAS(difference_t, difference_type);
HAM_CPT_ALLOC_TRAITS_ALIAS(prop_on_cntr_copy_t, propagate_on_container_copy_assignment);
HAM_CPT_ALLOC_TRAITS_ALIAS(prop_on_cntr_move_t, propagate_on_container_move_assignment);
HAM_CPT_ALLOC_TRAITS_ALIAS(prop_on_cntr_swap_t, propagate_on_container_swap);
HAM_CPT_ALLOC_TRAITS_ALIAS(always_equal_t, is_always_equal);

template <class Alloc, class T>
using alloc_rebind_t = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;

template <class Alloc, class T>
using alloc_rebind_traits_t = typename std::allocator_traits<Alloc>::template rebind_traits<T>;

#undef HAM_CPT_ALLOC_TRAITS_ALIAS
}

}