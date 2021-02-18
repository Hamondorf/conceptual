#pragma once

#include "conceptual/aliases/numeric_limits.hpp"
#include "conceptual/traits/properties.hpp"
#include "conceptual/detail/numeric_limits_specialized.hpp"




namespace ham::cpt
{

template <class T>
concept nlim_specialized_for =
       req_arithmetic<T>
    || req_totally_ordered<T>
    && req_regular<T>
    && detail::nlim_specialized_for_impl<T>;

template <class T>
concept nlim_signed = 
    req_signed<T> || nlim_specialized_for<T> && nlim_is_signed_v<T>;


}