#pragma once

#include <type_traits>
#include "conceptual/traits/categories.hpp"


namespace ham::cpt
{

namespace detail
{

struct fn_call_operator_fallback
{
    void operator()();
};

template <class T>
struct detect_fn_call_operator
{
private:
    using Fallback = fn_call_operator_fallback;
    struct Derived : T, Fallback {};

    
    static std::false_type check()
        requires requires { {&Derived::operator()}; };

    static std::true_type check();

public:
    static constexpr bool value = decltype(check())::value;
};

template <class T>
requires (std::is_union_v<T> || std::is_final_v<T>)
struct detect_fn_call_operator<T> 
{

};

}

}