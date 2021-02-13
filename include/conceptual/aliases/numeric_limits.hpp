#pragma once

#include <limits>
#include <type_traits>


namespace ham::cpt
{

namespace detail
{

template <class T, auto value>
struct nlim_constant_impl
{};

template <class T, auto value>
requires std::is_arithmetic_v<decltype(value)>
struct nlim_constant_impl<T, value>
    : std::integral_constant<decltype(value), value>
{   };

template <class T, T (*fn)() noexcept>
requires (fn != nullptr) && std::is_arithmetic_v<T>
struct nlim_constant_impl<T, fn>
    : std::integral_constant<T, fn()>
{   };

template <class T, T (*fn)() noexcept>
requires (fn != nullptr) && (!std::is_arithmetic_v<T>)
struct nlim_constant_impl<T, fn>
{
    using value_type = T;
    using type       = nlim_constant_impl;

    static constexpr T value = fn();

    static constexpr operator value_type()   const noexcept { return value; }
    static constexpr value_type operator()() const noexcept { return value; }
};

template <class T, auto value_or_fn>
using nlim_constant = nlim_constant_impl<T, value_or_fn>;

}

inline namespace numeric_limits
{

template <class T>
using nlim = std::numeric_limits<T>;

#define HAM_CPT_NLIM_ALIAS(name) \
template <class T> \
using nlim_ ## name = detail::nlim_constant<std::remove_cv_t<T>, nlim<T>:: name>; \
template <class T> \
inline constexpr auto nlim_ ## name ## _v = nlim_ ## name<T>::value


HAM_CPT_NLIM_ALIAS(is_specialized);
HAM_CPT_NLIM_ALIAS(is_signed);
HAM_CPT_NLIM_ALIAS(is_integer);
HAM_CPT_NLIM_ALIAS(is_exact);
HAM_CPT_NLIM_ALIAS(is_iec559);
HAM_CPT_NLIM_ALIAS(is_bounded);
HAM_CPT_NLIM_ALIAS(is_modulo);

HAM_CPT_NLIM_ALIAS(has_infinity);
HAM_CPT_NLIM_ALIAS(has_quiet_NaN);
HAM_CPT_NLIM_ALIAS(has_signaling_NaN);
HAM_CPT_NLIM_ALIAS(has_denorm_loss);

HAM_CPT_NLIM_ALIAS(traps);
HAM_CPT_NLIM_ALIAS(tinyness_before);

HAM_CPT_NLIM_ALIAS(digits);
HAM_CPT_NLIM_ALIAS(digits10);
HAM_CPT_NLIM_ALIAS(max_digits10);

HAM_CPT_NLIM_ALIAS(radix);

HAM_CPT_NLIM_ALIAS(min_exponent);
HAM_CPT_NLIM_ALIAS(min_exponent10);
HAM_CPT_NLIM_ALIAS(max_exponent);
HAM_CPT_NLIM_ALIAS(max_exponent10);

HAM_CPT_NLIM_ALIAS(has_denorm);
HAM_CPT_NLIM_ALIAS(round_style);

HAM_CPT_NLIM_ALIAS(min);
HAM_CPT_NLIM_ALIAS(max);
HAM_CPT_NLIM_ALIAS(lowest);

HAM_CPT_NLIM_ALIAS(epsilon);
HAM_CPT_NLIM_ALIAS(round_error);
HAM_CPT_NLIM_ALIAS(infinity);
HAM_CPT_NLIM_ALIAS(quiet_NaN);
HAM_CPT_NLIM_ALIAS(signaling_NaN);
HAM_CPT_NLIM_ALIAS(denorm_min);



#undef HAM_CPT_NLIM_ALIAS


}

}