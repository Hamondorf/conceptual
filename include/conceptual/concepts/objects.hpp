#pragma once

#include "conceptual/concepts/comparison.hpp"
#include "conceptual/traits/relationships.hpp"


namespace ham::cpt
{

//  Move

template <class T>
concept req_movable = 
       req_object<T> 
    && req_move_constructible<T>
    && req_move_assignable<T>
    && req_swappable<T>;

template <class T>
concept req_nothrow_movable =
       req_object<T>
    && req_nothrow_move_constructible<T>
    && req_nothrow_move_assignable<T>
    && req_nothrow_swappable<T>;

//  Copy

template <class T>
concept req_copyable =
       req_movable<T>
    && req_copy_constructible<T>
    && req_copy_assignable<T>;

template <class T>
concept req_nothrow_copyable =
       req_nothrow_movable<T>
    && req_nothrow_copy_constructible<T>
    && req_nothrow_copy_assignable<T>;

template <class T>
concept req_trivially_copyable = 
       req_monomorphic<T>
    && (   req_trivially_move_assignable<T>
        || req_trivially_move_constructible<T>
        || req_trivially_copy_assignable<T>
        || req_trivially_copy_constructible<T>) 
    && std::is_trivially_copyable_v<T>;

template <class T>
concept req_trivial = req_trivially_copyable<T> && req_trivially_default_initializable<T>;


//   Regularity

template <class T>
concept req_semiregular = req_copyable<T> && req_default_initializable<T>;

template <class T>
concept req_regular = req_semiregular<T> && req_equality_comparable<T>;


}