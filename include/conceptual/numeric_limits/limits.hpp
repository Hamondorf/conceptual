#pragma once

#include "conceptual/aliases/numeric_limits.hpp"
#include "conceptual/traits/properties.hpp"
#include "conceptual/detail/numeric_limits_specialized.hpp"




namespace ham::cpt
{
namespace detail
{

template <class T>
concept nlim_specialized_for = 
          req_totally_ordered<T>
       && req_regular<T>
       && detail::nlim_specialized_for_impl<T>;

}

template <class T>
concept nlim_specialized_for =
    req_arithmetic<T> || detail::nlim_specialized_for<T>;

template <class T>
concept nlim_signed = 
    req_signed<T> || detail::nlim_specialized_for<T> && nlim_is_signed_v<T>;

template <class T>
concept nlim_unsigned = 
    req_unsigned<T> || detail::nlim_specialized_for<T> && nlim_is_unsigned_v<T>;





}