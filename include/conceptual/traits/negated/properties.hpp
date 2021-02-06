#pragma once

#include "conceptual/traits/negated/properties.hpp"


//  At this time not every single property has a negated form, only the ones
//  I anticipate being the most useful.

namespace ham::cpt
{

template <class T>
concept non_const = !req_const<T>;

template <class T>
concept non_volatile = !req_volatile<T>;

template <class T>
concept req_cv_unqualified = non_const<T> && non_volatile<T>;

template <class T>
concept req_cvref_unqualified =
    non_reference<T> && req_cv_unqualified<std::remove_reference_t<T>>;

}