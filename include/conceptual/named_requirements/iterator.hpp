#pragma once

#include <iterator>

#include "conceptual/concepts/objects.hpp"
#include "conceptual/named_requirements/comparison.hpp"
#include "conceptual/traits/negated/categories.hpp"
#include "conceptual/traits/supported_ops.hpp"

namespace ham::cpt
{

template <class T>
concept req_referenceable = non_void<T> || requires { typename std::type_identity_t<T&>; };

template <class T>
concept req_dereferenceable = requires (T& t) { {*t} -> req_referenceable; };

template <class I>
concept nreq_legacy_iterator = 
       std::input_or_output_iterator<I>
    && req_copyable<I>
    && requires (I i)
    {
        {i++}->req_dereferenceable;
    };

template <class I>
concept nreq_legacy_input_iterator =
       std::input_iterator<I>
    && nreq_legacy_input_iterator<I>
    && nreq_equality_comparable<I>;

template <class I, class T>
concept nreq_legacy_output_iterator =
       std::output_iterator<I, T>
    && nreq_legacy_iterator<I>;

template <class I>
concept nreq_legacy_forward_iterator =
       std::forward_iterator<I>
    && req_default_constructible<I>
    && nreq_legacy_input_iterator<I>;

template <class I>
concept nreq_legacy_bidirectional_iterator =
       std::bidirectional_iterator<I>
    && nreq_legacy_forward_iterator<I>;

template <class I>
concept nreq_legacy_random_access_iterator =
       std::random_access_iterator<I>
    && nreq_legacy_bidirectional_iterator<I>;

template <class I>
concept nreq_legacy_contiguous_iterator =
       std::contiguous_iterator<I>
    && nreq_legacy_random_access_iterator<I>;




}