#pragma once

#include <concepts>


namespace ham::cpt
{

template <class T, class U>
concept req_same_as = std::same_as<T, U>;


namespace detail
{

template <class T, class U, 
    template <class> class Add, template <class> class Remove>
concept req_same_as_ignore_impl =
        req_same_as<T, U>
    || req_same_as<Add<T>, Add<U>> && req_same_as<Remove<T>, Remove<U>>
    || (
           (req_same_as<T, Add<U>> || req_same_as<T, Remove<U>>)
        && (req_same_as<U, Add<T>> || req_same_as<U, Remove<T>>));
}

// TODO: Test these concepts for proper subsumption

template <class T, class U>
concept req_same_as_ignore_const = 
    detail::req_same_as_ignore_impl<T, U, 
        std::add_const_t, std::remove_const_t>;


template <class T, class U>
concept req_same_as_ignore_volatile = 
    detail::req_same_as_ignore_impl<T, U, 
        std::add_volatile_t, std::remove_volatile_t>;

template <class T, class U>
concept req_same_as_ignore_cv = 
       detail::req_same_as_ignore_impl<T, U, std::add_cv_t, std::remove_cv_t>
    || req_same_as_ignore_const<T, U> || req_same_as_ignore_volatile<T, U>;




}