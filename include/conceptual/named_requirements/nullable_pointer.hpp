#pragma once

#include "conceptual/concepts/objects.hpp"
#include "conceptual/concepts/comparison.hpp"
#include "conceptual/traits/supported_ops.hpp"


namespace ham::cpt
{

template <class T>
concept nreq_nullable_pointer =

      req_regular<T> 

   && req_constructible_from<T, std::nullptr_t >
   && req_constructible_from<T, std::nullptr_t&>
   && req_constructible_from<T, std::nullptr_t const >
   && req_constructible_from<T, std::nullptr_t const&>
   
   && req_convertible_to<std::nullptr_t       , T>
   && req_convertible_to<std::nullptr_t      &, T>
   && req_convertible_to<std::nullptr_t const , T>
   && req_convertible_to<std::nullptr_t const&, T>
   
   && req_equality_comparable_with<T , std::nullptr_t>
   
   && req_assignable_from<T&, std::nullptr_t >
   && req_assignable_from<T&, std::nullptr_t&>
   && req_assignable_from<T&, std::nullptr_t const >
   && req_assignable_from<T&, std::nullptr_t const&>;


}