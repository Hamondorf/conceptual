#pragma once

#include "conceptual/concepts/comparison.hpp"



namespace ham::cpt
{

template <class T>
concept nreq_equality_comparable = req_equality_comparable<T>;

template <class T>
concept nreq_less_than_comparable =
       req_totally_ordered<T>
    || requires (detail::make_cref_t<T> a, detail::make_cref_t<T> b)
    {
        {a < b} -> req_boolean_testable;
    };

}