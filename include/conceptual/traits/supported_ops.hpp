#pragma once

#include "conceptual/traits/properties.hpp"



namespace ham::cpt
{


// Destructibility

template <class T>
concept req_destructible = std::destructible<T>;

template <class T>
concept req_destructible_allow_throwing =
    req_destructible<T> || std::is_destructible_v<T>;

template <class T>
concept req_trivially_destructible =
    req_destructible<T> && std::is_trivially_destructible_v<T>;

template <class T>
concept req_virutal_destructor =
    req_polymorphic<T> && std::has_virtual_destructor_v<T>;


// Constructibility

template <class T, class... Args>
concept req_constructible_from = std::constructible_from<T, Args...>;

template <class T, class... Args>
concept req_nothrow_constructible_from = 
    req_constructible_from<T, Args...> && std::is_nothrow_constructible_v<T>;

template <class T, class... Args>
concept req_trivially_constructible_from = 
       req_constructible_from<T, Args...> 
    && req_trivially_destructible<T>
    && std::is_trivially_constructible_v<T>;


// Default

template <class T>
concept req_default_constructible = 
    std::default_initializable<T> || req_constructible_from<T>;

template <class T>
concept req_nothrow_default_constructible = 
    req_default_constructible<T> && req_nothrow_constructible_from<T>;

template <class T>
concept req_trivially_default_constructible = 
    req_default_constructible<T> && req_trivially_constructible_from<T>;


// Default Init

template <class T>
concept req_default_initializable = std::default_initializable<T>;

template <class T>
concept req_nothrow_default_initializable = 
    req_default_initializable<T> && req_nothrow_default_constructible<T>;

template <class T>
concept req_trivially_default_initializable = 
    req_default_initializable<T> && req_trivially_default_constructible<T>;


// Move

template <class T>
concept req_move_constructible = std::move_constructible<T>;

template <class T>
concept req_nothrow_move_constructible = 
    req_move_constructible<T> && req_nothrow_convertible_to<T, T>;

template <class T>
concept req_trivially_move_constructible =
    req_move_constructible<T> && std::is_trivially_move_constructible_v<T>;

// Copy

template <class T>
concept req_copy_constructible = std::copy_constructible<T>;

template <class T>
concept req_nothrow_copy_constructible =
       req_copy_constructible<T>
    && req_nothrow_move_constructible<T>
    && req_nothrow_constructible_from<T, T      &> && req_nothrow_convertible_to<T      &, T> 
    && req_nothrow_constructible_from<T, T const&> && req_nothrow_convertible_to<T const&, T> 
    && req_nothrow_constructible_from<T, T const > && req_nothrow_convertible_to<T const , T>;

template <class T>
concept req_trivially_copy_constructible =
       req_copy_constructible<T>
    && req_trivially_move_constructible<T>
    && req_trivially_constructible_from<T, T      &> 
    && req_trivially_constructible_from<T, T const&> 
    && req_trivially_constructible_from<T, T const >;


// Swappability

template <class T>
concept req_swappable = std::swappable<T>;

template <class T>
concept req_nothrow_swappable = 
    req_swappable<T> && std::is_nothrow_swappable_v<T>;

template <class T, class U>
concept req_swappable_with = std::swappable_with<T, U>;

template <class T, class U>
concept req_nothrow_swappable_with = 
    req_swappable_with<T, U> && std::is_nothrow_swappable_with_v<T, U>;


// Assignability

template <class LHS, class RHS> 
concept req_assignable_from = std::assignable_from<LHS, RHS>;

template <class LHS, class RHS> 
concept req_nothrow_assignable_from =
    req_assignable_from<LHS, RHS> && std::is_nothrow_assignable_v<LHS, RHS>;

template <class LHS, class RHS> 
concept req_trivially_assignable_from =
    req_assignable_from<LHS, RHS> && std::is_trivially_assignable_v<LHS, RHS>;


// Move

template <class T>
concept req_move_assignable = req_assignable_from<T&, T>;

template <class T>
concept req_nothrow_move_assignable = req_nothrow_assignable_from<T&, T>;

template <class T>
concept req_trivially_move_assignable = req_trivially_assignable_from<T&, T>;


//   Copy

template <class T>
concept req_copy_assignable = 
       req_move_assignable<T>
    && req_assignable_from<T&, T      &>
    && req_assignable_from<T&, T const&>
    && req_assignable_from<T&, T const >;

template <class T>
concept req_nothrow_copy_assignable = 
       req_nothrow_move_assignable<T>
    && req_nothrow_assignable_from<T&, T      &>
    && req_nothrow_assignable_from<T&, T const&>
    && req_nothrow_assignable_from<T&, T const >;

template <class T>
concept req_trivially_copy_assignable = 
       req_trivially_move_assignable<T>
    && req_trivially_assignable_from<T&, T      &>
    && req_trivially_assignable_from<T&, T const&>
    && req_trivially_assignable_from<T&, T const >;


}