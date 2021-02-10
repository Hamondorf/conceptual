#pragma once

#include <memory>


namespace ham::cpt
{
inline namespace ptr_traits
{

template <class Ptr>
using ptr_pointer_t = std::pointer_traits<Ptr>::pointer;

template <class Ptr> 
using ptr_element_t = std::pointer_traits<Ptr>::element_type;

template <class Ptr>
using ptr_difference_t = std::pointer_traits<Ptr>::difference_type;

template <class Ptr, class T>
using ptr_rebind_t = typename std::pointer_traits<Ptr>::template rebind<T>;
}

}