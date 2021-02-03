#pragma once

#include <type_traits>
#include "conceptual/traits/properties.hpp"



namespace ham::cpt::detail
{

#ifdef __cpp_lib_is_layout_compatible

template <class T, class U>
concept req_partial_layout_compatible_with = 
    req_standard_layout<T> && std::is_layout_compatible_v<T, U>;

#endif

}